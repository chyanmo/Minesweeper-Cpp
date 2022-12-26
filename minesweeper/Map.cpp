#include "Map.h"
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

Map::Map(int ROW, int COL, int Mine)
{
    resize(ROW,COL,Mine);
    pre_init();
}

Map::~Map()
{
    clear();
}

// r, c 是坐标，对 (r,c) 周围的一圈，包括它本身执行 fn(x,y)，返回值是 fn(r+1, c+1) 的返回值 
template<typename Function>
auto Map::for_around(int r, int c, Function fn)
{
    for (int k = r - 1; k < r + 1; k++)
        for (int l = c - 1; l <= c + 1; l++)
            fn(k, l);
    fn(r + 1, c - 1);
    fn(r + 1, c);
    return fn(r + 1, c + 1);
}

void Map::pre_init()
{
    // 分配空间
    const int n = m_row * m_col;
    mine_map = new bool[n]();
    display_map = new int[n];
    find_map = new bool[n]();
    mark_map = new bool[n]();
    // 布雷
    std::for_each(mine_map + n - m_mine, mine_map + n, [](bool& x) {x = 1; });
}

void Map::init(int click_r, int click_c)
{
    int total = for_around(click_r, click_c,
        [this, count = 0](int r, int c)mutable {count += is_in(r, c); return (int)count; }
    );
    if (m_row * m_col < m_mine + total) total = 0;

    // 将 mine_map 打乱
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(mine_map + total, mine_map + m_row * m_col, std::default_random_engine(seed));

    // 保证点的地方周围没有地雷 ( 如果可能的话 )
    for_around(click_r, click_c,
        [this, count = 0](int r, int c)mutable {if (is_in(r, c)) std::swap(mine_map[r * m_col + c], mine_map[count++]); }
    );

    // 生成 displayed_map
    for (int i = 0; i < m_row; i++)
        for (int j = 0; j < m_col; j++)
            display_map[i * m_col + j] = for_around(i, j,
                [this, count = 0](int r, int c)mutable {count += is_mine(r, c); return (int)count; });

#ifdef DEBUG
    debug();
#endif // DEBUG
}

void Map::clear()
{
    delete[] mine_map;
    delete[] display_map;
    delete[] find_map;
    delete[] mark_map;
    display_map = nullptr;
    mark_map = mine_map = find_map = nullptr;
}

unsigned short Map::stat(unsigned i)
{
    if (mark_map[i])return 10;
    if (!(find_map[i]))return 11;
    if (mine_map[i])return 9;
    return display_map[i];
}

void Map::resize(int ROW, int COL, int Mine)
{
    m_row = ROW;
    m_col = COL;
    m_mine = (Mine < ROW * COL) ? (Mine) : (ROW * COL);
}

bool Map::is_in(int r, int c)
{
    return (r >= 0) && (c >= 0) && (r < m_row) && (c < m_col);
}

bool Map::is_mine(int r, int c)
{
    return (is_in(r, c)) && (mine_map[r * m_col + c]);
}

bool Map::is_marked(int r, int c)
{
    return (is_in(r, c)) && (mark_map[r * m_col + c]);
}

void Map::right_click(int r, int c)
{
    // 作用于点开的格子
    if (find_map[r * m_col + c])
        try_open_around(r, c);
    // 作用于没点开的格子，标记或取消标记
    else
        mark_map[r * m_col + c] = !(mark_map[r * m_col + c]);
}

void Map::left_click(int r, int c)
{
    // 被标记的，取消标记
    if (mark_map[r * m_col + c])
        mark_map[r * m_col + c] = false;

    // 已经点开的，尝试点开周围
    else if (find_map[r * m_col + c])
        try_open_around(r, c);

    else if (display_map[r * m_col + c] == 0)
        zero_extend(r, c);

    else find_map[r * m_col + c] = true;
}

void Map::try_open_around(int r, int c) {

    // 统计周围有标记的格子数
    int mark_count = for_around(r, c, [this, count = 0](int r, int c)mutable {count += is_marked(r, c); return (int)count; });

    // 假设标记的格子数不少于显示的数字，点开其他格子
    if (mark_count >= display_map[r * m_col + c])
    {
        for (int i = r - 1; i <= r + 1; i++)
            for (int j = c - 1; j <= c + 1; j++)
                if (is_in(i, j) && !(find_map[i * m_col + j]) && !(mark_map[i * m_col + j]))
                    left_click(i, j);
    }
}

void Map::zero_extend(int r, int c)
{
    // 首先排除：不在地图内的 / 已经找过的 / 做标记的
    if (!is_in(r, c) || (find_map[r * m_col + c]) || (mark_map[r * m_col + c]))
        return;

    // 记下，防止重复找
    find_map[r * m_col + c] = true;

    // 不满足条件
    if (display_map[r * m_col + c] != 0)
        return;

    for (int i = r - 1; i <= r + 1; i++)
        for (int j = c - 1; j <= c + 1; j++)
            zero_extend(i, j);
}

#ifdef DEBUG
void Map::debug()
{
    std::cout << "Mine map:" << std::endl;
    for (int r = 0; r < m_row; r++) {
        for (int c = 0; c < m_col; c++)
            std::cout << mine_map[r * m_col + c] << " ";
        std::cout << std::endl;
    }

    std::cout << "Display map:" << std::endl;
    for (int r = 0; r < m_row; r++) {
        for (int c = 0; c < m_col; c++){
            if (mine_map[r * m_col + c])
                std::cout << "x ";
            else
                std::cout << display_map[r * m_col + c] << " ";
        }
        std::cout << std::endl;
    }
    /*
    std::cout << "Find map:" << std::endl;
    for (int r = 0; r < m_row; r++) {
        for (int c = 0; c < m_col; c++)
            std::cout << find_map[r * m_col + c] << " ";
        std::cout << std::endl;
    }
    */
}
#endif

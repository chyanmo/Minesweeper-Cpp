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

bool Map::empty_buffer()
{
    return buffer.empty();
}

std::pair<short, unsigned> Map::front_buffer()
{
    return buffer.front();
}

void Map::pop_buffer()
{
    buffer.pop();
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
    unsigned i = r * m_col + c;
    // 作用于点开的格子
    if (find_map[i])
        try_open_around(r, c);
    // 作用于没点开的格子，标记或取消标记
    else if (mark_map[i]) {
        mark_map[i] = false;
        buffer.push(std::make_pair((short)(-2), i));
    }
    else if (!(mark_map[i])) {
        mark_map[i] = true;
        buffer.push(std::make_pair((short)(-1), i));
    }
}

void Map::left_click(int r, int c)
{
    unsigned i = r * m_col + c;
    // 被标记的，取消标记
    if (mark_map[i]) {
        mark_map[i] = false;
        buffer.push(std::make_pair((short)(-2), i));
    }

    // 已经点开的，尝试点开周围
    else if (find_map[i])
        try_open_around(r, c);

    // 0-扩展
    else if (display_map[i] == 0) {
        find_map[i] = true;
        buffer.push(std::make_pair((short)0, i));
        try_open_around(r, c);
    }

    // 普通格子，点开它
    else { 
        find_map[i] = true; 
        if (mine_map[i])
            buffer.push(std::make_pair((short)9, i));
        else
            buffer.push(std::make_pair((short)(display_map[i]), i));
    }
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


#ifdef DEBUG
void Map::debug()
{
    std::cout << std::endl << "Mine map:" << std::endl;
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
}
#endif
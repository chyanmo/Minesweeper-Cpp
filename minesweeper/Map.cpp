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

#ifdef DEBUG
    debug();
#endif // DEBUG
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
    displayed_map = new int[n];
    find_map = new bool[n];
    // 布雷
    std::for_each(mine_map + m_row * m_col - m_mine, mine_map + m_row * m_col, [](bool& x) {x = 1; });
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
            displayed_map[i * m_col + j] = for_around(i, j,
                [this, count = 0](int r, int c)mutable {count += is_mine(r, c); return (int)count; });
}

void Map::clear()
{
    delete[] mine_map;
    delete[] displayed_map;
    delete[] find_map;
    displayed_map = nullptr;
    mine_map = find_map = nullptr;
}

// 至少有 9 个空位
void Map::resize(int ROW, int COL, int Mine)
{
    m_row = ROW;
    m_col = COL;
    m_mine = (Mine < ROW * COL) ? (Mine) : (ROW * COL);
}

bool Map::is_mine(int r, int c)
{
    return (is_in(r, c)) && (mine_map[r * m_col + c]);
}

bool Map::is_in(int r, int c)
{
    return (r >= 0) && (c >= 0) && (r < m_row) && (c < m_col);
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

    std::cout << "Displayed map:" << std::endl;
    for (int r = 0; r < m_row; r++) {
        for (int c = 0; c < m_col; c++){
            if (mine_map[r * m_col + c])
                std::cout << "x ";
            else
                std::cout << displayed_map[r * m_col + c] << " ";
        }
        std::cout << std::endl;
    }

}
#endif
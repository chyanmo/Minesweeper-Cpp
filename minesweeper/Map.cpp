#include "Map.h"
#include <algorithm>
#include <random>
#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

Map::Map(int ROW, int COL, int Mine)
    : m_row(ROW), m_col(COL)
{
    const int n = ROW * COL;

    mine_map = new bool[n]();
    displayed_map = new int[n];
    find_map = new bool[n];

    if (Mine > n) Mine = n;

    std::for_each(mine_map, mine_map + Mine, [](bool& x) {x = 1; });

    init();

#ifdef DEBUG
    debug();
#endif // DEBUG
}

Map::~Map()
{
    delete[] mine_map;
    delete[] displayed_map;
    delete[] find_map;
    displayed_map = nullptr;
    mine_map = find_map = nullptr;
}

void Map::init()
{
    // 对 mine_map 随机洗牌
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(mine_map, mine_map + m_row * m_col, std::default_random_engine(seed));
    // 生成 displayed_map
    for (int i = 0; i < m_row; i++)
        for (int j = 0; j < m_col; j++) {
            int count = 0;
            for (int k = i - 1; k <= i + 1; k++)
                for (int l = j - 1; l <= j + 1; l++)
                    if (is_mine(k, l))
                        count++;
            displayed_map[i * m_col + j] = count;
        }
}


bool Map::is_mine(int r, int c)
{
    if (r >= 0 && c >= 0 && r < m_row && c < m_col)
        return mine_map[r * m_col + c];
    else
        return 0;
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
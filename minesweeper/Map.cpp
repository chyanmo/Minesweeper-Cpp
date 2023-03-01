#include "Map.h"
#include <utility>
#include <algorithm>
#include <random>
#include <chrono>

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

Map::Map(int ROW, int COL, int Mine)
    :m_row(ROW), m_col(COL)
{
    const int n = ROW * COL;
    m_mark = m_mine = Mine < n ? Mine : n;
    m_remain = n - m_mine;

    // 分配空间
    mine_map = new bool[n]();
    display_map = new int[n]();
    find_map = new bool[n]();
    mark_map = new bool[n]();
    // 布雷
    for (int i = n - m_mine; i < n; i++)
        mine_map[i] = 1;
}

Map::~Map()
{
    delete[] mine_map;
    delete[] display_map;
    delete[] find_map;
    delete[] mark_map;
    display_map = nullptr;
    mark_map = mine_map = find_map = nullptr;
}

void Map::for_around(int r, int c, auto Func)
{
    for (int k = r - 1; k <= r + 1; k++)
        for (int l = c - 1; l <= c + 1; l++)
            Func(k, l);
}

void Map::init(int click_r, int click_c)
{
    // 统计被点击的格子周围有几个格子
    int total = 0;
    if (m_row * m_col >= m_mine + total)
        for_around(click_r, click_c, [this, &total](int r, int c) {if (is_in(r, c)) total++; });

    // 将 mine_map 打乱
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(mine_map + total, mine_map + m_row * m_col, std::default_random_engine(seed));

    // 保证首次点击的周围没有地雷 ( 如果可能的话 )
    int k = 0;
    for (int i = click_r - 1; i <= click_r + 1; i++)
        for (int j = click_c - 1; j <= click_c + 1; j++)
            if (is_in(i, j)) {
                if (i * m_col + j < total) continue;
                while ((k / m_col - click_r) * (k / m_col - click_r) + (k % m_col - click_c) * (k % m_col - click_c) <= 2) k++;
                std::swap(mine_map[i * m_col + j], mine_map[k]);
                k++;
            }

    // 生成 displayed_map
    for (unsigned i = 0; i < m_row; i++)
        for (unsigned j = 0; j < m_col; j++)
            for_around(i, j, [this, i, j](int r, int c) {if (is_mine(r, c)) display_map[i * m_col + j]++; });

#ifdef DEBUG
    debug();
#endif // DEBUG
}

bool Map::win()
{
    if (m_remain == 0) {
        for (unsigned i = 0; i < m_row * m_col; i++)
            if (mine_map[i] && !mark_map[i])
                buffer.push(std::make_pair((short)DOMARK, i));
        m_mark = 0;
        return true;
    }
    return false;
}

void Map::fail()
{
    clear_buffer();

    for (unsigned i = 0; i < m_row * m_col; i++) 
    {
        // 是地雷
        if (mine_map[i]) {
            // 被点击
            if (find_map[i])
                buffer.push(std::make_pair((short)ONAMINE, i));
            // 未被标记
            else if (!mark_map[i])
                buffer.push(std::make_pair((short)OTHERMINE, i));
        }
        // 不是地雷，但被标记
        else if (mark_map[i])
            buffer.push(std::make_pair((short)NOTAMINE, i));
    }
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

void Map::clear_buffer()
{
    while (buffer.size() != 0)
        buffer.pop();
}

unsigned Map::getmark()
{
    return m_mark;
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
        m_mark++;
        buffer.push(std::make_pair((short)UNMARK, i));
    }
    else if (!(mark_map[i]) && m_mark) {
        mark_map[i] = true;
        m_mark--;
        buffer.push(std::make_pair((short)DOMARK, i));
    }
}

void Map::left_click(int r, int c)
{
    unsigned i = r * m_col + c;
    // 被标记的，取消标记
    if (mark_map[i]) {
        mark_map[i] = false;
        m_mark++;
        buffer.push(std::make_pair((short)UNMARK, i));
    }

    // 已经点开的，尝试点开周围
    else if (find_map[i])
        try_open_around(r, c);

    // 0-扩展
    else if (display_map[i] == 0) {
        find_map[i] = true;
        m_remain--;
        buffer.push(std::make_pair((short)BLANK, i));
        try_open_around(r, c);
    }

    // 普通格子，点开它
    else { 
        find_map[i] = true; 
        if (mine_map[i]) {
            clear_buffer();
            buffer.push(std::make_pair((short)TERM, 0));
        }
        else {
            m_remain--;
            buffer.push(std::make_pair((short)(display_map[i]), i));
        }
    }
}

void Map::try_open_around(int r, int c) {
    
    int mine_count = display_map[r * m_col + c];

    // 0-扩展
    if (mine_count == 0) {
        for (int i = r - 1; i <= r + 1; i++)
            for (int j = c - 1; j <= c + 1; j++)
                if (is_in(i, j) && !(find_map[i * m_col + j]) && !(mark_map[i * m_col + j]))
                    left_click(i, j);
    }
    else {
        bool SAFE = true;
        int mark_count = 0;
        for_around(r, c, [this, &mark_count](int r, int c) { if (is_marked(r, c)) mark_count++; });

        // 假设标记数不少于显示的数字，点开其他格子
        if (mark_count >= mine_count)
        {
            for_around(r, c, [this, &SAFE](int r, int c) {
                if (is_in(r, c) && !mark_map[r * m_col + c] && mine_map[r * m_col + c])
                {
                    SAFE = false;
                    find_map[r * m_col + c] = true;
                } });

            if (!SAFE) {
                clear_buffer();
                buffer.push(std::make_pair((short)TERM, 0));
                return;
            }

            for (int i = r - 1; i <= r + 1; i++)
                for (int j = c - 1; j <= c + 1; j++)
                    if (is_in(i, j) && !(find_map[i * m_col + j]) && !(mark_map[i * m_col + j]))
                        left_click(i, j);
        }

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
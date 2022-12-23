#pragma once
#include "configer.h"
class Map
{
public:
    Map(int ROW, int COL, int Mine);
    ~Map();

    void init();

#ifdef DEBUG
    void debug();
#endif

private:
    bool is_mine(int r, int c);

private:
    int m_row;
    int m_col;
    bool* mine_map;
    int* displayed_map;
    bool* find_map;
};

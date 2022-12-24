#pragma once
#include "configer.h"
class Map
{
public:
    Map(int ROW, int COL, int Mine);
    ~Map();

    void resize(int ROW, int COL, int Mine);

    void pre_init();
    void init(int, int);
    void clear();


#ifdef DEBUG
    void debug();
#endif

private:
    bool is_mine(int r, int c);
    bool is_in(int r, int c);

    template<typename Function>
    auto for_around(int r, int c, Function fn);

private:
    int m_row;
    int m_col;
    int m_mine;
    bool* mine_map;
    int* displayed_map;
    bool* find_map;
};
#pragma once
#include "configer.h"

class Map
{
public:
    Map(int ROW, int COL, int Mine);
    ~Map();

    void resize(int ROW, int COL, int Mine);

    void pre_init();        // 分配空间
    void init(int, int);    // 将 mine_map 打乱，生成 display_map
    void clear();           // 清理空间

    unsigned short stat(unsigned i);

#ifdef DEBUG
    void debug();
#endif

    bool is_in(int r, int c);
    bool is_mine(int r, int c);
    bool is_marked(int r, int c);


    template<typename Function>
    auto for_around(int r, int c, Function fn);

    void zero_extend(int r, int c);
    void left_click(int r, int c);
    void right_click(int r, int c);
    void try_open_around(int r, int c);

    int m_row;
    int m_col;
    int m_mine;
    bool* mine_map;
    int* display_map;
    bool* find_map;
    bool* mark_map;
};
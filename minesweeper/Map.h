#pragma once
#include "configer.h"
#include <utility>
#include <queue>

class Map
{
public:
    enum Act{
        TERM = -3,
        UNMARK = -2,
        DOMARK,
        BLANK,
        ONAMINE = 9,
        OTHERMINE,
        NOTAMINE
    };

    Map(int ROW, int COL, int Mine);
    ~Map();

    void left_click(int r, int c);
    void right_click(int r, int c);
    void init(int r, int c);    // 将 mine_map 打乱，并生成 display_map

    bool win();
    void fail();

    bool empty_buffer();
    std::pair<short,unsigned> front_buffer();
    void pop_buffer();
    void clear_buffer();

    unsigned getmark();

private:

    bool is_in(int r, int c);
    bool is_mine(int r, int c);
    bool is_marked(int r, int c);

    void for_around(int r, int c, auto Func);   // 仅在 Map.cpp 内定义和使用

    void try_open_around(int r, int c);

#ifdef DEBUG
    void debug();
#endif

    unsigned m_row;
    unsigned m_col;
    unsigned m_mine;
    unsigned m_mark;
    unsigned m_remain;

    std::queue<std::pair<short, unsigned>> buffer;

    bool* mine_map;
    int* display_map;
    bool* find_map;
    bool* mark_map;
};
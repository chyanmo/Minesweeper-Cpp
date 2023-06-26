#pragma once
#include "configer.h"
#include <utility>  // std::pair
#include <queue>

class Map
{
public:
    enum OpType{
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

    std::pair<intf8_t, int> front_buffer();

    void pop_buffer();
    

    int getmark();

private:

    bool is_in(int r, int c);
    bool is_mine(int r, int c);
    bool is_marked(int r, int c);

    void clear_buffer();
    inline void push_buffer(intf8_t opType, int index_);

    void for_around(int r, int c, auto Func);

    void try_open_around(int r, int c);

#ifdef DEBUG
    void debug();
#endif

    int m_row;
    int m_col;
    int m_mine;
    int m_mark;
    int m_remain;

    std::queue<std::pair<intf8_t, int>> buffer;

    intf8_t* mine_map;
    bool* find_map;
    bool* mark_map;
};

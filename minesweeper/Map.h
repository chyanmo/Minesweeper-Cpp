#pragma once
#include "configer.h"
#include <utility>
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

    Map(uint16_t ROW, uint16_t COL, uint16_t Mine);
    ~Map();

    void left_click(uint16_t r, uint16_t c);
    void right_click(uint16_t r, uint16_t c);
    void init(uint16_t r, uint16_t c);    // 将 mine_map 打乱，并生成 display_map

    bool win();
    void fail();

    bool empty_buffer();

    std::pair<int8_t, uint16_t> front_buffer();

    void pop_buffer();
    

    uint16_t getmark();

private:

    bool is_in(int r, int c);
    bool is_mine(int r, int c);
    bool is_marked(int r, int c);

    void clear_buffer();
    inline void push_buffer(int8_t opType, uint16_t i);

    inline void for_around(int r, int c, auto Func);

    void try_open_around(int r, int c);

#ifdef DEBUG
    void debug();
#endif

    uint16_t m_row;
    uint16_t m_col;
    uint16_t m_mine;
    uint16_t m_mark;
    uint16_t m_remain;

    std::queue<std::pair<int8_t, uint16_t>> buffer;

    int8_t* mine_map;
    bool* find_map;
    bool* mark_map;
};
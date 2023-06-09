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

    Map(u16_t ROW, u16_t COL, u16_t Mine);
    ~Map();

    void left_click(u16_t r, u16_t c);
    void right_click(u16_t r, u16_t c);
    void init(u16_t r, u16_t c);    // 将 mine_map 打乱，并生成 display_map

    bool win();
    void fail();

    bool empty_buffer();

    std::pair<char, u16_t> front_buffer();

    void pop_buffer();
    

    u16_t getmark();

private:

    bool is_in(int r, int c);
    bool is_mine(int r, int c);
    bool is_marked(int r, int c);

    void clear_buffer();
    inline void push_buffer(char opType, u16_t i);

    inline void for_around(int r, int c, auto Func);

    void try_open_around(int r, int c);

#ifdef DEBUG
    void debug();
#endif

    u16_t m_row;
    u16_t m_col;
    u16_t m_mine;
    u16_t m_mark;
    u16_t m_remain;

    std::queue<std::pair<char, u16_t>> buffer;

    char* mine_map;
    bool* find_map;
    bool* mark_map;
};
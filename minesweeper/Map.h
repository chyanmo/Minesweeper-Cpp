#pragma once
#include "configer.h"
#include <utility>
#include <queue>

class Map
{
public:
    Map(int ROW, int COL, int Mine);
    ~Map();

    void left_click(int r, int c);
    void right_click(int r, int c);
    void init(int r, int c);    // 将 mine_map 打乱，生成 display_map

    bool empty_buffer();
    std::pair<short,unsigned> front_buffer();
    void pop_buffer();


private:

    void resize(int ROW, int COL, int Mine);

    void pre_init();        // 分配空间
    void clear();           // 清理空间

    bool is_in(int r, int c);
    bool is_mine(int r, int c);
    bool is_marked(int r, int c);


    template<typename Function>
    auto for_around(int r, int c, Function fn);

    void try_open_around(int r, int c);

#ifdef DEBUG
    void debug();
#endif

    int m_row;
    int m_col;
    int m_mine;

    std::queue<std::pair<short, unsigned>> buffer;

    bool* mine_map;
    int* display_map;
    bool* find_map;
    bool* mark_map;
};
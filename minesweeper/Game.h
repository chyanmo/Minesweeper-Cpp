#pragma once
#include "Window.h"
#include "Button.h"
#include "Map.h"
#include <vector>

class Game
{
public:
    Game(int ROW = 9, int COL = 9, int ButtonSize = 30, int Mine = 8);
    ~Game();
    // 启动
    void run();

    // 哪个按钮被按下
    unsigned short get_click_pos(unsigned int &coor);

    void show_button();

    void draw_bkground();

private:

    void init_button();
    void clear_button();

private:

    int game_row;
    int game_col;
    int game_mine;
    int button_size;

    IMAGE m_bk;
    ExMessage m_msg;

    std::vector<Button*> btns;
};



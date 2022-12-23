#pragma once
#include "Window.h"
#include <vector>
#include "Button.h"
class Game
{
public:
    Game(const int ROW = 9, const int COL = 9, const int ButtonSize = 30, const int Bomb = 8);
    // 启动
    void run();

    // 哪个按钮被按下
    int get_click_pos();

    void show_button();

    void draw_bkground();

private:
    IMAGE m_bk;
    ExMessage m_msg;

    std::vector<Button*> btns;
};



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

private:
    
    // 哪个按钮被按下
    unsigned short get_click_pos(unsigned int &coor);
    void init(int ROW = 9, int COL = 9, int Mine = 8);

    void test();

private:


    void init_button();
    void show_button();
    void clear_button();
    


private:

    bool started = false;
    bool inited = false;

    int game_row;
    int game_col;
    int game_mine;
    int button_size;

    ExMessage m_msg;

    Window* window = nullptr;
    Map* map = nullptr;

    std::vector<Label*> labels;
    std::vector<Button*> menus;
    std::vector<Button*> btns;
};



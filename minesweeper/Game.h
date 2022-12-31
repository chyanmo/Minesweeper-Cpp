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
    
    bool click_forward();

    void init(int ROW = 9, int COL = 9, int Mine = 8);

private:


    void init_button();
    void show_button();
    void update_button();
    void clear_button();
    


private:

    const COLORREF COLOR[9] = {
        WHITE,
        RGB(0,4,251),
        RGB(1,124,8),
        RGB(247,2,7),
        RGB(0,0,123),
        RGB(122,3,0),
        RGB(0,127,128),
        BLACK,
        RGB(126,126,126)
    };

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



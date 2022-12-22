#pragma once
#include "Window.h"
#include <vector>
#include "Button.h"
class Game
{
    enum Operator
    {
        Display,
        Insert,
        Remove,
        Modify,
        Search,
        Menu = 66
    };
public:
    Game();
    
    void run();
    
    int map();

    void drawBackground();

private:
    IMAGE m_bk;
    ExMessage m_msg;

    std::vector<Button*> btns;
};



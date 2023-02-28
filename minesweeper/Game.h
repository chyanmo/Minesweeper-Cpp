#pragma once
#include "Window.h"
#include "Button.h"
#include "Map.h"
#include "TimeBox.h"
#include <vector>

class Game
{
public:
    enum Icon {
        SMILE,
        COOL,
        CRY,
        FLAG,
        MINE,
        MINERED,
        NOTAMINE
    };

    const COLORREF COLOR[9] = {
        RGB(244, 240, 230),
        RGB(0,4,251),
        RGB(1,124,8),
        RGB(247,2,7),
        RGB(0,0,123),
        RGB(122,3,0),
        RGB(0,127,128),
        BLACK,
        RGB(126,126,126)
    };

    Game(unsigned ROW = 9, unsigned COL = 9, unsigned Mine = 10, unsigned ButtonSize = 25);
    ~Game();

    void run();

private:

    void load_icons();
    void init(unsigned ROW, unsigned COL, unsigned Mine);

    void init_button();
    void show_button();
    void update_button();
    void clear_button();

    bool lclick_forward();
    bool rclick_forward();
    
private:

    bool started = false;
    bool inited = false;
    bool finished = false;

    unsigned game_row;
    unsigned game_col;
    unsigned game_mine;
    unsigned button_size;

    IMAGE icons[7];

    ExMessage m_msg;

    Window* window = nullptr;
    Map* map = nullptr;

    std::vector<Label*> labels;
    Button* emojiButton = nullptr;
    TimeBox* timer = nullptr;
    std::vector<Button*> btns;
};



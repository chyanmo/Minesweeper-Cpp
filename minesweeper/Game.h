#pragma once
#include "configer.h"
#include "Window.h"
#include "Button.h"
#include "Map.h"
#include "TimeBox.h"
#include <vector>
#include <shared_mutex>
#include <thread>


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
        MINEREDX
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

    Game(uint16_t ROW = 9, uint16_t COL = 9, uint16_t Mine = 10, uint16_t ButtonSize = 25);
    ~Game();

    void run();

private:

    void load_icons();
    void init(uint16_t ROW, uint16_t COL, uint16_t Mine);

    void init_button();
    void show_button();
    void update_button();
    void clear_button();

    bool lclick_forward();
    bool rclick_forward();

    void event_handle();
    void show();
    
private:

    bool started = false;
    bool inited = false;
    bool finished = false;

    uint16_t game_row;
    uint16_t game_col;
    uint16_t game_mine;
    uint16_t button_size;

    uint16_t FPS = 30;

    IMAGE icons[7];

    ExMessage m_msg;

    Window* window = nullptr;
    Map* map = nullptr;

    std::vector<Label*> labels;
    Button* emojiButton = nullptr;
    TimeBox* timer = nullptr;
    std::vector<Button*> btns;

    bool quit_ = false;
    std::shared_mutex mutex_;
};



#pragma once
#include "configer.h"
#include "Window.h"
#include "Button.h"
#include "Map.h"
#include "TimeBox.h"
#include <vector>
#include <shared_mutex>

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

    Game(int ROW = 9, int COL = 9, int Mine = 10, int ButtonSize = 25);
    ~Game();

    void run();

private:

    void load_icons();
    void init(int ROW, int COL, int Mine);

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

    int game_row;
    int game_col;
    int game_mine;
    int button_size;

    int FPS = 30;

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



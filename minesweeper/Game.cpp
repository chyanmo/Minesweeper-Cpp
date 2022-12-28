#include "Game.h"
#include <iostream>
#include <conio.h>

Game::Game(int ROW, int COL, int ButtonSize, int Mine)
    : game_row(ROW), game_col(COL), button_size(ButtonSize), game_mine(Mine) {}

Game::~Game()
{
    clear_button();
}

void Game::run()
{
    init(game_row, game_col, game_mine);

    // init 中已经包含 Window::beginDraw();

    while (true)
    {
        Window::clear();

        show_button();

        // 用于记录鼠标 左击/右击
        unsigned short bt = 0;
        unsigned coor = -1;

        // 获取消息
        if (Window::hasMsg()) {
            m_msg = Window::getMsg();
            switch (m_msg.message)
            {
            case WM_KEYDOWN:    // 键盘操作
                if (m_msg.vkcode == 0x52)       // R
                    init(game_row, game_col, game_mine);
                else if (m_msg.vkcode == 0x51)  // Q
                    init(9, 9, 10);
                else if (m_msg.vkcode == 0x57)  // W
                    init(16, 16, 40);
                else if (m_msg.vkcode == 0x45)  // E
                    init(16, 30, 99);
                break;
            default:            // 鼠标操作
                bt = get_click_pos(coor);

                if (bt != 0)
                {
                    unsigned r = coor / game_col;
                    unsigned c = coor % game_col;

                    if (bt == 1) {
                        std::cout << "L";
                        if (!started) {
                            map->init(r, c);
                            started = true;
                        }
                        map->left_click(r, c);
                    }
                    else if (bt == 2) {
                        std::cout << "R";
                        map->right_click(r, c);
                    }

                    test();

                }
                break;
            }
        }

        
        Window::flushDraw();
    }
    Window::endDraw();
}


unsigned short Game::get_click_pos(unsigned int& coor)
{
    unsigned int key;
    for (int i = 0; i < btns.size(); i++) {

        btns[i]->eventLoop(m_msg);  // 传递消息
        
        key = btns[i]->is_clicked();
        if (key) {
            coor = i;
            return key;
        }
    }
    coor = -1;
    return 0;
}

void Game::init(int ROW, int COL, int Mine)
{
    started = false;

    if (ROW == game_row && COL == game_col && Mine == game_mine && inited)
        ;
    else {
        game_row = ROW;
        game_col = COL;
        game_mine = Mine;

        if (window) {
            Window::endDraw;
            delete window;
            window = nullptr;
        }

        window = new Window(button_size * (COL + 7) + 60 , button_size * (ROW + 2) + 100, EX_SHOWCONSOLE);

        window->setWindowTitle("Minesweeper");

        Window::beginDraw();
        Window::clear();

        ::setbkcolor(RGB(157, 211, 168));
    }
    
    clear_button();
    init_button();

    if (map) {
        delete map;
        map = nullptr;
    }

    map = new Map(game_row, game_col, game_mine);

    inited = true;
}

void Game::test()
{
    for (int i = 0; i < btns.size(); i++) {

        unsigned short st = map->stat(i);

        if (st == 0)
            btns[i]->setBkClr(RGB(244, 240, 230));
        else if (st == 9)
            btns[i]->setText("X");
        else if (st == 10)
            btns[i]->setText("M");
        else if (st == 11)
            btns[i]->setText();
        else {
            btns[i]->setBkClr(RGB(244, 240, 230));
            btns[i]->setText(std::to_string(st));
        }
    }
}

void Game::show_button()
{
    for (auto btn : btns)
        btn->show();
}

void Game::init_button()
{
    int xspace = (Window::width() - button_size * max(game_col, 7)) / 2;
    int yspace = (Window::height() - button_size * (game_row + 2)) / 2;
    // labels.push_back(new Label(xspace, yspace, 2 * button_size, button_size, BLACK, std::to_string(game_mine), RED));
    // labels.push_back(new Label(xspace, yspace, 2 * button_size, button_size, BLACK, std::to_string(game_mine), RED));

    btns.resize(game_row * game_col);
    for (int i = 0; auto & btn : btns) {
        int bx = xspace + button_size * (i % game_col);
        int by = yspace + button_size * (i / game_col);
        btn = new Button(bx, by, button_size, button_size);
        i++;
    }
}

void Game::clear_button()
{
    for (auto btn : btns)
        delete btn;
    btns.clear();
}

#include "Game.h"
#include <iostream>
#include <conio.h>

Game::Game(int ROW, int COL, int ButtonSize, int Mine)
    : game_row(ROW), game_col(COL), button_size(ButtonSize), game_mine(Mine)
{
    ::loadimage(&m_bk, "./images/bk.png", Window::width(), Window::height());
    init_button();
}

Game::~Game()
{
    clear_button();
}

void Game::run()
{
    map = new Map(game_row, game_col, game_mine);

    
    Window::beginDraw();

    while (true)
    {
        Window::clear();

        draw_bkground();
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
                if (m_msg.vkcode == VK_ESCAPE)
                    exit(0);
                break;
            default:            // 鼠标操作
                bt = get_click_pos(coor);
                break;
            }
        }

        
        if (bt != 0) 
        {
            unsigned r = coor / game_col;
            unsigned c = coor % game_col;
            
            if (bt == 1) {
                std::cout << "L";
                if (!start) {
                    map->init(r, c);
                    start = true;
                }
                map->left_click(r, c);
            }
            else if (bt == 2) {
                std::cout << "R";
                map->right_click(r, c);
            }


            for (int i = 0; i < btns.size(); i++) {

                unsigned short st = map->stat(i);

                if (st == 0)
                    btns[i]->setBkClr(RGB(232, 232, 236));
                else if (st == 9)
                    btns[i]->setText("X");
                else if (st == 10)
                    btns[i]->setText("M");
                else if (st == 11)
                    btns[i]->setText();
                else {
                    btns[i]->setBkClr(RGB(232, 232, 236));
                    btns[i]->setText(std::to_string(st));
                }
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


void Game::show_button()
{
    for (auto btn : btns)
        btn->show();
}

void Game::draw_bkground()
{
    ::putimage(0, 0, &m_bk);
}

void Game::init_button()
{
    btns.resize(game_row * game_col);

    int xspace = (Window::width() - button_size * game_col) / 2;
    int yspace = (Window::height() - button_size * game_row) / 2;

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

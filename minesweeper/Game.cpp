#include "Game.h"
#include <iostream>
#include <conio.h>

Game::Game(int ROW, int COL, int ButtonSize, int Mine)
    : game_row(ROW), game_col(COL), button_size(ButtonSize), game_mine(Mine)
{
    ::loadimage(&m_bk, "./images/bk.png", Window::width(), Window::height());
    init_button();

#ifdef DEBUG
    Map m(ROW, COL, Mine);
#endif // DEBUG
}

Game::~Game()
{
    clear_button();
}

void Game::run()
{
    int op = -1;
    Window::beginDraw();

    while (true)
    {
        Window::clear();

        draw_bkground();
        show_button();

        // 获取消息
        if (Window::hasMsg()) {
            m_msg = Window::getMsg();
            switch (m_msg.message)
            {
            case WM_KEYDOWN:    // 键盘操作
                if (m_msg.vkcode == VK_ESCAPE)
                    exit(0);
                break;
            default:    // 鼠标操作
                op = get_click_pos();
                break;
            }
        }

        // 根据返回的坐标进行操作
        if (op >= 0)
            btns[op]->setText("a");


        Window::flushDraw();
    }
    Window::endDraw();
}

int Game::get_click_pos()
{
    // 根据按钮的点击，返回坐标
    for (int i = 0; auto btn : btns) {
        // 获取消息
        btn->eventLoop(m_msg);
        
        if (btn->isClicked())
            return i;
        i++;
    }
    return -1;
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

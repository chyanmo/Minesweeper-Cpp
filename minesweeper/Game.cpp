#include "Game.h"
#include <iostream>
#include <conio.h>

Game::Game(const int ROW, const int COL, const int ButtonSize, const int Bomb)
{
    ::loadimage(&m_bk, "./images/bk.png", Window::width(), Window::height());

    // 按钮初始化
    btns.resize(ROW * COL);

    int xspace = (Window::width() - ButtonSize * COL) / 2;
    int yspace = (Window::height() - ButtonSize * ROW) / 2;

    for (int i = 0; auto& btn : btns) {
        int bx = xspace + ButtonSize * (i % COL);
        int by = yspace + ButtonSize * (i / COL);
        btn = new Button(bx, by, ButtonSize, ButtonSize);
        i++;
    }
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

#include "Game.h"
#include <iostream>
#include <conio.h>

Game::Game()
{
    ::loadimage(&m_bk, "./images/bk.png", Window::width(), Window::height());

    // 按钮初始化
    int ROW = 9, COL = 9;
    for (int i = 0; i < ROW; i++)
        for(int j = 0;j<COL;j++)
            btns.push_back(new Button());

    for (int i = 0; auto btn : btns) {
        btn->resize(30,30);
        int xspace = (Window::width() - btn->width()*ROW) / 2;
        int yspace = (Window::height() - btn->height()*COL) / 2;
        int bx = xspace + btn->width() * (i % ROW);
        int by = yspace + btn->height() * (i / ROW);
        btn->move(bx, by);
        i++;
    }
}

void Game::run()
{
    // 获取按钮的返回值
    int op = -1;
    Window::beginDraw();

    while (true)
    {
        Window::clear();
        drawBackground();

        // 获取键盘消息
        if (Window::hasMsg()) {
            m_msg = Window::getMsg();
            switch (m_msg.message)
            {
            case WM_KEYDOWN:
                // 按 ESC 退出
                if (m_msg.vkcode == VK_ESCAPE)
                    exit(0);
                break;
            default:    // 鼠标操作
                break;
            }
        }

        // 根据按钮点击进行操作
        op = map();
        if (op >= 0)
            btns[op]->setText("a");
        


        Window::flushDraw();
    }
    Window::endDraw();
}

int Game::map()
{
    // 根据按钮的点击，返回坐标
    int pos = -1;

    for (int i = 0; auto btn : btns) {
        btn->show();
        btn->eventLoop(m_msg);
        
        if (btn->isClicked())
            pos = i;
        i++;
    }
    return pos;
}

void Game::drawBackground()
{
    ::putimage(0, 0, &m_bk);
}

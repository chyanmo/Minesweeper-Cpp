#pragma once

class BasicWidget
{
public:
    BasicWidget(int x, int y, int w, int h);

    // 返回位置和大小
    int x();
    int y();
    int width();
    int height();

    // 移动和调整大小
    void resize(int w, int h);
    void move(int x, int y);

    // 显示
    virtual void show() = 0;

protected:
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};


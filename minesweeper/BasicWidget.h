#pragma once
class BasicWidget
{
public:
    BasicWidget(int x, int y, int w, int h);

    int x();
    int y();
    int width();
    int height();


    void resize(int w, int h);
    void move(int x, int y);

    virtual void show() = 0;

protected:
    int m_x;
    int m_y;
    int m_w;
    int m_h;
};


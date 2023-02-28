#include "Button.h"

Button::Button(int x, int y, int w, int h)
    :Label(x,y,w,h,RGB(217, 217, 243)) {}

void Button::show()
{
    if (showImg) {
        ::setfillcolor(bk_clr);
        ::fillroundrect(m_x, m_y, m_x + m_w, m_y + m_h, 5, 5);
        ::putimage(m_x + m_w / 8 + 1, m_y + m_h / 8 + 1, img);
    }
    else
        Label::show();
}

bool Button::isin(const ExMessage& msg)
{
    if (msg.x >= m_x && msg.x < m_x + m_w &&
        msg.y >= m_y && msg.y < m_y + m_h)
        return true;
    else
        return false;
}

void Button::setImage(IMAGE* ImgFile)
{
    img = ImgFile;
    showImg = true;
}

void Button::setImage(bool showImg)
{
    this->showImg = showImg;
}
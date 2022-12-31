#include "Button.h"

Button::Button(int x, int y, int w, int h)
    :Label(x,y,w,h,RGB(217, 217, 243)) {}

bool Button::isin()
{
    if (m_msg.x > m_x && m_msg.x < m_x + m_w &&
        m_msg.y > m_y && m_msg.y < m_y + m_h)
        return true;
    return false;
}

unsigned short Button::is_clicked()
{
    if (isin()) {
        switch (m_msg.message)
        {
        case WM_RBUTTONDOWN:
            return 2;
        case WM_LBUTTONDOWN:
            return 1;
        }
    }
    return 0;
}

void Button::getmsg(const ExMessage& msg)
{
    m_msg = msg;
    if (!isin())
        bk_clr = normal_clr;
    else bk_clr = hover_clr;
}

void Button::getmsg2(const ExMessage& msg)
{
    m_msg = msg;
}

void Button::setBkClr(COLORREF c)
{
    bk_clr = normal_clr = c;
}

void Button::setHoverClr(COLORREF c)
{
    hover_clr = c;
}
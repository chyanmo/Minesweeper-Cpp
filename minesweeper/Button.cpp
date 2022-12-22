#include "Button.h"

Button::Button(int x, int y, int w, int h)
    :BasicWidget(x,y,w,h) {}

void Button::show()
{
    setfillcolor(current_clr);
    ::fillroundrect(m_x, m_y, m_x + m_w, m_y + m_h, 10, 10);

    settextcolor(BLACK);
    // ÎÄ×Ö¾ÓÖÐÏÔÊ¾
    int tx = m_x + (m_w - textwidth(m_text.c_str())) / 2,
        ty = m_y + (m_h - textheight(m_text.c_str())) / 2;
    ::outtextxy(tx, ty, m_text.c_str());
}

bool Button::isin()
{
    if (m_msg.x > m_x && m_msg.x < m_x + m_w &&
        m_msg.y > m_y && m_msg.y < m_y + m_h)
        return true;
    return false;
}

bool Button::isClicked()
{
    if (isin())
        if (m_msg.message == WM_LBUTTONUP)
            return true;
    return false;
}

void Button::eventLoop(const ExMessage& msg)
{
    m_msg = msg;
    if (!isin())
        current_clr = normal_clr;
    else current_clr = hover_clr;
}

void Button::setBkClr(COLORREF c)
{
    normal_clr = c;
}

void Button::setHoverClr(COLORREF c)
{
    hover_clr = c;
}

void Button::setText(const std::string& text)
{
    m_text = text;
}

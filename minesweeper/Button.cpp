#include "Button.h"

Button::Button(int x, int y, int w, int h)
    :BasicWidget(x,y,w,h) {}

void Button::show()
{
    setfillcolor(current_clr);
    ::fillroundrect(m_x, m_y, m_x + m_w, m_y + m_h, 10, 10);

    settextcolor(text_clr);
    // 文字居中
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

void Button::setTextClr(COLORREF c)
{
    text_clr = c;
}

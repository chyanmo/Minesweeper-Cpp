#include "Label.h"

Label::Label(int x, int y, int w, int h, const std::string& text, COLORREF textcolor, COLORREF backgroundcolor)
    :BasicWidget(x, y, w, h), m_text(text), text_clr(textcolor), bk_clr(backgroundcolor) {}

Label::Label(int x, int y, int w, int h, COLORREF backgroundcolor, const std::string& text, COLORREF textcolor)
    :BasicWidget(x, y, w, h), m_text(text), text_clr(textcolor), bk_clr(backgroundcolor) {}

void Label::show()
{
    setfillcolor(bk_clr);
    ::fillroundrect(m_x, m_y, m_x + m_w, m_y + m_h, 5, 5);

    settextcolor(text_clr);
    // 文字居中
    int tx = m_x + (m_w - textwidth(m_text.c_str())) / 2,
        ty = m_y + (m_h - textheight(m_text.c_str())) / 2;
    ::outtextxy(tx, ty, m_text.c_str());
}

void Label::setText(const std::string& text)
{
    m_text = text;
}

void Label::setTextClr(COLORREF c)
{
    text_clr = c;
}

void Label::setBkClr(COLORREF c)
{
    bk_clr = c;
}
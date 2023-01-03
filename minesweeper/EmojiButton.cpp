#include "EmojiButton.h"

EmojiButton::EmojiButton(int x, int y, int w, int h, LPCTSTR Norm_ImgFile, LPCTSTR Win_ImgFile, LPCTSTR Fail_ImgFile)
    :Button(x, y, w, h)
{
    ::loadimage(&emoji[NORM], Norm_ImgFile, m_w * 3 / 4, m_h * 3 / 4);
    ::loadimage(&emoji[WIN], Win_ImgFile, m_w * 3 / 4, m_h * 3 / 4);
    ::loadimage(&emoji[FAIL], Fail_ImgFile, m_w * 3 / 4, m_h * 3 / 4);
    current_emoji = &emoji[NORM];
}

void EmojiButton::show()
{
    ::setfillcolor(bk_clr);
    ::fillroundrect(m_x, m_y, m_x + m_w, m_y + m_h, 5, 5);
    ::putimage(m_x + m_w / 8 + 1, m_y + m_h / 8 + 1, current_emoji);
}

void EmojiButton::setstat(stat s)
{
    current_emoji = &emoji[s];
}


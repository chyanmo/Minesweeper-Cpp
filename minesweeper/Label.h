#pragma once
#include "configer.h"
#include "BasicWidget.h"

class Label :
    public BasicWidget
{
public:
    Label(int x, int y, int w, int h,
        const std::string& text = "",
        COLORREF textcolor = BLACK, COLORREF backgroundcolor = WHITE);

    Label(int x, int y, int w, int h, COLORREF backgroundcolor,
        const std::string& text = "",
        COLORREF textcolor = BLACK);

    void show() override;

    void setText(const std::string & text = "");        // 设置文本
    void setTextClr(COLORREF);                          // 设置文本颜色
    void setBkClr(COLORREF);                            // 设置背景颜色

protected:
    std::string m_text;

    COLORREF text_clr;
    COLORREF bk_clr;
};
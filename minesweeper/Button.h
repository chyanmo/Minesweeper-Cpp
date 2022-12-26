#pragma once
#include "configer.h"
#include "BasicWidget.h"

class Button :
    public BasicWidget
{
public:
    Button(int x = 0, int y = 0, int w = 10, int h = 10);
    void show() override;

    void setText(const std::string & = std::string());  // 设置文本
    void setTextClr(COLORREF);                          // 设置文本颜色
    void setBkClr(COLORREF);                            // 设置背景颜色
    void setHoverClr(COLORREF);                         // 设置被鼠标扫过时的颜色
    
    bool isin();                    // 判断鼠标在按钮上
    unsigned short is_clicked();    // 判断点击，左击返回 1，右击返回 2，没点击返回 0

    void eventLoop(const ExMessage&);


private:
    std::string m_text = std::string();
    ExMessage m_msg;
private:
    COLORREF text_clr = BLACK;
    // COLORREF current_clr = RGB(232, 232, 236);
    COLORREF current_clr = RGB(100, 100, 200);
    COLORREF normal_clr = RGB(100, 100, 200);
    COLORREF hover_clr = RED;
};


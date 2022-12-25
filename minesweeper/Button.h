#pragma once
#include "configer.h"
#include "BasicWidget.h"

class Button :
    public BasicWidget
{
public:
    Button(int x = 0, int y = 0, int w = 10, int h = 10);
    void show() override;


    void setBackgroundColor(COLORREF);
    void setHoverColor(COLORREF);
    void setText(const std::string& = std::string());


    bool isin();                // 鼠标在按钮上
    unsigned short is_clicked();          // 左击返回 1，右击返回 2，没点击返回 0

    void eventLoop(const ExMessage&);


private:
    std::string m_text = std::string();
    ExMessage m_msg;
private:
    COLORREF current_clr = RGB(232, 232, 236);
    COLORREF normal_clr = RGB(232, 232, 236);
    COLORREF hover_clr = RED;
};


#pragma once
#include "configer.h"
#include "BasicWidget.h"

class Button :
    public BasicWidget
{
public:
    Button(int x = 0, int y = 0, int w = 10, int h = 10);
    void show() override;


    void setBkClr(COLORREF);
    void setHoverClr(COLORREF);
    void setText(const std::string& = std::string());


    bool isin();    // 判断鼠标是否在按钮上
    bool isClicked();   // 判断鼠标是否点击了按钮

    void eventLoop(const ExMessage&);


private:
    std::string m_text = std::string();
    ExMessage m_msg;
private:
    COLORREF current_clr = RGB(232, 232, 236);
    COLORREF normal_clr = RGB(232, 232, 236);
    COLORREF hover_clr = RED;
};


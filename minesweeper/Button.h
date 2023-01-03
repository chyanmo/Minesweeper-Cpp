#pragma once
#include "configer.h"
#include "Label.h"

class Button :
    public Label
{
public:
    Button(int x = 0, int y = 0, int w = 10, int h = 10);

    void setHoverClr(COLORREF);         // 设置被鼠标扫过时的颜色
    void setBkClr(COLORREF);            // 设置没有被鼠标扫过时的颜色

    void getmsgx(const ExMessage&);   // 用于接收消息
    void getmsg(const ExMessage&);

    bool isin();                        // 判断鼠标在按钮上
    unsigned short is_clicked();        // 判断点击，左击返回 1，右击返回 2，没点击返回 0

    bool active = true;
    
private:
    ExMessage m_msg;

    COLORREF normal_clr = RGB(217, 217, 243);
    COLORREF hover_clr = RGB(206, 239, 228);
};
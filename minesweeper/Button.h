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

    void eventLoop(const ExMessage&);   // 用于接收消息

    bool isin();                        // 判断鼠标在按钮上
    unsigned short is_clicked();        // 判断点击，左击返回 1，右击返回 2，没点击返回 0

    
    /*
     * 在父类中实现：
     * 
     * void show();                                         // 显示
     * void setText(const std::string & = std::string());   // 设置文本
     * void setTextClr(COLORREF);                           // 设置文本颜色
     */


private:
    std::string m_text = std::string();
    ExMessage m_msg;

    COLORREF normal_clr = RGB(217, 217, 243);
    COLORREF hover_clr = RGB(206, 239, 228);
};
#pragma once
#include "configer.h"
class Window
{
public:
    Window(int w, int h, int flag = 0);
    ~Window();
    void setWindowTitle(const std::string& title);

    static int width();
    static int height();

    // 绘图
    static void clear();
    static void beginDraw();
    static void flushDraw();
    static void endDraw();

private:

    HWND m_handle;		// 窗口句柄
};

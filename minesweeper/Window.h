#pragma once
#include "configer.h"
class Window
{
public:
    Window(int w, int h, int flag = 0);
    ~Window();
    void setWindowTitle(const std::string& title);
    int exec();

    static int width();
    static int height();

    // 绘图
    static void clear();
    static void beginDraw();
    static void flushDraw();
    static void endDraw();

    // 按键操作和鼠标操作
    // 判断有没有消息
    inline static bool hasMsg() {
        return ::peekmessage(&m_msg, EX_MOUSE | EX_KEY);
    }

    inline static const ExMessage& getMsg() {
        return m_msg;
    }

private:

    HWND m_handle;		// 窗口句柄
    inline static ExMessage m_msg;	// 消息
};

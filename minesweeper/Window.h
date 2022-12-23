#pragma once
#include "configer.h"
class Window
{
public:
    Window(int w, int h, int flag = 0);
    void setWindowTitle(const std::string& title);
    int exec();

    static int width();
    static int height();
    static void clear();
    static void beginDraw();
    static void flushDraw();
    static void endDraw();

    // 按键操作和鼠标操作
    // 判断有没有消息
    inline static bool hasMsg() {
        return ::peekmessage(&m_msg, EX_MOUSE | EX_KEY);
    }	// 没有 this 指针，需要 m_msg 也是 static
    inline static const ExMessage& getMsg() {
        return m_msg;
    }

private:
    HWND m_handle;		// 窗口句柄
    // 可定义如下 (Since C++17)
    inline static ExMessage m_msg;	// 消息
};

#pragma once
#include "configer.h"
class Window
{
public:
    Window(int w, int h, int flag);
    void setWindowTitle(const std::string& title);
    int exec();

    static int width();
    static int height();
    static void clear();
    static void beginDraw();
    static void flushDraw();
    static void endDraw();

    // ����������������
    // �ж���û����Ϣ
    inline static bool hasMsg() {
        return ::peekmessage(&m_msg, EX_MOUSE | EX_KEY);
    }	// û�� this ָ�룬��Ҫ m_msg Ҳ�� static
    inline static const ExMessage& getMsg() {
        return m_msg;
    }

private:
    HWND m_handle;		// ���ھ��
    // C++ 17
    inline static ExMessage m_msg;	// ��Ϣ
};

// C++ 17 ֮ǰ��Ҫ�� cpp �ļ�����������һ�²��� ExMessage Window::m_msg;

#pragma once
#include "Button.h"

class EmojiButton :
    public Button
{
public:
    enum stat {
        NORM,
        WIN,
        FAIL
    };

    EmojiButton(int x, int y, int w, int h, LPCTSTR Norm_ImgFile, LPCTSTR Win_ImgFile, LPCTSTR Fail_ImgFile);

    void show();

    void setstat(stat);

private:
    IMAGE emoji[3];
    IMAGE* current_emoji = nullptr;
};
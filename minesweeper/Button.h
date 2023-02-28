#pragma once
#include "configer.h"
#include "Label.h"

class Button :
    public Label
{
public:
    Button(int x = 0, int y = 0, int w = 10, int h = 10);

    bool isin(const ExMessage& msg);                        // 判断鼠标在按钮上
    
    void setImage(IMAGE* ImgFile);
    void setImage(bool showImg);

    void show();

protected:

    bool showImg;
    IMAGE* img = nullptr;
};
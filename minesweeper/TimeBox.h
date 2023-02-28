#pragma once
#include "Label.h"
#include "StopWatch.h"

class TimeBox
    :public Label, public StopWatch
{
public:
    TimeBox(int x, int y, int w, int h, COLORREF backgroundcolor = WHITE, COLORREF textcolor = RED);

    void show();
};
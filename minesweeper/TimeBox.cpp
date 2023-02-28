#include "TimeBox.h"

TimeBox::TimeBox(int x, int y, int w, int h, COLORREF backgroundcolor, COLORREF textcolor)
    :Label(x, y, w, h, backgroundcolor, "", textcolor), StopWatch() {}


void TimeBox::show()
{
    setText(std::to_string(gettime() / 1000));
    Label::show();
}
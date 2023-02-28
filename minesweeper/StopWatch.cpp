#include "StopWatch.h"

StopWatch::StopWatch()
{
    reset();
}

void StopWatch::start()
{
    started = true;
    begin = std::chrono::steady_clock::now();
}

long long StopWatch::gettime()
{
    if (started)
        end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

long long StopWatch::stop()
{
    end = std::chrono::steady_clock::now();
    started = false;
    return gettime();
}

void StopWatch::reset()
{
    end = begin = std::chrono::steady_clock::now();
    started = false;
}
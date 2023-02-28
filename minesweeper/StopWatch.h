#pragma once
#include <chrono>

class StopWatch
{
public:
    StopWatch();
    void start();
    long long gettime();
    long long stop();
    void reset();

private:

    bool started;
    std::chrono::steady_clock::time_point begin, end;
};
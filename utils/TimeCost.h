#ifndef TIME_COST_H
#define TIME_COST_H

#include <cstdio>

#if __cplusplus >= 201103l
#include <chrono>
#elif _POSX_C_SOURCE >= 199903l

#include <time.h>
#else
#include <ctime>
#endif


class TimeCost {
public:
explicit TimeCost();
~TimeCost();

private:
/*
struct timespec {
               time_t   tv_sec;        
               long     tv_nsec;       
            };

*/
#if __cplusplus >= 201103l

using Clock = std::chrono::steady_clock;
using Second = std::chrono::duration<double, std::ratio<1> >;

std::chrono::time_point<Clock> bTime{ Clock::now()};
#elif _POSIX_C_SOURCE >= 199903l
struct timespec ts;
#else
clock_t bTime;
#endif

/*https://www.learncpp.com/cpp-tutorial/timing-your-code/*/
};

#endif
#include "stdafx.h"
#include "Timer.h"

Timer::Timer(Context * const context)
    : ISubsystem(context)
{
}

bool Timer::Initialize()
{
    previous_time = std::chrono::high_resolution_clock::now();
    return true;
}

void Timer::Update(const float & delta_time_sec)
{
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = current_time - previous_time;
    previous_time = current_time;

    delta_time_ms = ms.count();
}

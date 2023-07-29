module;

#include "common.h"

export module eqapp_timer;

export
{

namespace eqapp
{

class Timer
{

public:

    Timer();
    ~Timer();

private:

    //

public:

    using TimePoint = std::chrono::steady_clock::time_point;
    using TimeInterval = long long;

    TimeInterval GetTimeElapsedInSeconds();
    TimeInterval GetTimeElapsedInMilliseconds();

    bool HasTimeElapsedInSeconds(TimeInterval timeInterval);
    bool HasTimeElapsedInMilliseconds(TimeInterval timeInterval);

    void Restart();

private:

    TimePoint m_time = std::chrono::steady_clock::now();

};

Timer::Timer()
{
    //
}

Timer::~Timer()
{
    //
}

Timer::TimeInterval Timer::GetTimeElapsedInSeconds()
{
    TimePoint currentTime = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_time).count();
}

Timer::TimeInterval Timer::GetTimeElapsedInMilliseconds()
{
    TimePoint currentTime = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_time).count();
}

bool Timer::HasTimeElapsedInSeconds(TimeInterval timeInterval)
{
    TimeInterval timeElapsedInSeconds = GetTimeElapsedInSeconds();
    if (timeElapsedInSeconds >= timeInterval)
    {
        return true;
    }

    return false;
}

bool Timer::HasTimeElapsedInMilliseconds(TimeInterval timeInterval)
{
    TimeInterval timeElapsedInMilliseconds = GetTimeElapsedInMilliseconds();
    if (timeElapsedInMilliseconds >= timeInterval)
    {
        return true;
    }

    return false;
}

void Timer::Restart()
{
    m_time = std::chrono::steady_clock::now();
}

}

}


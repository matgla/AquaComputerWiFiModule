#pragma once

#include "timer/ITimer.hpp"

#include "utils/types.hpp"

namespace timer
{

class IManager
{
public:
    virtual ITimer::TimerPtr setTimeout(u32 milliseconds, ITimer::TimerCallback callback) = 0;
    virtual ITimer::TimerPtr setInterval(u32 milliseconds, ITimer::TimerCallback callback) = 0;
    virtual ITimer::TimerPtr setInterval(u32 milliseconds, ITimer::TimerCallback callback,
                                         int times) = 0;

    virtual void run() = 0;
};

} // namespace timer

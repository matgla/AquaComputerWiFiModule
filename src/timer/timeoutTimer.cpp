#include "timer/timeoutTimer.hpp"

#include "hal/time/time.hpp"
#include "utils/types.hpp"

namespace timer
{

TimeoutTimer::TimeoutTimer(const u64 time, TimerCallback callback)
    : callback_(std::move(callback)), startTime_(hal::time::milliseconds()), time_(time),
      enabled_(true)
{
}

void TimeoutTimer::run()
{
    if (hal::time::milliseconds() - startTime_ >= time_)
    {
        fire();
    }
}

void TimeoutTimer::cancel()
{
    enabled_ = false;
}

bool TimeoutTimer::enabled() const
{
    return enabled_;
}


void TimeoutTimer::fire()
{
    if (enabled_)
    {
        enabled_ = false;
        callback_();
    }
}

} // namespace timer

#include "timer/intervalTimer.hpp"

#include "hal/time/time.hpp"

namespace timer
{

IntervalTimer::IntervalTimer(u64 time, TimerCallback callback, int times)
    : callback_(callback), startTime_(hal::time::milliseconds()), time_(time), enabled_(true),
      times_(times)
{
}

void IntervalTimer::run()
{
    if (hal::time::milliseconds() - startTime_ >= time_)
    {
        fire();
    }
}

void IntervalTimer::cancel()
{
    enabled_ = false;
}

bool IntervalTimer::enabled() const
{
    return enabled_;
}


void IntervalTimer::fire()
{
    if (enabled_)
    {
        startTime_ = hal::time::milliseconds();

        if (-1 != times_)
        {
            --times_;
        }

        if (times_ == 0)
        {
            enabled_ = false;
        }

        callback_();
    }
}

} // namespace timer

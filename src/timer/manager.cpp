#include "timer/manager.hpp"

#include <algorithm>

#include "timer/intervalTimer.hpp"
#include "timer/timeoutTimer.hpp"

namespace timer
{

ITimer::TimerPtr Manager::setTimeout(u32 milliseconds, ITimer::TimerCallback callback)
{
    timers_.emplace_back(new TimeoutTimer(milliseconds, callback));
    return timers_.back();
}

ITimer::TimerPtr Manager::setInterval(u32 milliseconds, ITimer::TimerCallback callback)
{
    timers_.emplace_back(new IntervalTimer(milliseconds, callback));
    return timers_.back();
}

ITimer::TimerPtr Manager::setInterval(u32 milliseconds, ITimer::TimerCallback callback, int times)
{
    timers_.emplace_back(new IntervalTimer(milliseconds, callback, times));
    return timers_.back();
}

void Manager::run()
{
    for (auto& timer : timers_)
    {
        timer->run();
    }

    timers_.erase(std::remove_if(timers_.begin(), timers_.end(),
                                 [](const ITimer::TimerPtr& timer) { return !timer->enabled(); }),
                  timers_.end());
}

} // namespace timer

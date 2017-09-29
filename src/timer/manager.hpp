#pragma once

#include "timer/IManager.hpp"

#include <utility>
#include <vector>

namespace timer
{

class Manager : public IManager
{
public:
    ITimer::TimerPtr setTimeout(u32 milliseconds, ITimer::TimerCallback callback) override;
    ITimer::TimerPtr setInterval(u32 milliseconds, ITimer::TimerCallback callback) override;
    ITimer::TimerPtr setInterval(u32 milliseconds, ITimer::TimerCallback callback,
                                 int times) override;

    void run() override;

private:
    using TimerContainer = std::vector<ITimer::TimerPtr>;
    TimerContainer timers_;
};

} // namespace timer

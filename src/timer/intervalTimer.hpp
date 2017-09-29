#pragma once

#include "timer/ITimer.hpp"
#include "utils/types.hpp"

namespace timer
{

class IntervalTimer : public ITimer
{
public:
    IntervalTimer(u64 time, TimerCallback callback, int times = -1);

    void run() override;
    void cancel() override;
    bool enabled() const override;

protected:
    void fire() override;

    TimerCallback callback_;
    u64 startTime_;
    u64 time_;
    bool enabled_;
    int times_;
};

} // namespace timer

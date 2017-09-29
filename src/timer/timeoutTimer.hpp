#pragma once
#include "timer/ITimer.hpp"
#include "utils/types.hpp"

namespace timer
{

class TimeoutTimer : public ITimer
{
public:
    TimeoutTimer(u64 time, TimerCallback callback);

    void run() override;
    void cancel() override;
    bool enabled() const override;

protected:
    void fire() override;

    TimerCallback callback_;
    u64 startTime_;
    u64 time_;
    bool enabled_;
};

} // namespace timer

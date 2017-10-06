#pragma once

#include <functional>
#include <memory>

namespace timer
{

class ITimer
{
public:
    using TimerCallback = std::function<void()>;
    using TimerPtr = std::shared_ptr<ITimer>;

    ITimer() = default;
    virtual ~ITimer() = default;
    ITimer(const ITimer&) = delete;
    ITimer(const ITimer&&) = delete;
    ITimer& operator=(const ITimer&&) = delete;
    ITimer& operator=(const ITimer&) = delete;

    virtual void run() = 0;
    virtual void cancel() = 0;
    virtual bool enabled() const = 0;

protected:
    virtual void fire() = 0;
};


} // namespace timer

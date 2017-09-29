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

    virtual void run() = 0;
    virtual void cancel() = 0;
    virtual bool enabled() const = 0;

protected:
    virtual void fire() = 0;
};


} // namespace timer

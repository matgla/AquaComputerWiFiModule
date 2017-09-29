#include "timer/timeoutTimer.hpp"

#include <gtest/gtest.h>

#include "stub/timeStub.hpp"

namespace timer
{

TEST(TimeoutTimerShould, fireAfterTimeout)
{
    const u64 timeoutTime = 100;
    bool fired = false;
    stub::time::setCurrentTime(1000);
    ITimer::TimerPtr timer(new TimeoutTimer(timeoutTime, [&fired]() { fired = true; }));

    timer->run();
    EXPECT_FALSE(fired);
    EXPECT_TRUE(timer->enabled());
    stub::time::forwardTime(99);
    timer->run();
    EXPECT_FALSE(fired);
    EXPECT_TRUE(timer->enabled());

    stub::time::forwardTime(1);
    timer->run();
    EXPECT_TRUE(fired);
    EXPECT_FALSE(timer->enabled());

    fired = false;
    stub::time::forwardTime(100);
    timer->run();
    EXPECT_FALSE(fired);
    EXPECT_FALSE(timer->enabled());
}

TEST(TimeoutTimerShould, cancel)
{
    const u64 timeoutTime = 100;
    bool fired = false;
    stub::time::setCurrentTime(1000);
    ITimer::TimerPtr timer(new TimeoutTimer(timeoutTime, [&fired]() { fired = true; }));

    timer->run();
    EXPECT_FALSE(fired);
    EXPECT_TRUE(timer->enabled());
    stub::time::forwardTime(timeoutTime);
    timer->cancel();
    timer->run();
    EXPECT_FALSE(fired);
    EXPECT_FALSE(timer->enabled());
}

} // namespace timer

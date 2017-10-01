#include "timer/intervalTimer.hpp"

#include <gtest/gtest.h>

#include "stub/timeStub.hpp"

namespace timer
{

TEST(IntervalTimerShould, fireAfterTimeout)
{
    const u64 timeoutTime = 100;

    bool fired = false;
    stub::time::setCurrentTime(1000);
    ITimer::TimerPtr timer(new IntervalTimer(timeoutTime, [&fired]() { fired = true; }, 1));

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
}

TEST(IntervalTimerShould, fireMultipleAfterTimeout)
{
    const u64 timeoutTime = 100;
    const int fireTimes = 3;

    int firedTimes = 0;
    stub::time::setCurrentTime(1000);
    ITimer::TimerPtr timer(
        new IntervalTimer(timeoutTime, [&firedTimes]() { ++firedTimes; }, fireTimes));

    timer->run();
    EXPECT_EQ(0, firedTimes);
    EXPECT_TRUE(timer->enabled());

    stub::time::forwardTime(100);
    timer->run();
    EXPECT_EQ(1, firedTimes);
    EXPECT_TRUE(timer->enabled());

    stub::time::forwardTime(100);
    timer->run();
    EXPECT_EQ(2, firedTimes);
    EXPECT_TRUE(timer->enabled());

    stub::time::forwardTime(100);
    timer->run();
    EXPECT_EQ(3, firedTimes);
    EXPECT_FALSE(timer->enabled());

    stub::time::forwardTime(100);
    timer->run();
    EXPECT_EQ(3, firedTimes);
    EXPECT_FALSE(timer->enabled());
}

TEST(IntervalTimerShould, cancel)
{
    const u64 timeoutTime = 100;
    const int fireTimes = 3;

    int firedTimes = 0;
    stub::time::setCurrentTime(1000);
    ITimer::TimerPtr timer(
        new IntervalTimer(timeoutTime, [&firedTimes]() { ++firedTimes; }, fireTimes));

    timer->run();
    EXPECT_EQ(0, firedTimes);
    EXPECT_TRUE(timer->enabled());

    stub::time::forwardTime(100);
    timer->run();
    EXPECT_EQ(1, firedTimes);
    EXPECT_TRUE(timer->enabled());

    timer->cancel();
    stub::time::forwardTime(100);
    timer->run();
    EXPECT_EQ(1, firedTimes);
    EXPECT_FALSE(timer->enabled());

    stub::time::forwardTime(100);
    timer->run();
    EXPECT_EQ(1, firedTimes);
    EXPECT_FALSE(timer->enabled());
}

} // namespace timer

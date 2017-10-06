#include <gtest/gtest.h>

#include "timer/ITimer.hpp"
#include "timer/manager.hpp"

#include "stub/timeStub.hpp"

TEST(ManagerShould, run)
{
    stub::time::setCurrentTime(100);
    timer::Manager timerManager;

    int timeout1Fires = 0;
    int timeout2Fires = 0;
    int timeout3Fires = 0;
    int interval1Fires = 0;
    int interval2Fires = 0;
    int interval3Fires = 0;

    const int intervalFires = 2;

    auto timeout1 = timerManager.setTimeout(10, [&timeout1Fires]() { timeout1Fires++; });

    auto timeout2 = timerManager.setTimeout(15, [&timeout2Fires]() { timeout2Fires++; });

    auto timeout3 = timerManager.setTimeout(20, [&timeout3Fires]() { timeout3Fires++; });

    auto interval1 = timerManager.setInterval(5, [&interval1Fires]() { interval1Fires++; });
    auto interval2 =
        timerManager.setInterval(11, [&interval2Fires]() { interval2Fires++; }, intervalFires);
    auto interval3 = timerManager.setInterval(16, [&interval3Fires]() { interval3Fires++; });


    EXPECT_TRUE(timeout1->enabled());
    EXPECT_TRUE(timeout2->enabled());
    EXPECT_TRUE(timeout3->enabled());
    EXPECT_TRUE(interval1->enabled());
    EXPECT_TRUE(interval2->enabled());
    EXPECT_TRUE(interval3->enabled());
    EXPECT_EQ(0, timeout1Fires);
    EXPECT_EQ(0, timeout2Fires);
    EXPECT_EQ(0, timeout3Fires);
    EXPECT_EQ(0, interval1Fires);
    EXPECT_EQ(0, interval2Fires);
    EXPECT_EQ(0, interval3Fires);

    timerManager.run();

    EXPECT_TRUE(timeout1->enabled());
    EXPECT_TRUE(timeout2->enabled());
    EXPECT_TRUE(timeout3->enabled());
    EXPECT_TRUE(interval1->enabled());
    EXPECT_TRUE(interval2->enabled());
    EXPECT_TRUE(interval3->enabled());
    EXPECT_EQ(0, timeout1Fires);
    EXPECT_EQ(0, timeout2Fires);
    EXPECT_EQ(0, timeout3Fires);
    EXPECT_EQ(0, interval1Fires);
    EXPECT_EQ(0, interval2Fires);
    EXPECT_EQ(0, interval3Fires);

    timeout2->cancel();
    interval3->cancel();

    stub::time::forwardTime(10);
    timerManager.run();
    EXPECT_FALSE(timeout1->enabled());
    EXPECT_FALSE(timeout2->enabled());
    EXPECT_FALSE(interval3->enabled());
    EXPECT_TRUE(timeout3->enabled());
    EXPECT_TRUE(interval1->enabled());
    EXPECT_TRUE(interval2->enabled());
    EXPECT_EQ(1, timeout1Fires);
    EXPECT_EQ(0, timeout2Fires);
    EXPECT_EQ(0, timeout3Fires);
    EXPECT_EQ(1, interval1Fires);
    EXPECT_EQ(0, interval2Fires);
    EXPECT_EQ(0, interval3Fires);

    stub::time::forwardTime(1);
    timerManager.run();
    EXPECT_FALSE(timeout1->enabled());
    EXPECT_FALSE(timeout2->enabled());
    EXPECT_FALSE(interval3->enabled());
    EXPECT_TRUE(timeout3->enabled());
    EXPECT_TRUE(interval1->enabled());
    EXPECT_TRUE(interval2->enabled());
    EXPECT_EQ(1, timeout1Fires);
    EXPECT_EQ(0, timeout2Fires);
    EXPECT_EQ(0, timeout3Fires);
    EXPECT_EQ(1, interval1Fires);
    EXPECT_EQ(1, interval2Fires);
    EXPECT_EQ(0, interval3Fires);

    stub::time::forwardTime(40);
    timerManager.run();

    EXPECT_FALSE(timeout1->enabled());
    EXPECT_FALSE(timeout2->enabled());
    EXPECT_FALSE(timeout3->enabled());
    EXPECT_FALSE(interval2->enabled());
    EXPECT_FALSE(interval3->enabled());

    EXPECT_TRUE(interval1->enabled());

    EXPECT_EQ(1, timeout1Fires);
    EXPECT_EQ(0, timeout2Fires);
    EXPECT_EQ(1, timeout3Fires);
    EXPECT_EQ(2, interval1Fires);
    EXPECT_EQ(2, interval2Fires);
    EXPECT_EQ(0, interval3Fires);

    for (int i = 0; i < 10000; ++i)
    {
        stub::time::forwardTime(1);
        timerManager.run();
    }

    EXPECT_FALSE(timeout1->enabled());
    EXPECT_FALSE(timeout2->enabled());
    EXPECT_FALSE(timeout3->enabled());
    EXPECT_FALSE(interval2->enabled());
    EXPECT_FALSE(interval3->enabled());

    EXPECT_TRUE(interval1->enabled());
    EXPECT_EQ(1, timeout1Fires);
    EXPECT_EQ(0, timeout2Fires);
    EXPECT_EQ(1, timeout3Fires);
    EXPECT_EQ(2002, interval1Fires);
    EXPECT_EQ(2, interval2Fires);
    EXPECT_EQ(0, interval3Fires);
}

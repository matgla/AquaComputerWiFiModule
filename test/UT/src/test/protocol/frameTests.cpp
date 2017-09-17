#include "protocol/frame.hpp"

#include <gtest/gtest.h>

#include "matcher/arrayCompare.hpp"

using namespace testing;

namespace protocol
{

TEST(FrameShould, SetPortAndMessageNumber)
{
    Frame<255> frame;

    EXPECT_EQ(0, frame.port());
    EXPECT_EQ(0, frame.number());

    const u8 port = 10;
    const u8 frameNumber = 40;
    frame.port(port);
    frame.number(frameNumber);
    EXPECT_EQ(port, frame.port());
    EXPECT_EQ(frameNumber, frame.number());
}

TEST(FrameShould, ReturnMaxPayloadSize)
{
    const u8 payloadSize = 10;
    Frame<payloadSize> frame;
    EXPECT_EQ(payloadSize, frame.payloadSize());
}

TEST(FrameShould, Clear)
{
    Frame<4> frame;
    const u8 payload[] = {1, 2, 3, 4};
    frame.port(12);
    frame.number(14);
    frame.control(1);
    frame.payload(payload, sizeof(payload));

    EXPECT_EQ(12, frame.port());
    EXPECT_EQ(14, frame.number());
    EXPECT_EQ(1, frame.control());
    EXPECT_EQ(sizeof(payload), frame.length());
    frame.clear();
    EXPECT_EQ(0, frame.port());
    EXPECT_EQ(0, frame.number());
    EXPECT_EQ(0, frame.control());
    EXPECT_EQ(0, frame.length());
}

TEST(FrameShould, NotOverflow)
{
    const u8 frameSize = 2;
    Frame<frameSize> frame;
    const u8 payload[] = {1, 2, 3, 4};
    const u8 insertedPayload[] = {1, 2};

    EXPECT_EQ(frameSize, frame.payload(payload, sizeof(payload)));
    EXPECT_EQ(0, frame.payload(payload, sizeof(payload)));
    EXPECT_THAT(frame.payload(), ArrayCompare(insertedPayload, sizeof(insertedPayload)));

    frame.clear();
    EXPECT_EQ(frameSize, frame.payload(payload + 1, sizeof(payload) - 1));
    EXPECT_EQ(0, frame.payload(payload, sizeof(payload)));
    const u8 insertedPayload2[] = {2, 3};
    EXPECT_THAT(frame.payload(), ArrayCompare(insertedPayload2, sizeof(insertedPayload2)));
}

} // namespace protocol

#include "protocol/frameHandler.hpp"

#include <gtest/gtest.h>

namespace protocol
{
TEST(FrameHandlerShould, Create)
{
    FrameHandler handler;

    const u8 payload[] = {0x12, 0xaa};
    u8 frameNumber = 0;
    const u16 testingPort = 10;
    const u16 controlByte = 0;

    const u8 frame[] = {
        FrameByte::Start, sizeof(payload), frameNumber, testingPort, controlByte,
        payload[0],       payload[1],      0x0,         0x0,         FrameByte::End};

    handler.onRead(frame, sizeof(frame));
    Frame<10> testFrame;
    testFrame.number(0);
    testFrame.port(testingPort);
    testFrame.control(0);

    // handler.send
}
}

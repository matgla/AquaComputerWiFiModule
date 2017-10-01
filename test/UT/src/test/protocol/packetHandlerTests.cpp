#include <gtest/gtest.h>

#include "protocol/packetHandler.hpp"

#include "timer/manager.hpp"

#include "helper/frameHelper.hpp"
#include "matcher/arrayCompare.hpp"
#include "stub/receiverStub.hpp"
#include "stub/timeStub.hpp"

namespace protocol
{

TEST(PacketHandlerShould, SendMessage)
{
    stub::time::setCurrentTime(0);
    const u16 testingPort = 10;
    const u8 messageNr = 0;
    u8 currentFrameNumber = 0;
    const auto receiver(std::make_shared<stub::ReceiverStub>());
    timer::Manager timerManager;

    PacketHandler packetHandler(testingPort, receiver, timerManager);
    DataBuffer testingPayload = {0xa, 0xb, 0xc, 0xd};
    testingPayload.resize(300);
    testingPayload.push_back(0xab);
    testingPayload.push_back(0xac);
    testingPayload.push_back(0xad);
    testingPayload.push_back(0xff);

    packetHandler.send(testingPayload);

    const auto expectedFrame =
        helper::createHeader(testingPayload, testingPort, messageNr, currentFrameNumber);

    // received header?
    EXPECT_THAT(receiver->writeBuffer.data(),
                ArrayCompare(expectedFrame.data(), expectedFrame.size()));

    receiver->writeBuffer.clear();
    auto ack = helper::createAck(testingPort, currentFrameNumber++);
    // ack header
    receiver->readerCallback(ack.data(), ack.size(), dispatcher::defaultWriter);
    // received payload ?
    int startIndex = 0;
    auto expectedMessage =
        helper::createFrame(testingPayload, testingPort, currentFrameNumber, startIndex);
    EXPECT_THAT(receiver->writeBuffer.data(),
                ArrayCompare(expectedMessage.data(), expectedMessage.size()));

    // ack first part
    receiver->writeBuffer.clear();
    auto ackFrame1 = helper::createAck(testingPort, currentFrameNumber++);
    receiver->readerCallback(ackFrame1.data(), ackFrame1.size(), dispatcher::defaultWriter);

    // received rest of payload?
    auto expectedPart2 = helper::createFrame(testingPayload, testingPort, currentFrameNumber,
                                             protocol::MaxPayloadSize);
    EXPECT_THAT(receiver->writeBuffer.data(),
                ArrayCompare(expectedPart2.data(), expectedPart2.size()));

    // ack second part
    receiver->writeBuffer.clear();
    auto ackFrame2 = helper::createAck(testingPort, currentFrameNumber++);
    receiver->readerCallback(ackFrame2.data(), ackFrame2.size(), dispatcher::defaultWriter);

    // received CRC32 for message?
    auto crcPart = helper::createCrc32Frame(testingPort, currentFrameNumber++, testingPayload);
    std::cerr << "Received: " << std::to_string(receiver->writeBuffer) << std::endl;
    EXPECT_THAT(receiver->writeBuffer.data(), ArrayCompare(crcPart.data(), crcPart.size()));
}
}

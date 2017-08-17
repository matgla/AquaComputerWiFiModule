#include "handler/baseHandler.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "matcher/arrayCompare.hpp"
#include "message/messages.hpp"
#include "mock/writerHandlerMock.hpp"

using namespace testing;

namespace handler
{

struct ReceiverForTest : public handler::IDataReceiver
{
    void setHandler(ReaderCallback readerCallback) override
    {
        readerCallback_ = readerCallback;
    }

    ReaderCallback readerCallback_;
};

struct BaseHandlerShould : public testing::Test
{
    BaseHandlerShould() : handler_(std::make_unique<ReceiverForTest>())
    {
        receiver_ = static_cast<ReceiverForTest*>(handler_.getReceiver());
    }

    handler::BaseHandler handler_;
    ReceiverForTest* receiver_;
};

TEST_F(BaseHandlerShould, InitializeMembers)
{
    EXPECT_FALSE(handler_.transmissionStarted());
    EXPECT_FALSE(handler_.allDataArrived());
}

TEST_F(BaseHandlerShould, StartTransmissionAndSendAck)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionMessage::Ack};
    u8 msg[] = {message::TransmissionMessage::Start};

    test::mock::WriterHandlerMock writerMock;

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_TRUE(handler_.transmissionStarted());
}

TEST_F(BaseHandlerShould, NackWhenNotStartedAndNotAckReceived)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionMessage::Nack};
    const u8 someMessageDifferentThatStart = ~message::TransmissionMessage::Start;
    const u8 msg[] = {someMessageDifferentThatStart};

    test::mock::WriterHandlerMock writerMock;

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_FALSE(handler_.transmissionStarted());
}

template <typename T>
void serialize(u8* buffer, const T& data)
{
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        const u8* byte = reinterpret_cast<const u8*>(&data);
        buffer[i] = *byte;
        ++byte;
    }
}

// TEST_F(BaseHandlerShould, ReceiveMessageLengthCorrectly)
// {
//     EXPECT_FALSE(handler_.transmissionStarted());

//     const u8 expectedAnswer[] = {message::TransmissionMessage::Ack};

//     u64 messageLength = 0xbeef12345678baca;
//     // beef12345678baca
//     // ffffffffcacacaca
//     u8 msg[9] = {message::TransmissionMessage::Start};
//     serialize(&msg[1], messageLength);

//     test::mock::WriterHandlerMock writerMock;

//     EXPECT_EQ(0, handler_.lengthToBeReceived());
//     EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

//     receiver_->readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
//         writerMock.doWrite(buf, len);
//     });

//     EXPECT_EQ(messageLength, handler_.lengthToBeReceived());
//     EXPECT_TRUE(handler_.transmissionStarted());
// }

} // namespace handler

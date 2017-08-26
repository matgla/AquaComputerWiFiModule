#include "handler/MessageReceiver.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ArduinoJson.h>

#include "matcher/arrayCompare.hpp"
#include "message/messages.hpp"
#include "mock/writerHandlerMock.hpp"
#include "serializer/serializer.hpp"

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

struct MessageHandler
{
    std::string data_;

    void handle(std::vector<u8> data)
    {
        for (auto& byte : data)
        {
            data_ += static_cast<char>(byte);
        }
    }
};

struct MessageReceiverShould : public testing::Test
{
    MessageReceiverShould();

    handler::MessageReceiver receiver_;
    MessageHandler handler_;
    ReceiverForTest dataReceiver_;
};

MessageReceiverShould::MessageReceiverShould()
    : receiver_(std::bind(MessageHandler::handle, &handler_, std::placeholders::_1))
{
    dataReceiver_.setHandler(std::bind(&MessageReceiver::onRead, &receiver_, std::placeholders::_1,
                                       std::placeholders::_2, std::placeholders::_3));
}

TEST_F(MessageReceiverShould, InitializeMembers)
{
    EXPECT_FALSE(receiver_.transmissionStarted());
}

TEST_F(MessageReceiverShould, StartTransmissionAndSendAck)
{
    EXPECT_FALSE(receiver_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Ack};
    u8 msg[] = {message::TransmissionId::Start};

    test::mock::WriterHandlerMock writerMock;

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    dataReceiver_.readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_TRUE(receiver_.transmissionStarted());
}

TEST_F(MessageReceiverShould, NackWhenNotStartedAndDataArrived)
{
    EXPECT_FALSE(receiver_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Nack};
    const u8 someMessageDifferentThatStart = ~message::TransmissionId::Start;
    const u8 msg[] = {someMessageDifferentThatStart};

    test::mock::WriterHandlerMock writerMock;

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    dataReceiver_.readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_FALSE(receiver_.transmissionStarted());
}

TEST_F(MessageReceiverShould, SendMessageLength)
{
    EXPECT_FALSE(receiver_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Nack};
    const u8 someMessageDifferentThatStart = ~message::TransmissionId::Start;
    const u8 msg[] = {someMessageDifferentThatStart};

    test::mock::WriterHandlerMock writerMock;

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    dataReceiver_.readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_FALSE(receiver_.transmissionStarted());
}

TEST_F(MessageReceiverShould, ReceiveMessageLengthCorrectly)
{
    EXPECT_FALSE(receiver_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Ack};

    const u64 messageLength = 0x0f;

    u8 msg[9] = {message::TransmissionId::Start};
    serializer::serialize(&msg[1], messageLength);

    test::mock::WriterHandlerMock writerMock;

    EXPECT_EQ(0, receiver_.lengthToBeReceived());
    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    dataReceiver_.readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_EQ(messageLength, receiver_.lengthToBeReceived());
    EXPECT_TRUE(receiver_.transmissionStarted());
}

TEST_F(MessageReceiverShould, ReceiveMessageCorrectly)
{
    EXPECT_FALSE(receiver_.transmissionStarted());

    DynamicJsonBuffer messageBuffer;
    JsonObject& message = messageBuffer.createObject();

    message["testing_string"] = "dynamic payload works";
    message["testing_float"] = 3.14;

    JsonArray& testingArray = message.createNestedArray("testing_array");
    testingArray.add(1);
    testingArray.add(2);
    testingArray.add(3);

    std::string serializedMessage;
    test::mock::WriterHandlerMock writerMock;

    message.printTo(serializedMessage);

    u8 msg[9] = {message::TransmissionId::Start};
    serializer::serialize(&msg[1], serializedMessage.length());

    EXPECT_EQ(0, receiver_.lengthToBeReceived());
    const u8 expectedAnswer[] = {message::TransmissionId::Ack};
    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    // send protocol data
    dataReceiver_.readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_EQ(serializedMessage.length(), receiver_.lengthToBeReceived());
    EXPECT_TRUE(receiver_.transmissionStarted());

    std::size_t dataSplit = serializedMessage.length() / 2;

    dataReceiver_.readerCallback_(reinterpret_cast<const u8*>(serializedMessage.c_str()), dataSplit,
                                  [](const u8* buf, std::size_t len) {});
    dataReceiver_.readerCallback_(
        reinterpret_cast<const u8*>(serializedMessage.c_str() + dataSplit),
        serializedMessage.length() - dataSplit, [](const u8* buf, std::size_t len) {});

    EXPECT_EQ(serializedMessage, handler_.data_);
    EXPECT_FALSE(receiver_.transmissionStarted());
}

TEST_F(MessageReceiverShould, ReceiveParts)
{
    test::mock::WriterHandlerMock writerMock;

    const u8 expectedAnswer[] = {message::TransmissionId::Ack};

    EXPECT_FALSE(receiver_.transmissionStarted());

    std::string msg1 = "ABCD";
    std::string msg2 = "ALA MA KOTTA";

    std::vector<u8> transmission1;
    transmission1.push_back(message::TransmissionId::Start);
    transmission1.resize(transmission1.size() + 8);
    serializer::serialize(transmission1.data() + 1, msg1.length());
    transmission1.resize(transmission1.size() + msg1.length());
    memcpy(transmission1.data() + 9, msg1.c_str(), msg1.length());
    transmission1.push_back(message::TransmissionId::Start);

    std::vector<u8> transmission2;
    transmission2.resize(transmission2.size() + 8);
    serializer::serialize(transmission2.data(), msg2.length());
    transmission2.resize(transmission2.size() + msg2.length());
    memcpy(transmission2.data() + 8, msg2.c_str(), msg2.length());


    EXPECT_EQ(0, receiver_.lengthToBeReceived());

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1))
        .Times(2);


    // send protocol data
    dataReceiver_.readerCallback_(
        reinterpret_cast<const u8*>(transmission1.data()), transmission1.size(),
        [&writerMock](const auto* buf, auto len) { writerMock.doWrite(buf, len); });

    EXPECT_EQ(msg1, handler_.data_);
    handler_.data_.clear();

    // next message queued so this should be cleaned
    EXPECT_EQ(0, receiver_.lengthToBeReceived());
    EXPECT_TRUE(receiver_.transmissionStarted());

    dataReceiver_.readerCallback_(
        reinterpret_cast<const u8*>(transmission2.data()), transmission2.size(),
        [&writerMock](const auto* buf, auto len) { writerMock.doWrite(buf, len); });

    EXPECT_EQ(msg2, handler_.data_);
    EXPECT_FALSE(receiver_.transmissionStarted());
}

} // namespace handler

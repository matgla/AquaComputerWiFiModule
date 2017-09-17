#include "dispatcher/MessageHandler.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ArduinoJson.h>

#include "dispatcher/IDataReceiver.hpp"

#include "matcher/arrayCompare.hpp"
#include "message/messages.hpp"
#include "mock/writerHandlerMock.hpp"
#include "serializer/serializer.hpp"

using namespace testing;

namespace dispatcher
{

class MessageHandlerForTest : public MessageHandler
{
public:
    std::string data_;
    void handleData(const DataBuffer& data)
    {
        for (const auto& byte : data)
        {
            data_ += static_cast<char>(byte);
        }
    }
};

struct ReceiverForTest : public dispatcher::IDataReceiver
{
    void setHandler(ReaderCallback readerCallback) override
    {
        readerCallback_ = readerCallback;
    }

    void write(const std::string& data)
    {
        writeBuffer_.resize(writeBuffer_.size() + data.length());
        memcpy(writeBuffer_.data() + writeBuffer_.size() - data.length(), data.c_str(),
               data.length());
    }

    void write(const u8* buf, std::size_t size)
    {
        writeBuffer_.resize(writeBuffer_.size() + size);
        memcpy(writeBuffer_.data() + writeBuffer_.size() - size, buf, size);
    }

    void write(u8 byte)
    {
        writeBuffer_.push_back(byte);
    }

    void clearBuffers()
    {
        writeBuffer_.clear();
    }

    ReaderCallback readerCallback_;

    DataBuffer writeBuffer_;
};

// struct MessageHandler
// {
//     std::string data_;

//     void handle(std::vector<u8> data)
//     {
//         for (auto& byte : data)
//         {
//             data_ += static_cast<char>(byte);
//         }
//     }
// };

struct MessageHandlerShould : public testing::Test
{
    MessageHandlerShould();

    MessageHandlerForTest handler_;
    std::shared_ptr<ReceiverForTest> receiver_;

    test::mock::WriterHandlerMock writerMock_;
};

MessageHandlerShould::MessageHandlerShould() : receiver_(new ReceiverForTest())
{
    receiver_->setHandler(std::bind(&IFrameHandler::onRead, &handler_, std::placeholders::_1,
                                    std::placeholders::_2, std::placeholders::_3));
}

TEST_F(MessageHandlerShould, InitializeMembers)
{
    EXPECT_FALSE(handler_.transmissionStarted());
}

TEST_F(MessageHandlerShould, StartTransmissionAndSendAck)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Ack};
    u8 msg[] = {message::TransmissionId::Start};


    EXPECT_CALL(writerMock_, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(
        msg, sizeof(msg), [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_TRUE(handler_.transmissionStarted());
}

TEST_F(MessageHandlerShould, NackWhenNotStartedAndDataArrived)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Nack};
    const u8 someMessageDifferentThatStart = ~message::TransmissionId::Start;
    const u8 msg[] = {someMessageDifferentThatStart};

    EXPECT_CALL(writerMock_, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(
        msg, sizeof(msg), [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_FALSE(handler_.transmissionStarted());
}

TEST_F(MessageHandlerShould, SendMessageLength)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Nack};
    const u8 someMessageDifferentThatStart = ~message::TransmissionId::Start;
    const u8 msg[] = {someMessageDifferentThatStart};

    EXPECT_CALL(writerMock_, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(
        msg, sizeof(msg), [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_FALSE(handler_.transmissionStarted());
}

TEST_F(MessageHandlerShould, ReceiveMessageLengthCorrectly)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Ack};

    const u64 messageLength = 0x0f;

    u8 msg[9] = {message::TransmissionId::Start};
    serializer::serialize(&msg[1], messageLength);

    EXPECT_EQ(0, handler_.lengthToBeReceived());
    EXPECT_CALL(writerMock_, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(
        msg, sizeof(msg), [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_EQ(messageLength, handler_.lengthToBeReceived());
    EXPECT_TRUE(handler_.transmissionStarted());
}

TEST_F(MessageHandlerShould, ReceiveMessageCorrectly)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    DynamicJsonBuffer messageBuffer;
    JsonObject& message = messageBuffer.createObject();

    message["testing_string"] = "dynamic payload works";
    message["testing_float"] = 3.14;

    JsonArray& testingArray = message.createNestedArray("testing_array");
    testingArray.add(1);
    testingArray.add(2);
    testingArray.add(3);

    std::string serializedMessage;

    message.printTo(serializedMessage);

    u8 msg[9] = {message::TransmissionId::Start};
    serializer::serialize(&msg[1], serializedMessage.length());

    EXPECT_EQ(0, handler_.lengthToBeReceived());
    const u8 expectedAnswer[] = {message::TransmissionId::Ack};
    EXPECT_CALL(writerMock_, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    // send protocol data
    receiver_->readerCallback_(
        msg, sizeof(msg), [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_EQ(serializedMessage.length(), handler_.lengthToBeReceived());
    EXPECT_TRUE(handler_.transmissionStarted());

    std::size_t dataSplit = serializedMessage.length() / 2;

    receiver_->readerCallback_(reinterpret_cast<const u8*>(serializedMessage.c_str()), dataSplit,
                               [](const u8* buf, std::size_t len) {});
    receiver_->readerCallback_(reinterpret_cast<const u8*>(serializedMessage.c_str() + dataSplit),
                               serializedMessage.length() - dataSplit,
                               [](const u8* buf, std::size_t len) {});

    EXPECT_EQ(serializedMessage, handler_.data_);
    EXPECT_FALSE(handler_.transmissionStarted());
}

TEST_F(MessageHandlerShould, ReceiveParts)
{
    const u8 expectedAnswer[] = {message::TransmissionId::Ack};

    EXPECT_FALSE(handler_.transmissionStarted());

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


    EXPECT_EQ(0, handler_.lengthToBeReceived());

    EXPECT_CALL(writerMock_, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1))
        .Times(2);

    // send protocol data
    receiver_->readerCallback_(
        reinterpret_cast<const u8*>(transmission1.data()), transmission1.size(),
        [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_EQ(msg1, handler_.data_);
    handler_.data_.clear();

    // next message queued so this should be cleaned
    EXPECT_EQ(0, handler_.lengthToBeReceived());
    EXPECT_TRUE(handler_.transmissionStarted());

    receiver_->readerCallback_(
        reinterpret_cast<const u8*>(transmission2.data()), transmission2.size(),
        [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_EQ(msg2, handler_.data_);
    EXPECT_FALSE(handler_.transmissionStarted());
}

TEST_F(MessageHandlerShould, SendMessageForReceiver)
{
    handler_.setConnection(receiver_);

    const std::string msg = "SOME PAYLOAD :P";
    DataBuffer buffer;
    buffer.resize(msg.length());
    memcpy(buffer.data(), msg.c_str(), msg.length());

    handler_.send(buffer);

    ASSERT_EQ(1, receiver_->writeBuffer_.size());
    EXPECT_EQ(message::TransmissionId::Start, receiver_->writeBuffer_.front());

    receiver_->clearBuffers();

    const u8 ackTransfer[] = {message::TransmissionId::Ack};

    std::vector<u8> expectedAnswer;
    expectedAnswer.resize(expectedAnswer.size() + 8);
    serializer::serialize(expectedAnswer.data(), msg.length());
    expectedAnswer.resize(expectedAnswer.size() + msg.length());
    memcpy(expectedAnswer.data() + 8, msg.c_str(), msg.length());

    DataBuffer receivedData;

    receiver_->readerCallback_(ackTransfer, sizeof(ackTransfer),
                               [&receivedData](const auto* buf, auto len) {
                                   for (std::size_t i = 0; i < len; i++)
                                   {
                                       receivedData.push_back(buf[i]);
                                   }
                               });

    EXPECT_EQ(expectedAnswer, receivedData);

    receivedData.clear();

    handler_.send(buffer);
    handler_.send(buffer);

    ASSERT_EQ(2, receiver_->writeBuffer_.size());
    EXPECT_EQ(message::TransmissionId::Start, receiver_->writeBuffer_.front());
    EXPECT_EQ(message::TransmissionId::Start, receiver_->writeBuffer_.back());

    receiver_->readerCallback_(ackTransfer, sizeof(ackTransfer),
                               [&receivedData](const auto* buf, auto len) {
                                   for (std::size_t i = 0; i < len; i++)
                                   {
                                       receivedData.push_back(buf[i]);
                                   }
                               });

    EXPECT_EQ(expectedAnswer, receivedData);
    receivedData.clear();

    receiver_->readerCallback_(ackTransfer, sizeof(ackTransfer),
                               [&receivedData](const auto* buf, auto len) {
                                   for (std::size_t i = 0; i < len; i++)
                                   {
                                       receivedData.push_back(buf[i]);
                                   }
                               });

    EXPECT_EQ(expectedAnswer, receivedData);
    receivedData.clear();
}

TEST_F(MessageHandlerShould, NotSendStartWhileTransmissionOngoing)
{
    handler_.setConnection(receiver_);

    EXPECT_FALSE(handler_.transmissionStarted());

    const u8 expectedAnswer[] = {message::TransmissionId::Ack};
    u8 startMessage[] = {message::TransmissionId::Start};

    EXPECT_CALL(writerMock_, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(
        startMessage, sizeof(startMessage),
        [this](const auto* buf, auto len) { writerMock_.doWrite(buf, len); });

    EXPECT_TRUE(handler_.transmissionStarted());

    const std::string msg = "SOME PAYLOAD :P";
    DataBuffer buffer;
    buffer.resize(msg.length());
    memcpy(buffer.data(), msg.c_str(), msg.length());

    handler_.send(buffer);

    EXPECT_EQ(0, receiver_->writeBuffer_.size());
    receiver_->clearBuffers();
}

} // namespace dispatcher

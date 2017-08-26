#include "handler/messageReceiver.hpp"

#include <functional>

#include "message/messages.hpp"

namespace handler
{

const u8 LENGTH_SIZE = 8; // bytes

MessageReceiver::MessageReceiver(DataHandler handler)
    : handler_(handler), transmissionStarted_(false), lengthToBeReceived_(0),
      messageLengthReceived_(false), messageLengthToBeReceived_(0)
{
}

bool MessageReceiver::transmissionStarted()
{
    return transmissionStarted_;
}

u64 MessageReceiver::lengthToBeReceived()
{
    return messageLengthToBeReceived_;
}

void MessageReceiver::receiveMessageLength(u8 data)
{
    messageLengthToBeReceived_ |= static_cast<u64>(data) << 8 * (LENGTH_SIZE - lengthToBeReceived_);
    --lengthToBeReceived_;
    if (0 == lengthToBeReceived_)
    {
        messageLengthReceived_ = true;
        buffer_.reserve(messageLengthToBeReceived_);
    }
}

u8 MessageReceiver::initializeTransmission(u8 data)
{
    if (data == message::TransmissionId::Start)
    {
        transmissionStarted_ = true;
        messageLengthReceived_ = false;
        lengthToBeReceived_ = LENGTH_SIZE;
        messageLengthToBeReceived_ = 0;

        return message::TransmissionId::Ack;
    }
    else
    {
        return message::TransmissionId::Nack;
    }
}

void MessageReceiver::onRead(const u8* buffer, std::size_t length, WriterCallback write)
{
    for (std::size_t i = 0; i < length; ++i)
    {
        if (!transmissionStarted_)
        {
            reply(initializeTransmission(buffer[i]), write);
            continue;
        }

        if (!messageLengthReceived_)
        {
            receiveMessageLength(buffer[i]);
            continue;
        }

        buffer_.push_back(buffer[i]);
        if (0 == --messageLengthToBeReceived_)
        {
            // Received whole message
            handler_(buffer_);
            buffer_.clear();
            transmissionStarted_ = false;
        }
    }
}

void MessageReceiver::reply(const u8 answer, WriterCallback& write) const
{
    u8 answerMessage[] = {answer};
    write(answerMessage, sizeof(answerMessage));
}

} // namespace handler

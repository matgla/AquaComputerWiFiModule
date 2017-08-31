#include "handler/MessageHandler.hpp"

#include <functional>

#include "message/messages.hpp"

namespace handler
{

const u8 LENGTH_SIZE = 8; // bytes

MessageHandler::MessageHandler()
    : transmissionStarted_(false), lengthToBeReceived_(0), messageLengthReceived_(false),
      messageLengthToBeReceived_(0)
{
}

bool MessageHandler::transmissionStarted()
{
    return transmissionStarted_;
}

u64 MessageHandler::lengthToBeReceived()
{
    return messageLengthToBeReceived_;
}

void MessageHandler::receiveMessageLength(u8 data)
{
    messageLengthToBeReceived_ |= static_cast<u64>(data) << 8 * (LENGTH_SIZE - lengthToBeReceived_);
    --lengthToBeReceived_;
    if (0 == lengthToBeReceived_)
    {
        messageLengthReceived_ = true;
        buffer_.reserve(messageLengthToBeReceived_);
    }
}

u8 MessageHandler::initializeTransmission(u8 data)
{
    if (message::TransmissionId::Start == data)
    {
        transmissionStarted_ = true;
        messageLengthReceived_ = false;
        lengthToBeReceived_ = LENGTH_SIZE;
        messageLengthToBeReceived_ = 0;

        return message::TransmissionId::Ack;
    }

    if (message::TransmissionId::Ack == data) // transmission accepted, send queued message
    {
        // handler_
    }

    return message::TransmissionId::Nack;
}

void MessageHandler::onRead(const u8* buffer, std::size_t length, WriterCallback write)
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
            handleData(buffer_);
            buffer_.clear();
            transmissionStarted_ = false;
        }
    }
}

void MessageHandler::reply(const u8 answer, WriterCallback& write) const
{
    u8 answerMessage[] = {answer};
    write(answerMessage, sizeof(answerMessage));
}

void MessageHandler::send(const DataBuffer& data)
{
    // TODO: implement me
}

} // namespace handler

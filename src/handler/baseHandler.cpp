#include "handler/baseHandler.hpp"

#include <functional>

#include "message/messages.hpp"

namespace handler
{

const u8 LENGTH_SIZE = 8; // bytes

BaseHandler::BaseHandler(ReceiverPtr receiver)
    : receiver_(std::move(receiver)), transmissionStarted_(false), allDataArrived_(false),
      lengthToBeReceived_(0)
{
    receiver_->setHandler(std::bind(&BaseHandler::onRead, this, std::placeholders::_1,
                                    std::placeholders::_2, std::placeholders::_3));
}

IDataReceiver* BaseHandler::getReceiver()
{
    return receiver_.get();
}

std::vector<u8>& BaseHandler::getBuffer()
{
    return buffer_;
}

bool BaseHandler::transmissionStarted()
{
    return transmissionStarted_;
}

bool BaseHandler::allDataArrived()
{
    return allDataArrived_;
}

void BaseHandler::transmissionStarted(bool transmissionStarted)
{
    transmissionStarted_ = transmissionStarted;
}

void BaseHandler::allDataArrived(bool allDataArrived)
{
    allDataArrived_ = allDataArrived;
}

u64 BaseHandler::lengthToBeReceived()
{
    return messageLengthToBeReceived_;
}

void BaseHandler::receiveMessageLength(u8 data)
{
    messageLengthToBeReceived_ |= static_cast<u64>(data) << 8 * (LENGTH_SIZE - lengthToBeReceived_);
    --lengthToBeReceived_;

    if (0 == lengthToBeReceived_)
    {
        messageLengthReceived_ = true;
    }
}

u8 BaseHandler::initializeTransmission(u8 data)
{
    if (data == message::TransmissionMessage::Start)
    {
        transmissionStarted_ = true;
        allDataArrived_ = false;
        messageLengthReceived_ = false;
        lengthToBeReceived_ = LENGTH_SIZE;
        messageLengthToBeReceived_ = 0;

        return message::TransmissionMessage::Ack;
    }
    else
    {
        return message::TransmissionMessage::Nack;
    }
}

void BaseHandler::onRead(const u8* buffer, std::size_t length, WriterCallback write)
{
    for (std::size_t i = 0; i < length; ++i)
    {
        if (!transmissionStarted_)
        {
            reply(initializeTransmission(buffer[i]), write);
            continue;
        }
        else if (!messageLengthReceived_)
        {
            receiveMessageLength(buffer[i]);
            continue;
        }
    }
}

void BaseHandler::reply(const u8 answer, WriterCallback& write) const
{
    u8 answerMessage[] = {answer};
    write(answerMessage, sizeof(answerMessage));
}

} // namespace handler

#include "handler/baseHandler.hpp"

#include <functional>

namespace handler
{

BaseHandler::BaseHandler(ReceiverPtr receiver)
    : receiver_(std::move(receiver)), transmissionStarted_(false), allDataArrived_(false)
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

void BaseHandler::onRead(const u8* buffer, std::size_t length, WriterCallback write)
{
    if (!transmissionStarted_)
    {
        transmissionStarted_ = true;
        allDataArrived_ = false;
        u8 ack[] = {0xFF};
        write(ack, sizeof(ack));
    }
}

} // namespace handler

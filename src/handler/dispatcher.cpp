#include "handler/dispatcher.hpp"

namespace handler
{

void Dispatcher::addReceiver(HandlerPtr receiver)
{
    receiver->getReceiver()->setHandler(std::bind(&Dispatcher::onRead, this,
                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    receivers_.push_back(std::move(receiver));
}

void Dispatcher::onRead(const u8* buffer, std::size_t length, WriterCallback write)
{
}

} // namespace handler
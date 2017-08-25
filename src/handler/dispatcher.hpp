#pragma once

#include <memory>
#include <vector>

#include "handler/messageReceiver.hpp"

namespace handler
{
class Dispatcher : public std::enable_shared_from_this<Dispatcher>
{
public:
    using HandlerPtr = std::unique_ptr<MessageReceiver>;
    using HandlerContainer = std::vector<HandlerPtr>;

    void addReceiver(HandlerPtr receiver);

protected:
    void onRead(const u8* buffer, std::size_t length, WriterCallback write);
    HandlerContainer receivers_;
};

} // namespace handler

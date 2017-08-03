#pragma once

#include <functional>
#include <memory>
#include <string>

#include "utils/types.hpp"

namespace net
{

class Message
{
public:
    u8* getPayload();

private:
    void* payload;
};

class WebSocket
{
public:
    using HandlerCallback = std::function<void(std::unique_ptr<Message>)>;
    WebSocket(const std::string& uri, u16 port);
    ~WebSocket();

    void start();
    void stop();

    void setHandler(HandlerCallback handler);

private:
    HandlerCallback handler_;
    class WebSocketWrapper;
    std::unique_ptr<WebSocketWrapper> webSocketWrapper_;
};

} // namespace net

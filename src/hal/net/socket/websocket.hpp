#pragma once

#include <functional>
#include <memory>
#include <string>

#include "dispatcher/IDataReceiver.hpp"
#include "dispatcher/handlers.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{
namespace socket
{
class WebSocket : public dispatcher::IDataReceiver
{
public:
    WebSocket(const std::string& uri, u16 port,
              dispatcher::ReaderCallback handler = dispatcher::defaultReader);
    ~WebSocket() override;
    WebSocket(const WebSocket&) = delete;
    WebSocket(const WebSocket&&) = delete;
    WebSocket& operator=(const WebSocket&&) = delete;
    WebSocket& operator=(const WebSocket&) = delete;

    void start();
    void stop();

    void setHandler(const dispatcher::ReaderCallback& handler) override;

private:
    class WebSocketWrapper;
    std::unique_ptr<WebSocketWrapper> webSocketWrapper_;
};

} // namespace net
} // namespace hal
} // namespace socket

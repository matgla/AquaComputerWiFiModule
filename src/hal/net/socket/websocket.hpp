#pragma once

#include <functional>
#include <memory>
#include <string>

#include "handler/IDataReceiver.hpp"
#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{
namespace socket
{
class WebSocket : public handler::IDataReceiver
{
public:
    WebSocket(const std::string& uri, u16 port, handler::ReaderCallback handler = handler::defaultReader);
    ~WebSocket();

    void start();
    void stop();

    void setHandler(handler::ReaderCallback handler) override;

private:
    class WebSocketWrapper;
    std::unique_ptr<WebSocketWrapper> webSocketWrapper_;
};

} // namespace net
} // namespace hal
} // namespace socket

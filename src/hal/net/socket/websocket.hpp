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
    ~WebSocket();

    void start();
    void stop();

    void setHandler(dispatcher::ReaderCallback handler) override;

private:
    class WebSocketWrapper;
    std::unique_ptr<WebSocketWrapper> webSocketWrapper_;
};

} // namespace net
} // namespace hal
} // namespace socket

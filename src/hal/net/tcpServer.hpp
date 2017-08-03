#pragma once

#include <functional>
#include <memory>

#include "utils/types.hpp"

namespace hal
{
namespace net
{

class TcpServer final
{
public:
    using HandlerCallback = std::function<void(void* data)>;
    TcpServer(u16 port);
    ~TcpServer();
    void start();
    void stop();
    void setHandler(HandlerCallback handler);

private:
    class TcpServerImpl;
    std::unique_ptr<TcpServerImpl> tcpServerImpl_;
    HandlerCallback handler_;
};

} // namespace net
} // namespace hal

#pragma once

#include <functional>
#include <memory>

#include "hal/net/tcpHandler.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{

class TcpServer final
{
public:
    TcpServer(u16 port, TcpReadCallback readerCallback = defaultReader);
    ~TcpServer();
    void start();
    void stop();

private:
    class TcpServerImpl;
    std::unique_ptr<TcpServerImpl> tcpServerImpl_;
};

} // namespace net
} // namespace hal

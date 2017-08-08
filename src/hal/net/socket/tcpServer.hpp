#pragma once

#include <functional>
#include <memory>

#include "handler/IDataReceiver.hpp"
#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{
namespace socket
{
class TcpServer : public handler::IDataReceiver
{
public:
    TcpServer(u16 port, handler::ReaderCallback readerCallback = handler::defaultReader);
    ~TcpServer();
    void start();
    void stop();

    void setHandler(handler::ReaderCallback reader) override;

private:
    class TcpServerImpl;
    std::unique_ptr<TcpServerImpl> tcpServerImpl_;
};

} // namespace net
} // namespace hal
} // namespace socket

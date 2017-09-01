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

    void write(const std::string& data) override
    {
    }
    void write(u8 byte) override
    {
    }
    void write(const u8* data, std::size_t len) override
    {
    }

private:
    class TcpServerImpl;
    std::unique_ptr<TcpServerImpl> tcpServerImpl_;
};

} // namespace net
} // namespace hal
} // namespace socket

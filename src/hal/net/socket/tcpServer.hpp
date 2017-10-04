#pragma once

#include <functional>
#include <memory>

#include "dispatcher/IDataReceiver.hpp"
#include "dispatcher/handlers.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{
namespace socket
{
class TcpServer : public dispatcher::IDataReceiver
{
public:
    TcpServer(u16 port, dispatcher::ReaderCallback readerCallback = dispatcher::defaultReader);
    ~TcpServer();
    TcpServer(const TcpServer&) = delete;
    TcpServer(const TcpServer&&) = delete;
    TcpServer& operator=(const TcpServer&&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    void start();
    void stop();

    void setHandler(dispatcher::ReaderCallback reader) override;

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

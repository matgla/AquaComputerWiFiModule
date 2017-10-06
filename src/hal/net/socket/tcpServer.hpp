#pragma once

#include <functional>
#include <memory>

#include <boost/core/ignore_unused.hpp>

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
    ~TcpServer() override;
    TcpServer(const TcpServer&) = delete;
    TcpServer(const TcpServer&&) = delete;
    TcpServer& operator=(const TcpServer&&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    void start();
    void stop();

    void setHandler(const dispatcher::ReaderCallback& reader) override;

    void write(const std::string& data) override
    {
        boost::ignore_unused(data);
    }
    void write(u8 byte) override
    {
        boost::ignore_unused(byte);
    }
    void write(const u8* data, std::size_t len) override
    {
        boost::ignore_unused(data);
        boost::ignore_unused(len);
    }

private:
    class TcpServerImpl;
    std::unique_ptr<TcpServerImpl> tcpServerImpl_;
};

} // namespace net
} // namespace hal
} // namespace socket

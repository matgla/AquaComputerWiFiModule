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
class TcpClient : public dispatcher::IDataReceiver
{
public:
    ~TcpClient();
    TcpClient(const std::string& url, u16 port,
              dispatcher::ReaderCallback readerCallback = dispatcher::defaultReader);
    TcpClient(const TcpClient&) = delete;
    TcpClient(const TcpClient&&) = delete;
    TcpClient& operator=(const TcpClient&&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    void start();
    void stop();

    void write(const std::string& data);
    void write(const u8* buf, std::size_t length);
    void write(u8 byte);

    void setHandler(dispatcher::ReaderCallback reader) override;

private:
    class TcpClientImpl;
    std::unique_ptr<TcpClientImpl> tcpClientImpl_;
};

} // namespace net
} // namespace hal
} // namespace socket

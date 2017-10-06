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
    ~TcpClient() override;
    TcpClient(const std::string& url, u16 port,
              const dispatcher::ReaderCallback& readerCallback = dispatcher::defaultReader);
    TcpClient(const TcpClient&) = delete;
    TcpClient(const TcpClient&&) = delete;
    TcpClient& operator=(const TcpClient&&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    void start();
    void stop();

    void write(const std::string& data) override;
    void write(const u8* buf, std::size_t length) override;
    void write(u8 byte) override;

    void setHandler(const dispatcher::ReaderCallback& reader) override;

private:
    class TcpClientImpl;
    std::unique_ptr<TcpClientImpl> tcpClientImpl_;
};

} // namespace net
} // namespace hal
} // namespace socket

#pragma once

#include <functional>
#include <memory>
#include <string>

#include "dispatcher/IDataReceiver.hpp"
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
              const ReaderCallback& readerCallback = defaultReader);
    TcpClient(const TcpClient&) = delete;
    TcpClient(const TcpClient&&) = delete;
    TcpClient& operator=(const TcpClient&&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    void start();
    void stop();

    void write(const std::string& data) override;
    void write(const BufferSpan& buffer) override;
    void write(u8 byte) override;

    void setHandler(const ReaderCallback& reader) override;

private:
    class TcpClientImpl;
    std::unique_ptr<TcpClientImpl> tcpClientImpl_;
};

} // namespace net
} // namespace hal
} // namespace socket

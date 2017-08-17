#pragma once

#include <functional>
#include <memory>
#include <string>

#include "handler/IDataReceiver.hpp"
#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{
namespace socket
{
class TcpClient : public handler::IDataReceiver
{
public:
    ~TcpClient();
    TcpClient(const std::string& url, u16 port, handler::ReaderCallback readerCallback = handler::defaultReader);

    void start();
    void stop();

    void write(const std::string& data);
    void write(const u8* buf, std::size_t length);
    void write(u8 byte);

    void setHandler(handler::ReaderCallback reader) override;

private:
    class TcpClientImpl;
    std::unique_ptr<TcpClientImpl> tcpClientImpl_;
};

} // namespace net
} // namespace hal
} // namespace socket

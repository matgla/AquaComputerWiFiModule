#pragma once

#include <functional>
#include <memory>
#include <string>

#include "utils/types.hpp"

namespace hal
{
namespace net
{

class TcpClient
{
public:
    ~TcpClient();
    TcpClient(const std::string& url, u16 port);

    void start();
    void stop();

    void write(const std::string& data);
    void write(const u8* buf, std::size_t length);
    void write(u8 byte);

private:
    class TcpClientImpl;
    std::unique_ptr<TcpClientImpl> tcpClientImpl_;
};

} // namespace net
} // namespace hal

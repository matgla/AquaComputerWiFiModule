#pragma once

#include <functional>
#include <memory>
#include <string>

#include "utils/types.hpp"

namespace net
{

class TcpSocket
{
public:
    using HandlerCallback = std::function<void(void* data)>;
    ~TcpSocket();
    TcpSocket(u16 port);
    void start();
    void connect(const std::string& name);
    void write(const std::string& data);
    void setHandler(HandlerCallback handler);
    void close();

private:
    class SocketWrapper;
    std::unique_ptr<SocketWrapper> socketWrapper_;
    HandlerCallback handler_;
    u8 port_;
};

}  // namespace net

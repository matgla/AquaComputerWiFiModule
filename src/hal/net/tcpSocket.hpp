#pragma once

#include <memory>
#include <string>

#include "utils/types.hpp"

namespace net
{
class TcpSocket
{
public:
    ~TcpSocket();
    TcpSocket();
    void connect(const std::string& name, u16 port);
    void write(const std::string& data);
    void close();
private:
    class SocketWrapper;
    std::unique_ptr<SocketWrapper> socketWrapper_;
};
}

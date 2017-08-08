#pragma once

#include <iostream>
#include <ostream>
#include <streambuf>
#include <string>

#include "hal/net/socket/tcpClient.hpp"

namespace stream
{

class SocketBuffer : public std::streambuf
{
public:
    SocketBuffer(const std::string& host, u16 port);
    virtual std::streambuf::int_type overflow(std::streambuf::int_type c) override;

private:
    hal::net::socket::TcpClient socket_;
};

} // namespace stream
#pragma once

#include <ostream>
#include <string>
#include <streambuf>
#include <iostream>

#include "hal/net/tcpSocket.hpp"

namespace stream
{

class SocketBuffer : public std::streambuf
{
public:
    SocketBuffer(const std::string& host, u16 port);
    virtual std::streambuf::int_type overflow(std::streambuf::int_type c) override;

private:
    net::TcpSocket socket_;
};

}  // namespace stream
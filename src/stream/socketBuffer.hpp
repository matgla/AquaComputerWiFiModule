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
    SocketBuffer(const std::string& host, u16 port)
    {
        socket_.connect(host, port);
    }

    virtual std::streambuf::int_type overflow(std::streambuf::int_type c) override
    {
        socket_.write(std::string(1, static_cast<char>(c)));
    }

private:
    net::TcpSocket socket_;
};

}  // namespace stream
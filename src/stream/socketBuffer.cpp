#include "socketBuffer.hpp"

namespace stream
{

SocketBuffer::SocketBuffer(const std::string& host, u16 port)
    : socket_(host, port)
{
}

std::streambuf::int_type SocketBuffer::overflow(std::streambuf::int_type c)
{
    socket_.write(static_cast<char>(c));
    return c;
}

} // namespace stream
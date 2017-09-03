#include "socketStream.hpp"

namespace stream
{

SocketStream::SocketStream(const std::string& host, u16 port)
    : std::ostream(&buffer_), buffer_(host, port)
{
}

} // namespace stream

#include "socketStream.hpp"

namespace stream
{

SocketStream::SocketStream(const std::string& host, u16 port) :
    buffer_(host, port),
    std::ostream(&buffer_)
{}

}  // namespace stream
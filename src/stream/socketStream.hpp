#pragma once

#include <ostream>
#include <string>

#include "stream/socketBuffer.hpp"

namespace stream
{

class SocketStream : public std::ostream
{
public:
    SocketStream(const std::string& host, u16 port);

private:
    SocketBuffer buffer_;
};

}  // namespace stream
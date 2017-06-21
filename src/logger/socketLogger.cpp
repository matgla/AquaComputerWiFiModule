#include "socketLogger.hpp"

#include "stream/SocketStream.hpp"

namespace logger
{

SocketLogger::SocketLogger(const std::string& host, u16 port)
{
    stream_ = std::make_shared<stream::SocketStream>(host, port);
}

}  // namespace logger
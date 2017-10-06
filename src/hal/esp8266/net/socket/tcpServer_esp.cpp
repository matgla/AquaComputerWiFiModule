#include "hal/net/socket/tcpServer.hpp"

namespace hal
{
namespace net
{
namespace socket
{

class TcpServer::TcpServerImpl
{
};

TcpServer::TcpServer(u16 port, handler::ReaderCallback readerCallback)
{
}

TcpServer::~TcpServer() = default;

void TcpServer::setHandler(const handler::ReaderCallback& reader)
{
}
} // namespace socket
} // namespace net
} // namespace hal

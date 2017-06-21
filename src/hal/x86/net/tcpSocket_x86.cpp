#include "hal/net/tcpSocket.hpp"

#include <string>

#include <boost/asio.hpp>

namespace net
{
using boost::asio::ip::tcp;

class TcpSocket::SocketWrapper
{
public:
    SocketWrapper() :
        socket_(io_service_),
        resolver_(io_service_)
    {

    }
    boost::asio::io_service io_service_;
    tcp::socket socket_;
    tcp::resolver resolver_;
};

TcpSocket::TcpSocket() : socketWrapper_(new SocketWrapper())
{}

TcpSocket::~TcpSocket() = default;

void TcpSocket::connect(const std::string& host, u16 port)
{
    boost::asio::connect(socketWrapper_->socket_, socketWrapper_->resolver_.resolve({host, std::to_string(port)}));
}

void TcpSocket::write(const std::string& data)
{
    boost::asio::write(socketWrapper_->socket_, boost::asio::buffer(data));
}

}  // namespace net
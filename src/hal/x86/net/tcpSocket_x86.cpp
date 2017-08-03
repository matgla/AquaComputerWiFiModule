#include "hal/net/tcpSocket.hpp"

#include <memory>
#include <string>
#include <thread>

#include <boost/asio.hpp>

#include "container/buffer.hpp"
#include "logger/logger.hpp"
#include "utils/types.hpp"

using namespace boost::asio::ip;

namespace net
{


void handle(std::shared_ptr<tcp::socket> socket_)
{
    logger::Logger logger_("SocketHandler");
    while (true)
    {
        const size_t bufferSize = 1024;
        u8 buffer[bufferSize];
        boost::system::error_code ec;
        size_t length = socket_->read_some(boost::asio::buffer(buffer), ec);
        if (ec == boost::asio::error::eof)
        {
            logger_.info() << "Connection closed by peer.";
            break;
        }
        else if (ec)
        {
            logger_.err() << "Error: " << ec.message();
        }
        buffer[length + 1] = 0;
        logger_.info() << "got data: " << (char*)(buffer);
    }
    socket_->close();
}

class TcpSocket::SocketWrapper
{
public:
    SocketWrapper()
        : socket_(io_service_),
          resolver_(io_service_)
    {
    }
    boost::asio::io_service io_service_;
    tcp::socket socket_;
    tcp::resolver resolver_;
};

TcpSocket::TcpSocket(u16 port)
    : socketWrapper_(new SocketWrapper()), port_(port)
{
}

TcpSocket::~TcpSocket() = default;

void TcpSocket::start()
{
    std::thread{[this]() {
        while (true)
        {
            tcp::acceptor acceptor(socketWrapper_->io_service_, tcp::endpoint(tcp::v4(), port_));
            std::shared_ptr<tcp::socket> socket(new tcp::socket(socketWrapper_->io_service_));
            acceptor.accept(*socket);
            std::thread{std::bind(handle, socket)}.detach();
        }
    }}.detach();
}

void TcpSocket::connect(const std::string& host)
{
    boost::asio::connect(socketWrapper_->socket_, socketWrapper_->resolver_.resolve({host, std::to_string(port_)}));
}

void TcpSocket::write(const std::string& data)
{
    boost::asio::write(socketWrapper_->socket_, boost::asio::buffer(data));
}

void TcpSocket::setHandler(HandlerCallback handler)
{
    handler_ = handler;
}

void TcpSocket::close()
{
    socketWrapper_->socket_.close();
}

} // namespace net
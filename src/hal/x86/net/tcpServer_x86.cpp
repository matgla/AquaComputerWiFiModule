#include "hal/net/tcpServer.hpp"

#include <functional>
#include <memory>
#include <thread>
#include <utility>

#include <boost/asio.hpp>

#include "logger/logger.hpp"

using namespace boost::asio;
using boost::asio::ip::tcp;

namespace hal
{
namespace net
{

const std::size_t BUF_SIZE = 1024;


class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket)
        : socket_(std::move(socket)),
          logger_("Session")
    {
        logger_.debug() << "Session started";
    }

    ~Session()
    {
        logger_.debug() << "Session killed";
    }

    void start()
    {
        doRead();
    }

    tcp::socket& getSocket()
    {
        return socket_;
    }

private:
    void doRead()
    {
        auto self(shared_from_this());

        socket_.async_read_some(buffer(buffer_, BUF_SIZE),
                                [this, self](boost::system::error_code error, std::size_t tranferred_bytes) {
                                    if (error == boost::asio::error::eof)
                                    {
                                        logger_.debug() << "Client from ["
                                                        << socket_.remote_endpoint().address().to_string()
                                                        << "] disconnected";
                                        return;
                                    }

                                    if (!error)
                                    {
                                        buffer_[tranferred_bytes + 1] = 0;
                                        logger_.info() << buffer_;
                                        doRead();
                                    }
                                    else
                                    {
                                        logger_.err() << "Reading failed: " << error.message();
                                    }
                                });
    }

    u8 buffer_[BUF_SIZE];
    tcp::socket socket_;
    logger::Logger logger_;
};

class TcpServer::TcpServerImpl
{
public:
    TcpServerImpl(u16 port)
        : logger_("TcpServerImpl"),
          acceptor_(ioService_, tcp::endpoint(tcp::v4(), port)),
          socket_(ioService_)
    {
    }

    void start()
    {
        doAccept();

        thread_ = std::thread{[this]() {
            ioService_.run();
        }};
    }

    void stop()
    {
        ioService_.stop();
        thread_.join();
    }

private:
    void doAccept()
    {
        logger_.info() << "Starting session";

        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code error) {
                                   if (!error)
                                   {
                                       std::make_shared<Session>(std::move(socket_))->start();
                                   }

                                   doAccept();
                               });
    }

    logger::Logger logger_;
    io_service ioService_;
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    std::thread thread_;
};


TcpServer::TcpServer(u16 port)
    : tcpServerImpl_(new TcpServerImpl(port))
{
}

TcpServer::~TcpServer() = default;

void TcpServer::start()
{
    tcpServerImpl_->start();
}

void TcpServer::stop()
{
    tcpServerImpl_->stop();
}

void TcpServer::setHandler(HandlerCallback handler)
{
    handler_ = handler;
}


} // namespace net
} // namespace hal

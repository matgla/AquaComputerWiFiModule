#include "hal/net/tcpServer.hpp"

#include <functional>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include "hal/x86/net/tcpSession.hpp"
#include "logger/logger.hpp"

using namespace boost::asio;
using boost::asio::ip::tcp;

namespace hal
{
namespace net
{
class TcpServer::TcpServerImpl
{
public:
    TcpServerImpl(u16 port, TcpReadCallback readerCallback)
        : logger_("TcpServerImpl"),
          socket_(ioService_),
          acceptor_(ioService_, tcp::endpoint(tcp::v4(), port)),
          readerCallback_(readerCallback)
    {
    }

    ~TcpServerImpl()
    {
        stop();
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
        sessions_.clear();

        ioService_.stop();
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

private:
    void doAccept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code error) {
                                   if (!error)
                                   {
                                       sessions_.push_back(std::make_unique<TcpSession>(std::move(socket_), readerCallback_));
                                       sessions_.back()->start();
                                   }

                                   doAccept();
                               });
    }

    logger::Logger logger_;
    io_service ioService_;
    tcp::socket socket_;
    tcp::acceptor acceptor_;
    std::vector<std::unique_ptr<TcpSession>> sessions_;
    std::thread thread_;
    TcpReadCallback readerCallback_;
};


TcpServer::TcpServer(u16 port, TcpReadCallback readerCallback)
    : tcpServerImpl_(new TcpServerImpl(port, readerCallback))
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

} // namespace net
} // namespace hal

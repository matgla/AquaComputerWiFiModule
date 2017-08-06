#include "hal/net/tcpClient.hpp"

#include <array>
#include <functional>
#include <memory>
#include <thread>
#include <utility>

#include <boost/asio.hpp>

#include "hal/x86/net/tcpSession.hpp"
#include "logger/logger.hpp"

using namespace boost::asio;
using boost::asio::ip::tcp;

namespace hal
{
namespace net
{

class TcpClient::TcpClientImpl
{
public:
    TcpClientImpl(const std::string& url, u16 port, TcpReadCallback readerCallback)
        : url_(url),
          port_(port),
          logger_("TcpClientImpl"),
          resolver_(ioService_),
          socket_(ioService_),
          readerCallback_(readerCallback)
    {
    }

    ~TcpClientImpl()
    {
        stop();
    }

    void start()
    {
        connect();

        thread_ = std::thread{[this]() {
            ioService_.run();
        }};
    }


    void write(const std::string& data)
    {
        session_->doWrite(data);
    }

    void write(const u8* buf, std::size_t length)
    {
        session_->doWrite(buf, length);
    }

    void write(u8 byte)
    {
        session_->doWrite(byte);
    }

    bool connected()
    {
        return session_ ? session_->connected() : false;
    }

private:
    void stop()
    {
        if (session_)
        {
            session_.release();
        }
        ioService_.stop();
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

    void connect()
    {
        boost::system::error_code error;
        boost::asio::connect(socket_, resolver_.resolve({url_, std::to_string(port_)}), error);

        if (!error)
        {
            logger_.info() << "Connected to " << url_ << ":" << port_;
            session_ = std::make_unique<TcpSession>(std::move(socket_), readerCallback_);
            session_->start();
        }
        else
        {
            handleConnectionLost();
        }
    }

    void handleConnectionLost()
    {
        logger_.err() << "Couldn't connect to " << url_ << ":" << port_;
        stop();
    }

    std::unique_ptr<TcpSession> session_;
    std::string url_;
    u16 port_;
    logger::Logger logger_;
    io_service ioService_;
    tcp::resolver resolver_;
    tcp::socket socket_;
    std::thread thread_;
    TcpReadCallback readerCallback_;
};


TcpClient::TcpClient(const std::string& url, u16 port, TcpReadCallback readerCallback)
    : tcpClientImpl_(new TcpClientImpl(url, port, readerCallback))
{
}

TcpClient::~TcpClient() = default;

void TcpClient::start()
{
    tcpClientImpl_->start();
}

void TcpClient::stop()
{
    tcpClientImpl_.release();
}

void TcpClient::write(const std::string& data)
{
    if (tcpClientImpl_->connected())
    {
        tcpClientImpl_->write(data);
    }
}

void TcpClient::write(const u8* buf, std::size_t length)
{
    if (tcpClientImpl_->connected())
    {
        tcpClientImpl_->write(buf, length);
    }
}

void TcpClient::write(u8 byte)
{
    if (tcpClientImpl_->connected())
    {
        tcpClientImpl_->write(byte);
    }
}

} // namespace net
} // namespace hal

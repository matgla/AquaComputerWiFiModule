#include "hal/net/socket/tcpClient.hpp"

#include <array>
#include <functional>
#include <memory>
#include <thread>
#include <utility>

#include <boost/asio.hpp>
#include <gsl/span>

#include "hal/x86/net/socket/tcpSession.hpp"
#include "logger/logger.hpp"

using namespace boost::asio;
using boost::asio::ip::tcp;

namespace hal
{
namespace net
{
namespace socket
{
class TcpClient::TcpClientImpl
{
public:
    TcpClientImpl(std::string url, u16 port, dispatcher::ReaderCallback readerCallback)
        : url_(std::move(url)), port_(port), logger_("TcpClientImpl"), resolver_(ioService_),
          socket_(ioService_), readerCallback_(std::move(readerCallback))
    {
    }

    ~TcpClientImpl()
    {
        stop();
    }

    TcpClientImpl(const TcpClientImpl&) = delete;
    TcpClientImpl(const TcpClientImpl&&) = delete;
    TcpClientImpl& operator=(const TcpClientImpl&& other) = delete;
    TcpClientImpl& operator=(const TcpClientImpl& other) = delete;

    void start()
    {
        connect();

        thread_ = std::thread{[this]() { ioService_.run(); }};
    }


    void write(const std::string& data)
    {
        session_->doWrite(data);
    }

    void write(const u8* buf, std::size_t length)
    {
        session_->doWrite(gsl::span<const u8>{
            buf, static_cast<gsl::span<const unsigned char>::index_type>(length)});
    }

    void write(u8 byte)
    {
        session_->doWrite(byte);
    }

    bool connected()
    {
        return session_ ? session_->connected() : false;
    }

    void setHandler(const dispatcher::ReaderCallback& reader)
    {
        readerCallback_ = reader;
        if (session_)
        {
            session_->setHandler(reader);
        }
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
        logger_.error() << "Couldn't connect to " << url_ << ":" << port_;
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
    dispatcher::ReaderCallback readerCallback_;
};


TcpClient::TcpClient(const std::string& url, u16 port,
                     const dispatcher::ReaderCallback& readerCallback)
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

void TcpClient::setHandler(const dispatcher::ReaderCallback& reader)
{
    tcpClientImpl_->setHandler(reader);
}

} // namespace net
} // namespace hal
} // namespace socket

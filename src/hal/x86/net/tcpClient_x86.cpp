#include "hal/net/tcpClient.hpp"

#include <array>
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

// TODO common this for client and server
  
class ClientSession
{
public:
    ClientSession(tcp::socket socket)
        : socket_(std::move(socket)),
          logger_("ClientSession")
    {
        logger_.info() << "CS created";
    }

    ~ClientSession()
    {
        disconnect();
        logger_.info() << "CS destroyed";
    }

    void start()
    {
        logger_.debug() << "ClientSession started";

        doRead();
    }

    tcp::socket& getSocket()
    {
        return socket_;
    }

    void doWrite(const std::string& data)
    {
        async_write(socket_, buffer(data),
                    [this](boost::system::error_code error, std::size_t size) {
                        if (error)
                        {
                            logger_.err() << "Write failed: " << error.message();
                        }
                    });
    }

    void doWrite(const u8* buf, std::size_t length)
    {
        async_write(socket_, buffer(buf, length),
                    [this](boost::system::error_code error, std::size_t size) {
                        if (error)
                        {
                            logger_.err() << "Write failed: " << error.message();
                        }
                    });
    }

    void doWrite(u8 byte)
    {
        std::array<u8, 1> buf = {byte};
        async_write(socket_, buffer(buf, 1),
                    [this](boost::system::error_code error, std::size_t size) {
                        if (error)
                        {
                            logger_.err() << "Write failed: " << error.message();
                        }
                    });
    }

    void disconnect()
    {
        if (socket_.is_open())
        {
            boost::system::error_code ec;
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            socket_.cancel();
            socket_.close();
        }
    }

    bool connected()
    {
        return socket_.is_open();
    }

private:
    void doRead()
    {
        socket_.async_read_some(buffer(buffer_, BUF_SIZE),
                                [this](boost::system::error_code error, std::size_t tranferred_bytes) {
                                    if (error == boost::asio::error::eof)
                                    {
                                        logger_.debug() << "Connection lost to "
                                                        << socket_.remote_endpoint().address().to_string();
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

class TcpClient::TcpClientImpl
{
public:
    TcpClientImpl(const std::string& url, u16 port)
        : url_(url),
          port_(port),
          logger_("TcpClientImpl"),
          resolver_(ioService_),
          socket_(ioService_)
    {
        logger_.info() << "created";
    }

    ~TcpClientImpl()
    {
        stop();
        logger_.info() << "destroyed";
    }

    void start()
    {
        logger_.info() << "Start";
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
        logger_.info() << "Client successfuly killed";
    }

    void connect()
    {
        boost::system::error_code error;
        boost::asio::connect(socket_, resolver_.resolve({url_, std::to_string(port_)}), error);

        if (!error)
        {
            logger_.info() << "Connected to " << url_ << ":" << port_;
            session_ = std::make_unique<ClientSession>(std::move(socket_));
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

    std::unique_ptr<ClientSession> session_;
    std::string url_;
    u16 port_;
    logger::Logger logger_;
    io_service ioService_;
    tcp::resolver resolver_;
    tcp::socket socket_;
    std::thread thread_;
};


TcpClient::TcpClient(const std::string& url, u16 port)
    : tcpClientImpl_(new TcpClientImpl(url, port))
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

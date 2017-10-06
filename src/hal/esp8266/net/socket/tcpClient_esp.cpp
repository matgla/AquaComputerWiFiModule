#include "hal/net/socket/tcpClient.hpp"

#include <memory>
#include <string>

#include <ESPAsyncTCP.h>

#include "hal/time/sleep.hpp"
#include "logger/logger.hpp"

namespace hal
{
namespace net
{
namespace socket
{

class TcpClient::TcpClientImpl
{
public:
    TcpClientImpl(const std::string& url, u16 port, handler::ReaderCallback readerCallback)
        : url_(url), port_(port), logger_("TcpClientImpl"), readerCallback_(readerCallback)
    {
    }

    ~TcpClientImpl()
    {
        stop();
    }

    void start()
    {
        connect();
        client_.onData([this](void*, AsyncClient* client, void* data, std::size_t length) {
            readerCallback_(reinterpret_cast<const u8*>(data), length,
                            [this, client](const u8* buf, std::size_t len) {
                                client->write(reinterpret_cast<const char*>(buf), len);
                            });
        });
    }


    void write(const std::string& data)
    {
        client_.write(data.c_str(), data.length());
    }

    void write(const u8* buf, std::size_t length)
    {
        client_.write(reinterpret_cast<const char*>(buf), length);
    }

    void write(u8 byte)
    {
        char data[] = {static_cast<char>(byte)};
        client_.write(data, 1);
    }

    bool connected()
    {
        return client_.connected();
    }

    void setHandler(const handler::ReaderCallback& reader)
    {
        readerCallback_ = reader;
    }

private:
    void stop()
    {
        client_.close(true);
    }

    void connect()
    {
        client_.connect(url_.c_str(), port_);
        while (client_.connected() && !client_.disconnecting())
        {
            hal::time::sleep(1);
        }
    }

    void handleConnectionLost()
    {
        logger_.error() << "Couldn't connect to " << url_ << ":" << port_;
        stop();
    }

    std::string url_;
    u16 port_;
    logger::Logger logger_;
    AsyncClient client_;
    handler::ReaderCallback readerCallback_;
};


TcpClient::TcpClient(const std::string& url, u16 port, handler::ReaderCallback readerCallback)
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

void TcpClient::setHandler(const handler::ReaderCallback& reader)
{
    tcpClientImpl_->setHandler(reader);
}

} // namespace net
} // namespace hal
} // namespace socket

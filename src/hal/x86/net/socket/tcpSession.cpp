#include "hal/x86/net/socket/tcpSession.hpp"

#include <cstring>
#include <functional>

#include <gsl/span>

using namespace boost::asio;
using boost::asio::ip::tcp;

namespace hal
{
namespace net
{
namespace socket
{
TcpSession::TcpSession(tcp::socket socket, dispatcher::ReaderCallback reader)
    : buffer_{}, socket_(std::move(socket)), logger_("TcpSession"),
      readerCallback_(std::move(reader))
{
}

TcpSession::~TcpSession()
{
    disconnect();
}

void TcpSession::start()
{
    doRead();
}

tcp::socket& TcpSession::getSocket()
{
    return socket_;
}

void TcpSession::doWrite(const std::string& data)
{
    async_write(socket_, buffer(data), [this](boost::system::error_code error, std::size_t size) {
        if (nullptr != error)
        {
            logger_.error() << "Write failed: " << error.message();
        }
    });
}

void TcpSession::doWrite(const gsl::span<const u8>& buf)
{
    std::vector<u8> dataBuffer;
    dataBuffer.resize(buf.length());
    std::memcpy(&dataBuffer[dataBuffer.size() - buf.length()], buf.data(), buf.length());
    async_write(socket_, buffer(dataBuffer, buf.length()),
                [this](boost::system::error_code error, std::size_t size) {
                    if (nullptr != error)
                    {
                        logger_.error() << "Write failed: " << error.message();
                    }
                });
}

void TcpSession::doWrite(u8 byte)
{
    std::array<u8, 1> buf = {byte};
    async_write(socket_, buffer(buf, 1), [this](boost::system::error_code error, std::size_t size) {
        if (nullptr != error)
        {
            logger_.error() << "Write failed: " << error.message();
        }
    });
}

void TcpSession::disconnect()
{
    if (socket_.is_open())
    {
        boost::system::error_code ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        socket_.cancel();
        socket_.close();
    }
}

bool TcpSession::connected()
{
    return socket_.is_open();
}

void TcpSession::doRead()
{
    socket_.async_read_some(buffer(buffer_, BUF_SIZE), [this](boost::system::error_code error,
                                                              std::size_t tranferred_bytes) {
        if (error == boost::asio::error::eof)
        {
            logger_.debug() << "Connection lost to "
                            << socket_.remote_endpoint().address().to_string();
            return;
        }

        if (!error)
        {
            std::lock_guard<std::mutex> safeCallback(readerCallbackMutex_);
            readerCallback_(buffer_, // NOLINT TODO: stadnik change reader callback to gsl::span
                            tranferred_bytes, [this](const u8* buf, std::size_t len) {
                                doWrite(gsl::span<const u8>{
                                    buf, static_cast<gsl::span<const u8>::index_type>(len)});
                            });
            return doRead();
        }

        logger_.error() << "Reading failed: " << error.message();

    });
}

void TcpSession::setHandler(const dispatcher::ReaderCallback& reader)
{
    std::lock_guard<std::mutex> safeCallback(readerCallbackMutex_);
    readerCallback_ = reader;
}

} // namespace net
} // namespace hal
} // namespace socket

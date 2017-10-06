#pragma once

#include <memory>
#include <mutex>
#include <string>

#include <boost/asio.hpp>
#include <gsl/span>

#include "dispatcher/handlers.hpp"
#include "logger/logger.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{
namespace socket
{
const std::size_t BUF_SIZE = 1024;

class TcpSession
{
public:
    TcpSession(boost::asio::ip::tcp::socket socket,
               dispatcher::ReaderCallback reader = dispatcher::defaultReader);
    ~TcpSession();
    TcpSession(const TcpSession&) = delete;
    TcpSession(const TcpSession&&) = delete;
    TcpSession& operator=(const TcpSession&& other) = delete;
    TcpSession& operator=(const TcpSession& other) = delete;

    void start();
    boost::asio::ip::tcp::socket& getSocket();

    void doWrite(const std::string& data);
    void doWrite(const gsl::span<const u8>& buf);
    void doWrite(u8 byte);

    void disconnect();
    bool connected();
    void setHandler(const dispatcher::ReaderCallback& reader);

private:
    void doRead();

    u8 buffer_[BUF_SIZE];
    boost::asio::ip::tcp::socket socket_;
    logger::Logger logger_;
    dispatcher::ReaderCallback readerCallback_;
    std::mutex readerCallbackMutex_;
};

} // namespace net
} // namespace hal
} // namespace socket

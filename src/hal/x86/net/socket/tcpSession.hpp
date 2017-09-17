#pragma once

#include <memory>
#include <mutex>
#include <string>

#include <boost/asio.hpp>

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

    void start();
    boost::asio::ip::tcp::socket& getSocket();

    void doWrite(std::string data);
    void doWrite(const u8* buf, std::size_t length);
    void doWrite(u8 byte);

    void disconnect();
    bool connected();
    void setHandler(dispatcher::ReaderCallback reader);

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

#pragma once

#include <memory>
#include <string>

#include <boost/asio.hpp>

#include "hal/net/tcpHandler.hpp"
#include "logger/logger.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace net
{

const std::size_t BUF_SIZE = 1024;

class TcpSession
{
public:
    TcpSession(boost::asio::ip::tcp::socket socket, TcpReadCallback reader = defaultReader);
    ~TcpSession();

    void start();
    boost::asio::ip::tcp::socket& getSocket();

    void doWrite(std::string data);
    void doWrite(const u8* buf, std::size_t length);
    void doWrite(u8 byte);

    void disconnect();
    bool connected();

private:
    void doRead();

    u8 buffer_[BUF_SIZE];
    boost::asio::ip::tcp::socket socket_;
    logger::Logger logger_;
    TcpReadCallback readerCallback_;
};

} // namespace net
} // namespace hal
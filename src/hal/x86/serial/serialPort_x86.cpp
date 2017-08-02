#include "hal/serial/serialPort.hpp"

#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>

#include "container/buffer.hpp"

using namespace boost;
using namespace boost::asio;

namespace serial
{

class SerialPort::SerialWrapper
{
public:
    SerialWrapper(const std::string& port, int baudrate);
    ~SerialWrapper();
    io_service ioService_;
    serial_port serialPort_;
    std::string port_;
    int baudrate_;
    u8 rawBuffer_[1024];
    container::Buffer<2048> buffer_;

private:
    void loop();
    void readCallback(const boost::system::error_code& error,
                      std::size_t bytes_transferred);
};

SerialPort::SerialWrapper::SerialWrapper(const std::string& port, int baudrate)
    : serialPort_(ioService_),
      port_(port),
      baudrate_(baudrate)
{
    try
    {
        serialPort_.open(port);
        serialPort_.set_option(asio::serial_port_base::baud_rate(baudrate_));
        std::thread{[this]() {while(true) {ioService_.run();} }}.detach();
        loop();
    }
    catch (boost::system::system_error& e)
    {
        throw boost::system::system_error{e};
    }
}

SerialPort::SerialWrapper::~SerialWrapper()
{
    serialPort_.close();
}

void SerialPort::SerialWrapper::loop()
{
    serialPort_.async_read_some(boost::asio::buffer(rawBuffer_),
                                boost::bind(&SerialPort::SerialWrapper::readCallback, this, _1, _2));
}

void SerialPort::SerialWrapper::readCallback(const boost::system::error_code& error,
                                             std::size_t bytes_transferred)
{
    if (error || !bytes_transferred)
    {
        return;
    }
    buffer_.write(rawBuffer_, bytes_transferred);
    loop();
}


SerialPort::SerialPort(const std::string& port, int baudrate)
    : serialWrapper_(new SerialWrapper(port, baudrate))
{
}

SerialPort::~SerialPort() = default;

std::size_t SerialPort::isDataToRecive()
{
    return serialWrapper_->buffer_.size();
}

std::size_t SerialPort::write(const char* data)
{
    return serialWrapper_->serialPort_.write_some(boost::asio::buffer(data, strlen(data)));
}

std::size_t SerialPort::write(const u8* buf, std::size_t length)
{
    return serialWrapper_->serialPort_.write_some(boost::asio::buffer(buf, length));
}

std::size_t SerialPort::write(u8 byte)
{
    return serialWrapper_->serialPort_.write_some(boost::asio::buffer(std::vector<u8>{byte}));
}

std::size_t SerialPort::read(u8* buf, std::size_t length)
{
    return serialWrapper_->buffer_.getData(buf, length);
}

u8 SerialPort::readByte()
{
    return serialWrapper_->buffer_.getByte();
}

} // namespace serial
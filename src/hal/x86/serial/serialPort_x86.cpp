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

namespace hal
{
namespace serial
{

class SerialPort::SerialWrapper
{
public:
    SerialWrapper(const std::string& port, int baudrate);
    ~SerialWrapper();
    SerialWrapper(const SerialWrapper&) = delete;
    SerialWrapper(const SerialWrapper&&) = delete;
    SerialWrapper& operator=(const SerialWrapper&&) = delete;
    SerialWrapper& operator=(const SerialWrapper&) = delete;
    io_service ioService_;
    serial_port serialPort_;
    std::string port_;
    int baudrate_;
    u8 rawBuffer_[1024];
    container::Buffer<2048> buffer_;
    ReaderCallback readerCallback_;

private:
    void loop();
    void readCallback(const boost::system::error_code& error, u32 bytesTransferred);
};

SerialPort::SerialWrapper::SerialWrapper(const std::string& port, const int baudrate)
    : serialPort_(ioService_), port_(port), baudrate_(baudrate), rawBuffer_{}
{
    try
    {
        serialPort_.open(port);
        serialPort_.set_option(asio::serial_port_base::baud_rate(baudrate_));
        std::thread{[this]() {
            while (true)
            {
                ioService_.run();
            }
        }}
            .detach();
        loop();
    }
    catch (boost::system::system_error& e)
    {
        throw std::string(e.what());
    }
}

SerialPort::SerialWrapper::~SerialWrapper()
{
    serialPort_.close();
}

void SerialPort::SerialWrapper::loop()
{
    serialPort_.async_read_some(
        boost::asio::buffer(rawBuffer_),
        boost::bind(&SerialPort::SerialWrapper::readCallback, this, _1, _2));
}

void SerialPort::SerialWrapper::readCallback(const boost::system::error_code& error,
                                             const u32 bytesTransferred)
{
    if (nullptr != error || bytesTransferred != 0u)
    {
        return;
    }
    readerCallback_(BufferSpan{ rawBuffer_, bytesTransferred },
                    [this](const BufferSpan& buffer) {
                        serialPort_.write_some(boost::asio::buffer(buffer.data(), buffer.length()));
                    });
    loop();
}

// TODO: set default reader
SerialPort::SerialPort(const std::string& port, const int baudrate)
    : serialWrapper_(new SerialWrapper(port, baudrate))
{
}

SerialPort::~SerialPort() = default;

std::size_t SerialPort::isDataToRecive()
{
    return serialWrapper_->buffer_.size();
}

void SerialPort::setHandler(const ReaderCallback& readerCallback)
{
    serialWrapper_->readerCallback_ = readerCallback;
}

void SerialPort::write(const std::string& data)
{
    serialWrapper_->serialPort_.write_some(boost::asio::buffer(data));
}

void SerialPort::write(const BufferSpan& buffer)
{
    serialWrapper_->serialPort_.write_some(boost::asio::buffer(buffer.data(), buffer.length()));
}

void SerialPort::write(u8 byte)
{
    serialWrapper_->serialPort_.write_some(boost::asio::buffer(std::vector<u8>{byte}));
}

void SerialPort::process()
{
}

// void SerialPort::read(u8* buf, std::size_t length)
// {
//     // DataBuffer buffer;
//     // buffer.resize(length);
//     // serialWrapper_->buffer_.getData(buf, length);
// }

// u8 SerialPort::readByte()
// {
//     // return serialWrapper_->buffer_.getByte();
// }

} // namespace serial
} // namespace hal

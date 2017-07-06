#include "hal/serial/serialPort.hpp"

#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

using namespace boost;
using namespace boost::asio;

namespace serial
{

class SerialPort::SerialWrapper
{
public:
    SerialWrapper(const std::string& port, int baudrate);
    io_service io_service_;
    serial_port serial_port_;
    std::string port_;
    int baudrate_;
};

SerialPort::SerialWrapper::SerialWrapper(const std::string& port, int baudrate) :
    serial_port_(io_service_),
    port_(port),
    baudrate_(baudrate)
{
    try
    {
        serial_port_.open(port);
        serial_port_.set_option(asio::serial_port_base::baud_rate(baudrate_));
    }
    catch(boost::system::system_error& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

SerialPort::SerialPort(const std::string& port, int baudrate) :
    serialWrapper_(new SerialWrapper(port, baudrate))
{}

SerialPort::~SerialPort() = default;

u32 SerialPort::isDataToRecive()
{
    return 0;
}

size_t SerialPort::write(const char* data)
{
    return serialWrapper_->serial_port_.write_some(boost::asio::buffer(data, strlen(data)));
}

size_t SerialPort::write(const u8* buf, size_t length)
{
    return serialWrapper_->serial_port_.write_some(boost::asio::buffer(buf, length));
}

size_t SerialPort::write(u8 byte)
{
    return serialWrapper_->serial_port_.write_some(boost::asio::buffer(std::vector<u8>{byte}));
}

u32 SerialPort::read(u8* buf, u32 length)
{
    return 0;
}

u8 SerialPort::readByte()
{
    return 0;
}

}  // namespace serial
#include "hal/serial/serialPort.hpp"

#include "ArduinoFixed.hpp"

namespace hal
{
namespace serial
{

class SerialPort::SerialWrapper
{
public:
    SerialWrapper(const std::string& port, int baudrate);
    ~SerialWrapper();
    int baudrate_;
};

SerialPort::SerialWrapper::SerialWrapper(const std::string&, int baudrate) : baudrate_(baudrate)
{
    Serial.begin(baudrate_);
}

SerialPort::SerialWrapper::~SerialWrapper()
{
    Serial.flush();
}


SerialPort::SerialPort(const std::string& port, int baudrate)
    : serialWrapper_(new SerialWrapper(port, baudrate))
{
}

SerialPort::~SerialPort() = default;

std::size_t SerialPort::isDataToRecive()
{
    return Serial.available();
}

std::size_t SerialPort::write(const char* data)
{
    return Serial.write(data);
}

std::size_t SerialPort::write(const u8* buf, std::size_t length)
{
    return Serial.write(buf, length);
}

std::size_t SerialPort::write(u8 byte)
{
    return Serial.write(byte);
}

std::size_t SerialPort::read(u8* buf, std::size_t length)
{
    return Serial.readBytes(buf, length);
}

u8 SerialPort::readByte()
{
    return Serial.read();
}

} // namespace serial
} // namespace hal

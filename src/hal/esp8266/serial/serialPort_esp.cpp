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

void SerialPort::write(const std::string& data)
{
    Serial.write(data.c_str());
}

void SerialPort::write(const u8* buf, std::size_t length)
{
    Serial.write(buf, length);
}

void SerialPort::write(u8 byte)
{
    Serial.write(byte);
}

void SerialPort::setHandler(const handler::ReaderCallback& readerCallback)
{
    readerCallback_ = readerCallback;
}

void SerialPort::process()
{
    DataBuffer buffer;
    buffer.resize(Serial.available());

    Serial.readBytes(buffer.data(), buffer.size());
    readerCallback_(buffer.data(), buffer.size(),
                    [this](const u8* buf, std::size_t len) { write(buf, len); });
}

} // namespace serial
} // namespace hal

#pragma once

#include <memory>
#include <string>

#include "utils/types.hpp"

namespace hal
{
namespace serial
{

class SerialPort
{
public:
    SerialPort(const std::string& port, int baudRate = 115200);
    ~SerialPort();
    std::size_t write(const char* data);
    std::size_t write(const u8* buf, std::size_t length);
    std::size_t write(u8 byte);

    std::size_t isDataToRecive();
    std::size_t read(u8* buf, std::size_t length);
    u8 readByte();

protected:
    class SerialWrapper;
    std::unique_ptr<SerialWrapper> serialWrapper_;
};

} // namespace serial
} // namespace hal

#pragma once

#include <memory>
#include <string>

#include "utils/types.hpp"

namespace serial
{

class SerialPort
{
public:
    SerialPort(const std::string& port, int baudRate = 115200);
    ~SerialPort();
    u32 isDataToRecive();
    size_t write(const char* data);
    size_t write(const u8* buf, size_t length);
    size_t write(u8 byte);

    u32 read(u8* buf, u32 length);
    u8 readByte();

protected:
    class SerialWrapper;
    std::unique_ptr<SerialWrapper> serialWrapper_;
};

}
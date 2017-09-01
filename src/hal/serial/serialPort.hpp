#pragma once

#include <memory>
#include <string>

#include "handler/IDataReceiver.hpp"
#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace serial
{

class SerialPort : public handler::IDataReceiver
{
public:
    SerialPort(const std::string& port, int baudRate = 115200);
    ~SerialPort();
    void write(const std::string& data) override;
    void write(const u8* buf, std::size_t length) override;
    void write(u8 byte) override;

    void setHandler(handler::ReaderCallback readerCallback) override;

    std::size_t isDataToRecive();
    void process();
    // void read(u8* buf, std::size_t length);
    // u8 readByte();

protected:
    class SerialWrapper;
    std::unique_ptr<SerialWrapper> serialWrapper_;

    handler::ReaderCallback readerCallback_;
};

} // namespace serial
} // namespace hal

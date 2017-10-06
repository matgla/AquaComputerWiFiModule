#pragma once

#include <memory>
#include <string>

#include "dispatcher/IDataReceiver.hpp"
#include "dispatcher/handlers.hpp"
#include "utils/types.hpp"

namespace hal
{
namespace serial
{

class SerialPort : public dispatcher::IDataReceiver
{
public:
    explicit SerialPort(const std::string& port, int baudrate = 115200);
    ~SerialPort() override;
    SerialPort(const SerialPort&) = delete;
    SerialPort(const SerialPort&&) = delete;
    SerialPort& operator=(const SerialPort&& other) = delete;
    SerialPort& operator=(const SerialPort& other) = delete;

    void write(const std::string& data) override;
    void write(const u8* buf, std::size_t length) override;
    void write(u8 byte) override;

    void setHandler(const dispatcher::ReaderCallback& readerCallback) override;

    std::size_t isDataToRecive();
    void process();
    // void read(u8* buf, std::size_t length);
    // u8 readByte();

protected:
    class SerialWrapper;
    std::unique_ptr<SerialWrapper> serialWrapper_;

    dispatcher::ReaderCallback readerCallback_;
};

} // namespace serial
} // namespace hal

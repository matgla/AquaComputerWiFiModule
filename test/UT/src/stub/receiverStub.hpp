#pragma once

#include "dispatcher/IDataReceiver.hpp"

#include "logger/logger.hpp"

namespace stub
{
struct ReceiverStub : public dispatcher::IDataReceiver
{
    ReceiverStub() : logger("Receiver stub")
    {
    }
    ReceiverStub(const ReceiverStub&) = delete;
    ReceiverStub(const ReceiverStub&&) = delete;
    ReceiverStub& operator=(const ReceiverStub&&) = delete;
    ReceiverStub& operator=(const ReceiverStub&) = delete;
    ~ReceiverStub() = default;
    void setHandler(const dispatcher::ReaderCallback& callback) override
    {
        u8 test[] = {1, 2, 3};
        logger.info() << "Set reader callback";
        readerCallback = callback;
    }

    void write(const std::string& data) override
    {
        writeBuffer.resize(writeBuffer.size() + data.length());
        memcpy(writeBuffer.data() + writeBuffer.size() - data.length(), data.c_str(),
               data.length());
    }

    void write(const u8* buf, const std::size_t size) override
    {
        writeBuffer.resize(writeBuffer.size() + size);
        memcpy(writeBuffer.data() + writeBuffer.size() - size, buf, size);
    }

    void write(const u8 byte) override
    {
        writeBuffer.push_back(byte);
    }

    void clearBuffers()
    {
        writeBuffer.clear();
    }

    dispatcher::ReaderCallback readerCallback;

    DataBuffer writeBuffer;
    logger::Logger logger;
};
} // namespace stub

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
    void setHandler(const ReaderCallback& callback) override
    {
        readerCallback = callback;
    }

    void write(const std::string& data) override
    {
        writeBuffer.resize(writeBuffer.size() + data.length());
        memcpy(writeBuffer.data() + writeBuffer.size() - data.length(), data.c_str(),
               data.length());
    }

    void write(const BufferSpan& buffer) override
    {
        writeBuffer.resize(writeBuffer.size() + buffer.size());
        memcpy(writeBuffer.data() + writeBuffer.size() - buffer.size(), buffer.data(), buffer.size());
    }

    void write(const u8 byte) override
    {
        writeBuffer.push_back(byte);
    }

    void clearBuffers()
    {
        writeBuffer.clear();
    }

    ReaderCallback readerCallback;

    DataBuffer writeBuffer;
    logger::Logger logger;
};
} // namespace stub

#pragma once

#include "dispatcher/IDataReceiver.hpp"

namespace stub
{
struct ReceiverStub : public dispatcher::IDataReceiver
{
    void setHandler(dispatcher::ReaderCallback readerCallback) override
    {
        readerCallback_ = readerCallback;
    }

    void write(const std::string& data)
    {
        writeBuffer_.resize(writeBuffer_.size() + data.length());
        memcpy(writeBuffer_.data() + writeBuffer_.size() - data.length(), data.c_str(),
               data.length());
    }

    void write(const u8* buf, std::size_t size)
    {
        writeBuffer_.resize(writeBuffer_.size() + size);
        memcpy(writeBuffer_.data() + writeBuffer_.size() - size, buf, size);
    }

    void write(u8 byte)
    {
        writeBuffer_.push_back(byte);
    }

    void clearBuffers()
    {
        writeBuffer_.clear();
    }

    dispatcher::ReaderCallback readerCallback_;

    DataBuffer writeBuffer_;
};
} // namespace stub

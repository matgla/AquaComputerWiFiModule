#pragma once

#include <functional>
#include <string>

#include "dispatcher/handlers.hpp"
#include "utils/types.hpp"

namespace dispatcher
{
class IDataReceiver
{
public:
    using RawDataReceiverPtr = std::shared_ptr<IDataReceiver>;

    virtual ~IDataReceiver() = default;
    IDataReceiver(const IDataReceiver&) = delete;
    IDataReceiver(const IDataReceiver&&) = delete;
    IDataReceiver& operator=(const IDataReceiver&&) = delete;
    IDataReceiver& operator=(const IDataReceiver&) = delete;

    virtual void setHandler(ReaderCallback readerCallback) = 0;

    virtual void write(const std::string& data) = 0;
    virtual void write(const u8* buf, std::size_t length) = 0;
    virtual void write(u8 byte) = 0;
};
} // namespace dispatcher

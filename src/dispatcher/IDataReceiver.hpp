#pragma once

#include <functional>
#include <string>

#include <gsl/span>

#include "utils/types.hpp"

namespace dispatcher
{
class IDataReceiver
{
public:
    using RawDataReceiverPtr = std::shared_ptr<IDataReceiver>;
    IDataReceiver() = default;
    virtual ~IDataReceiver() = default;
    IDataReceiver(const IDataReceiver&) = delete;
    IDataReceiver(const IDataReceiver&&) = delete;
    IDataReceiver& operator=(const IDataReceiver&&) = delete;
    IDataReceiver& operator=(const IDataReceiver&) = delete;

    virtual void setHandler(const ReaderCallback& readerCallback) = 0;

    virtual void write(const std::string& data) = 0;
    virtual void write(const BufferSpan& buffer) = 0;
    virtual void write(u8 byte) = 0;
};
} // namespace dispatcher

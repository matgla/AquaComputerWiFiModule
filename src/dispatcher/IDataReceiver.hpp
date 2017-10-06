#pragma once

#include <functional>
#include <string>

#include <gsl/span>

#include "dispatcher/handlers.hpp"
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
    virtual void write(const u8* buf, std::size_t length) = 0;
    virtual void write(u8 byte) = 0;

    // TODO stadnik : remove this an raw u8 buffer function
    virtual void write(const gsl::span<const u8>& buffer)
    {
        write(buffer.data(), buffer.length());
    }
};
} // namespace dispatcher

#pragma once

#include <string>

#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace handler
{
class IDataReceiver
{
public:
    using RawDataReceiverPtr = std::shared_ptr<IDataReceiver>;

    virtual ~IDataReceiver() = default;

    virtual void setHandler(ReaderCallback readerCallback) = 0;
};
} // namespace handler

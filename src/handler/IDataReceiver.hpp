#pragma once

#include <string>

#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace handler
{
class IDataReceiver
{
public:
    virtual ~IDataReceiver() = default;

    virtual void setHandler(ReaderCallback readerCallback) = 0;
};
} // namespace handler
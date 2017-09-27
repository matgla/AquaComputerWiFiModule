#pragma once

#include <memory>
#include <vector>

#include "utils/types.hpp"

#include "dispatcher/handlers.hpp"

namespace dispatcher
{

class IFrameHandler
{
public:
    using HandlerPtr = std::unique_ptr<IFrameHandler>;

    virtual ~IFrameHandler() = default;
    virtual void onRead(const u8* buffer, std::size_t length, WriterCallback write) = 0;

    virtual void send(const DataBuffer& data) = 0;
};

} // namespace dispatcher
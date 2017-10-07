#pragma once

#include <memory>
#include <vector>

#include "utils/types.hpp"

namespace dispatcher
{

class IFrameHandler
{
public:
    using HandlerPtr = std::unique_ptr<IFrameHandler>;

    IFrameHandler() = default;
    virtual ~IFrameHandler() = default;
    IFrameHandler(const IFrameHandler&) = delete;
    IFrameHandler(const IFrameHandler&&) = delete;
    IFrameHandler& operator=(const IFrameHandler&& other) = delete;
    IFrameHandler& operator=(const IFrameHandler& other) = delete;

    virtual void onRead(const BufferSpan& buffer, WriterCallback write) = 0;

    virtual void send(const DataBuffer& data) = 0;
};

} // namespace dispatcher

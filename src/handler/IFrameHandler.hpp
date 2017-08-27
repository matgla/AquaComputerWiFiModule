#pragma once

#include <memory>
#include <vector>

#include "utils/types.hpp"

namespace handler
{

class IFrameHandler
{
public:
    using HandlerPtr = std::shared_ptr<IFrameHandler>;

    virtual ~IFrameHandler() = default;
    virtual void onRead(std::vector<u8> data) = 0;
};

} // namespace handler

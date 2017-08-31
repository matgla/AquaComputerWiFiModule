#pragma once

#include <memory>

namespace handler
{

class IHandler
{
public:
    using HandlerPtr = std::unique_ptr<IHandler>;

    virtual ~IHandler() = default;
};

} // namespace handler

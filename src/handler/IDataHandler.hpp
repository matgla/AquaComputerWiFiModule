#pragma once

#include <memory>

#include "utils/types.hpp"

namespace handler
{

class IDataHandler
{
public:
    using DataHandlerPtr = std::unique_ptr<IDataHandler>;

    virtual void onRead(DataBuffer buffer) = 0;
};

} // handler

#pragma once

#include "utils/types.hpp"

namespace message
{

class IMessage
{
public:
    u32 id() const = 0;
    std::vector<u8> serialize() const = 0;
};

} // namespace message

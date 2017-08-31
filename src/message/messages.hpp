#pragma once

#include "utils/types.hpp"

namespace message
{

enum TransmissionId : u8
{
    Start = 0x10,
    Ack = 0x20,
    Nack
};

} // namespace message

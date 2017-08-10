#pragma once

#include "utils/types.hpp"

namespace message
{

enum TransmissionMessage : u8
{
    Start = 0x00,
    Ack,
    Nack
};

} // namespace message

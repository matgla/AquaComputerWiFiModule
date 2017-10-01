#pragma once

#include "utils/types.hpp"

namespace protocol
{
namespace messages
{

enum Control : u8
{
    Success = 0x20,
    PortNotConnect,
    CrcChecksumFailed,
    WrongEndByte,
    Transmission
};

} // namespace messages
} // namespace protocol
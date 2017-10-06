#pragma once

#include "utils/types.hpp"

namespace protocol
{
namespace messages
{

enum Control : u8
{
    Success = 0x20,
    PortNotConnect = 0x21,
    CrcChecksumFailed = 0x22,
    WrongEndByte = 0x23,
    Transmission = 0x24
};

} // namespace messages
} // namespace protocol
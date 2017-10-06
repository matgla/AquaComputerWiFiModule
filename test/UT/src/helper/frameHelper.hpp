#pragma once

#include "utils/types.hpp"

namespace helper
{

DataBuffer createFrame(const DataBuffer& data, const u8 port, const u8 frameNumber,
                       const int startIndex);

DataBuffer createHeader(const DataBuffer& data, const u8 port, const u8 messageNumber,
                        const u8 frameNumber);

DataBuffer createAck(const u8 port, const u8 frameNumber);

DataBuffer createCrc32Frame(const u8 port, const u8 frameNumber, const DataBuffer& data);

} // namespace helper
#pragma once

#include "utils/types.hpp"

namespace serializer
{

const u8 BITS_IN_BYTE = 8;

template <typename T>
void serialize(u8* buffer, const T& data)
{
    const u8* byte = reinterpret_cast<const u8*>(&data);
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        buffer[i] = static_cast<u8>(data >> i * BITS_IN_BYTE);
    }
}

} // namespace serializer

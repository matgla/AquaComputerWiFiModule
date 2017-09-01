#pragma once

#include <type_traits>

#include "utils/types.hpp"

namespace serializer
{

const u8 BITS_IN_BYTE = 8;

template <typename T>
void serialize(u8* buffer, const T& data)
{
    static_assert(std::is_arithmetic<T>::value, "Type provided for serialize isn't arithmetic");
    const u8* byte = reinterpret_cast<const u8*>(&data);
    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        buffer[i] = static_cast<u8>(data >> i * BITS_IN_BYTE);
    }
}

template <typename T>
void deserialize(const u8* buffer, T& data)
{
    static_assert(std::is_arithmetic<T>::value, "Type provided for serialize isn't arithmetic");
    u8* byte = reinterpret_cast<u8*>(&data);

    for (std::size_t i = 0; i < sizeof(T); ++i)
    {
        byte = static_cast<u8>(data << i * BITS_IN_BYTE);
    }
}

} // namespace serializer

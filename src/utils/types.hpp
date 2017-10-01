#pragma once
#include <cstdint>

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using DataBuffer = std::vector<u8>;

namespace std
{
inline std::string to_string(const DataBuffer& buffer)
{
    std::stringstream ss;
    ss << "[";
    for (auto i = 0; i < buffer.size(); ++i)
    {
        ss << "0x" << std::hex << static_cast<int>(buffer[i]);
        if (i < buffer.size() - 1)
        {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}
}


#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include <gsl/span>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using DataBuffer = std::vector<u8>;

using BufferSpan = gsl::span<const u8>;
using BufferIndexType = BufferSpan::index_type;

using WriterCallback = std::function<void(const BufferSpan& buffer)>;
using ReaderCallback = std::function<void(const BufferSpan& buffer, const WriterCallback& writer)>;

void defaultReader(const BufferSpan& buffer, const WriterCallback& writer);
void defaultWriter(const BufferSpan& buffer);

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


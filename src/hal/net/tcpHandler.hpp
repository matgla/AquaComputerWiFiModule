#pragma once

#include <functional>
#include <memory>

#include "utils/types.hpp"

namespace hal
{
namespace net
{

using TcpWriterCallback = std::function<void(const u8* buffer, std::size_t length)>;
using TcpReadCallback = std::function<void(const u8* buffer, std::size_t length, TcpWriterCallback writer)>;

void defaultReader(const u8* data, std::size_t size, TcpWriterCallback writer);

} // namespace net
} // namespace hal
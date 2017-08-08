#pragma once

#include <functional>
#include <memory>

#include "utils/types.hpp"

namespace handler
{

using WriterCallback = std::function<void(const u8* buffer, std::size_t length)>;
using ReaderCallback = std::function<void(const u8* buffer, std::size_t length, WriterCallback writer)>;

void defaultReader(const u8* data, std::size_t size, WriterCallback writer);

} // namespace handler
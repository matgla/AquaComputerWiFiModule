#pragma once

#include <functional>
#include <memory>

#include "utils/types.hpp"

namespace dispatcher
{

using WriterCallback = std::function<void(const u8* buffer, std::size_t length)>;
using ReaderCallback =
    std::function<void(const u8* buffer, std::size_t length, WriterCallback writer)>;

void defaultReader(const u8* data, std::size_t size, const WriterCallback& writer);
void defaultWriter(const u8* data, std::size_t size);


} // namespace dispatcher

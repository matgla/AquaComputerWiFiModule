#include "dispatcher/handlers.hpp"

namespace dispatcher
{

void defaultReader(const u8* data, std::size_t size, WriterCallback writer)
{
}

void defaultWriter(const u8* data, std::size_t size)
{
}

} // namespace dispatcher

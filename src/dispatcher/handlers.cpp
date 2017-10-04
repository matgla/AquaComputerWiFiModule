#include "dispatcher/handlers.hpp"

#include <boost/core/ignore_unused.hpp>

namespace dispatcher
{

void defaultReader(const u8* data, std::size_t size, const WriterCallback& writer)
{
    boost::ignore_unused(data);
    boost::ignore_unused(size);
    boost::ignore_unused(writer);
}

void defaultWriter(const u8* data, std::size_t size)
{
    boost::ignore_unused(data);
    boost::ignore_unused(size);
}

} // namespace dispatcher

#include "utils/types.hpp"
#include <boost/core/ignore_unused.hpp>

void defaultReader(const BufferSpan& buffer, const WriterCallback& writer)
{
    boost::ignore_unused(buffer);
    boost::ignore_unused(writer);
}

void defaultWriter(const BufferSpan& buffer)
{
    boost::ignore_unused(buffer);
}

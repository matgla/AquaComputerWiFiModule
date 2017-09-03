#include "stream/fileOStream.hpp"

namespace stream
{

FileOStream::FileOStream(const std::string& path) : std::ostream(&buffer_), buffer_(path)
{
}

} // namespace stream

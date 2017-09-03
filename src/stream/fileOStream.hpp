#pragma once

#include <ostream>
#include <string>

#include "stream/fileBuffer.hpp"

namespace stream
{
class FileOStream : public std::ostream
{
public:
    FileOStream(const std::string& path);

private:
    FileBuffer buffer_;
};
} // namespace stream

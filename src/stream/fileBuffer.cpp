#include "stream/fileBuffer.hpp"

namespace stream
{

FileBuffer::FileBuffer(const std::string& path) : path_(path)
{
    file_.open(path, "w+");
}

FileBuffer::~FileBuffer()
{
    file_.close();
}

std::streambuf::int_type FileBuffer::overflow(std::streambuf::int_type c)
{
    file_.write(std::string(1, static_cast<char>(c)));
    if (c == '\n' || c == 0)
    {
        file_.close();
        file_.open(path_, "w+");
    }
    return c;
}

} // namespace stream

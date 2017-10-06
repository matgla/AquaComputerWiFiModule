#pragma once

#include <streambuf>
#include <string>

#include "hal/fs/file.hpp"

namespace stream
{

class FileBuffer : public std::streambuf
{
public:
    FileBuffer(const std::string& path);
    ~FileBuffer() override;
    FileBuffer(const FileBuffer&) = delete;
    FileBuffer(const FileBuffer&&) = delete;
    FileBuffer& operator=(const FileBuffer&&) = delete;
    FileBuffer& operator=(const FileBuffer&) = delete;
    std::streambuf::int_type overflow(std::streambuf::int_type c) override;

private:
    hal::fs::File file_;
    std::string path_;
};

} // namespace stream

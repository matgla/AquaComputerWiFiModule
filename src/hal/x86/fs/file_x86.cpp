#include "hal/fs/file.hpp"

#include <fstream>
#include <iostream>

namespace fs
{

class File::FileWrapper
{
public:
    std::fstream fs_;
};

File::File()
    : fileWrapper_(new FileWrapper())
{
}

File::~File() = default;

void File::open(const std::string& path)
{
    fileWrapper_->fs_.open(path, std::fstream::in | std::fstream::out);
}

void File::write(const std::string& data)
{
    fileWrapper_->fs_ << data;
}

void File::seek(std::size_t index)
{
    fileWrapper_->fs_.seekg(index);
}

size_t File::read(char* buf, std::size_t len)
{
    size_t prevPos = fileWrapper_->fs_.tellg();
    fileWrapper_->fs_.seekg(0, fileWrapper_->fs_.end);
    size_t fileSize = fileWrapper_->fs_.tellg();
    fileSize -= prevPos;
    fileWrapper_->fs_.seekg(prevPos);
    if (fileSize < len)
    {
        len = fileSize;
    }
    fileWrapper_->fs_.read(buf, len);
    return len;
}

void File::close()
{
    fileWrapper_->fs_.close();
}

bool File::isOpen()
{
    return fileWrapper_->fs_.is_open();
}

} // namespace fs

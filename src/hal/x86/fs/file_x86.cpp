#include "hal/fs/file.hpp"

#include <exception>
#include <fstream>
#include <iostream>

namespace hal
{
namespace fs
{

class File::FileWrapper
{
public:
    FileWrapper() : size_{0}
    {
    }
    std::fstream fs_;
    std::size_t size_;
};

File::File() : fileWrapper_(new FileWrapper())
{
}

File::~File() = default;

void File::open(const std::string& path, const std::string& mode)
{
    std::ios_base::openmode openmode{};

    if (std::string::npos != mode.find('r'))
    {
        openmode |= std::ios::in;
    }

    if (std::string::npos != mode.find('w'))
    {
        openmode |= std::ios::out;
    }

    if (std::string::npos != mode.find('+'))
    {
        openmode |= std::ios::app;
    }

    fileWrapper_->fs_.open(path, openmode);
    if (!isOpen())
    {
        throw std::runtime_error("File " + path + " opening failed...");
    }
    std::streampos size = 0;
    fileWrapper_->fs_.tellg();
    fileWrapper_->fs_.seekg(0, std::ios::end);
    fileWrapper_->size_ = fileWrapper_->fs_.tellg() - size;
    fileWrapper_->fs_.seekg(0);
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

std::size_t File::size()
{
    return fileWrapper_->size_;
}

} // namespace fs
} // namespace hal

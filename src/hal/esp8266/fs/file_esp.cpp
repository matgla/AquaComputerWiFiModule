#include "hal/fs/file.hpp"

#include <algorithm>

#include <FS.h>

#include "logger/logger.hpp"

using SPIFFSFile = File;

namespace hal
{
namespace fs
{

namespace
{
logger::Logger logger("File");
}

class File::FileWrapper
{
public:
    FileWrapper() : isOpen_(false)
    {
    }
    bool isOpen_;
    SPIFFSFile file_;
};

File::File() : fileWrapper_(new FileWrapper())
{
}

File::~File()
{
}

void File::open(const std::string& path)
{
    if (!SPIFFS.begin())
    {
        logger.error() << "SPIFFS mounting failed!";
        return;
    }
    fileWrapper_->file_ = SPIFFS.open(path.c_str(), "wr");
    fileWrapper_->isOpen_ = true;
}

void File::write(const std::string& data)
{
    fileWrapper_->file_.write(reinterpret_cast<const u8*>(data.c_str()), data.length());
}

void File::seek(std::size_t index)
{
    fileWrapper_->file_.seek(index);
}

std::size_t File::read(char* buf, std::size_t len)
{
    fileWrapper_->file_.read(reinterpret_cast<u8*>(buf), len);
}

void File::close()
{
    fileWrapper_->file_.close();
}

bool File::isOpen()
{
    return fileWrapper_->isOpen_;
}

std::size_t File::size()
{
    return fileWrapper_->file_.size();
}

} // namespace fs
} // namespace hal

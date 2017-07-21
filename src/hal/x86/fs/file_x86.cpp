#include "hal/fs/file.hpp"

#include <fstream>

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

void File::close()
{
    fileWrapper_->fs_.close();
}

} // namespace fs

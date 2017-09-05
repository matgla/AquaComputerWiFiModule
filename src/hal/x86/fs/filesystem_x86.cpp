#include "hal/fs/filesystem.hpp"

#include <cstdio>

namespace hal
{
namespace fs
{

bool FileSystem::removeFile(const char* path)
{
    return std::remove(path) == 0;
}

} // namespace fs

} // namespace hal

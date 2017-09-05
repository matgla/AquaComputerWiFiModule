#pragma once

#include <string>

namespace hal
{
namespace fs
{

class FileSystem
{
public:
    static bool removeFile(const char* path);
};

} // namespace fs
} // namespace hal

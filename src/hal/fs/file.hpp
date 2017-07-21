#pragma once

#include <memory>
#include <string>

#include "utils/types.hpp"

namespace fs
{

class File
{
public:
    File();
    ~File();

    void open(const std::string& path);
    void close();

private:
    class FileWrapper;
    std::unique_ptr<FileWrapper> fileWrapper_;
};

} // namespace fs

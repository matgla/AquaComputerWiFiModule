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
    void seek(std::size_t index);
    size_t read(char* buf, std::size_t len);
    void write(const std::string& data);
    void close();
    bool isOpen();

private:
    class FileWrapper;
    std::unique_ptr<FileWrapper> fileWrapper_;
};

} // namespace fs

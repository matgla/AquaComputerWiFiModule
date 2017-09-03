#pragma once

#include <memory>
#include <string>

#include "utils/types.hpp"

namespace hal
{
namespace fs
{

class File
{
public:
    File();
    ~File();

    void open(const std::string& path, const std::string& mode = "rw+");
    void seek(std::size_t index);
    size_t read(char* buf, std::size_t len);
    void write(const std::string& data);
    void close();
    bool isOpen();
    std::size_t size();

private:
    class FileWrapper;
    std::unique_ptr<FileWrapper> fileWrapper_;
};

} // namespace fs
} // namespace hal

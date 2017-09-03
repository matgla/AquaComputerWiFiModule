#include "logger/fileLogger.hpp"

#include "stream/fileOStream.hpp"

namespace logger
{

FileLogger::FileLogger(const std::string& path)
{
    stream_ = std::make_shared<stream::FileOStream>(path);
}

} // namespace logger

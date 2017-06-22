#include "stdOutLogger.hpp"

namespace logger
{

StdOutLogger::StdOutLogger()
{
    stream_.reset(&std::cout, [](std::ostream*){});
}

StdOutLogger::StdOutLogger(const std::string& name) :
    LoggerBase(name)
{
    stream_.reset(&std::cout, [](std::ostream*){});
}

}  // namespace logger
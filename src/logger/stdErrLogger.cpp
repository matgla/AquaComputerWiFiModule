#include "stdErrLogger.hpp"

namespace logger
{

StdErrLogger::StdErrLogger()
{
    stream_.reset(&std::cerr, [](std::ostream*) {});
}

} // namespace logger

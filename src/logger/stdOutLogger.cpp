#include "stdOutLogger.hpp"

namespace logger
{

StdOutLogger::StdOutLogger()
{
    stream_.reset(&std::cout, [](std::ostream*){});
}

}  // namespace logger
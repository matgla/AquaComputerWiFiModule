#include "hal/time/time.hpp"

namespace hal
{
namespace time
{

static u64 currentTime = 0;


u64 milliseconds()
{
    return currentTime;
}

} // namespace time
} // namespace hal


namespace stub
{
namespace time
{

void setCurrentTime(u64 milliseconds)
{
    hal::time::currentTime = milliseconds;
}

void forwardTime(u64 milliseconds)
{
    hal::time::currentTime += milliseconds;
}

} // namespace time
} // namespace stub

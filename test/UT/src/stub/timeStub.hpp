#pragma once

namespace hal
{
namespace time
{

static u64 currentTime;

} // namespace time
} // namespace hal

namespace stub
{
namespace time
{

void setCurrentTime(u64 milliseconds);
void forwardTime(u64 milliseconds);

} // namespace time
} // namespace stub

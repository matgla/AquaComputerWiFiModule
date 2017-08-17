#pragma once

#include "utils/types.hpp"

namespace hal
{
namespace time
{
void sleep(u32 seconds);
void msleep(u32 milliseconds);
void usleep(u32 microseconds);
} // namespace time
} // namespace hal

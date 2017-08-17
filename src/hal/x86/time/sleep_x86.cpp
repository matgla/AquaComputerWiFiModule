#include "hal/time/sleep.hpp"

#include <chrono>
#include <thread>

namespace hal
{
namespace time
{
void sleep(u32 seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void msleep(u32 milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void usleep(u32 microseconds)
{
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}
} // namespace time
} // namespace hal

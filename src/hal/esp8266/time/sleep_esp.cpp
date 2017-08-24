#include "hal/time/sleep.hpp"

#include "ArduinoFixed.hpp"

namespace hal
{
namespace time
{

void sleep(u32 seconds)
{
    delay(seconds * 1000);
}

void msleep(u32 milliseconds)
{
    delay(milliseconds);
}

void usleep(u32 microseconds)
{
    delayMicroseconds(microseconds);
}

} // namespace time
} // namespace hal

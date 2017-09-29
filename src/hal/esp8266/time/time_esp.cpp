#include <Arduino.h>

namespace hal
{
namespace time
{

u64 milliseconds()
{
    return millis();
}

} // namespace time
} // namespace hal

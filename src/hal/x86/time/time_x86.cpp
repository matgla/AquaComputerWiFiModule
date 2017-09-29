#include "hal/time/time.hpp"

#include <chrono>

namespace hal
{
namespace time
{

u64 milliseconds()
{
    auto epoch = std::chrono::high_resolution_clock::from_time_t(0);
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - epoch).count();
}

} // namespace time
} // namespace hal

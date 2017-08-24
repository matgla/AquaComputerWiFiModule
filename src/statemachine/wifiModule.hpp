#pragma once

#include <boost/sml.hpp>

#include "logger/logger.hpp"
#include "statemachine/helper.hpp"

namespace statemachine
{

// <------------- states -------------> //
namespace state
{
struct Uninitialized;
struct WiFiInitialized;
struct LoggerInitialized;
struct SerialInitialized;
struct AllComponentsInitialized;
} // namespace state
// <------------- events -------------> //
namespace event
{
// clang-format off
struct InitializeSerial {};
struct InitializeWiFi {};
struct InitializeLogger {};
// clang-format on
} // namespace event

class WiFiModule
{
public:
    WiFiModule() : logger_("WiFiModule")
    {
    }

    auto operator()() noexcept
    {
        using namespace boost::sml;

        // clang-format off
        return make_transition_table(

        );
        // clang-format on
    }

private:
    logger::Logger logger_;
};

} // namespace statemachine

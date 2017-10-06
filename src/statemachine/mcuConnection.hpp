// #pragma once

// #include <memory>

// #include <boost/sml.hpp>

// #include "dispatcher/jsonHandler.hpp"
// #include "logger/logger.hpp"
// #include "statemachine/helper.hpp"

// namespace sml = boost::sml;

// namespace statemachine
// {

// // <------------- states -------------> //
// namespace states
// {
// struct NotConnected;
// struct Connecting;
// struct Connected;
// struct ConnectionLost;
// struct Failed;
// struct SwUpgrade;
// } // namespace state

// // <------------- events -------------> //
// namespace events
// {
// struct Connect
// {
// };
// struct Success
// {
// };
// struct Fail
// {
// };
// struct SendInfo
// {
// };
// }

// class McuConnection
// {
// public:
//     auto operator()() noexcept
//     {
//         using namespace sml;
//         using namespace statemachine::states;
//         using namespace statemachine::events;

//         return make_transition_table(
//             // clang-format off
//             // state<fin_wait_2> + event<fin> [ is_fin_valid ] / send_ack = state<timed_wait>
//             // |--------- source ---------|--------- event ---------|--------- guard
//             ---------|---------------------- action ----------------------|--------- desitnation
//             ---------|
//                 *state<NotConnected>      + event<Connect>                                    /
//                 call(this, &McuConnection::sendHandshake)          = state<Connecting>,
//                 state<Connecting>         + event<Success>                                    /
//                 call(this, &McuConnection::connected)              = state<Connected>,
//                 state<Connected>          + event<SendInfo>                                   /
//                 call(this, &McuConnection::sendInfo)               = state<Connected>
//             // clang-format on
//         );
//     }

// private:
//     using HandlerPtr = dispatcher::JsonHandler*;

//     void sendHandshake(HandlerPtr handler, logger::Logger& logger);
//     void sendInfo(HandlerPtr handler, logger::Logger& logger);
//     void connected(HandlerPtr handler, logger::Logger& logger);
// };

// } // namespace statemachine

// #include "statemachine/mcuConnectionFrontEnd.hpp"

// namespace statemachine
// {

// McuConnectionFrontEnd::McuConnectionFrontEnd(dispatcher::JsonHandler& handler)
//     : handler_(handler), logger_("McuConnection"), backend_{&handler_, logger_}
// {
// }

// void McuConnectionFrontEnd::connect()
// {
//     backend_.process_event(events::Connect{});
// }

// void McuConnectionFrontEnd::connected()
// {
//     backend_.process_event(events::Success{});
// }

// void McuConnectionFrontEnd::sendDeviceInfo()
// {
//     backend_.process_event(events::SendInfo{});
// }

// boost::sml::sm<McuConnection>& McuConnectionFrontEnd::backend()
// {
//     return backend_;
// }


// } // namespace statemachine

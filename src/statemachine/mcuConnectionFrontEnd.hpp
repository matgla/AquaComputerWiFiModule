// #pragma once

// #include <boost/sml.hpp>

// #include "dispatcher/jsonHandler.hpp"
// #include "logger/logger.hpp"
// #include "statemachine/mcuConnection.hpp"

// namespace statemachine
// {

// class McuConnectionFrontEnd
// {
// public:
//     McuConnectionFrontEnd(dispatcher::JsonHandler& handler);

//     void connect();
//     void connected();
//     void sendDeviceInfo();

//     boost::sml::sm<McuConnection>& backend();

// protected:
//     // dispatcher::JsonHandler& handler_;
//     logger::Logger logger_;
//     boost::sml::sm<McuConnection> backend_;
// };
// } // namespace statemachine

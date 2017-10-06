// #include "statemachine/mcuConnection.hpp"

// #include <string>

// #include "message/messages.hpp"

// namespace statemachine
// {

// void McuConnection::sendHandshake(HandlerPtr handler, logger::Logger& logger)
// {
//     std::string msg = "{\"id\":\"handshake\"}";
//     DynamicJsonBuffer buffer;
//     JsonObject& jsonMsg = buffer.parseObject(msg);
//     handler->send(jsonMsg);

//     logger.info() << "Handshake was sent to MCU";
// }

// void McuConnection::sendInfo(HandlerPtr handler, logger::Logger& logger)
// {
//     logger.info() << "Sending module info!";
//     DynamicJsonBuffer buffer;
//     JsonObject& jsonMsg = buffer.createObject();
//     jsonMsg["id"] = message::ModuleInfo;
// }

// void McuConnection::connected(HandlerPtr handler, logger::Logger& logger)
// {
//     logger.info() << "Connected to MCU!";
// }

// } // namespace statemachine

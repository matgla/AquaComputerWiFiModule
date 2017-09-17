#include "dispatcher/handler/handshakeHandler.hpp"

namespace dispatcher
{
namespace handler
{

HandshakeHandler::HandshakeHandler(statemachine::McuConnectionFrontEnd& sm)
    : sm_(sm), logger_("HandshakeHandler")
{
}


void HandshakeHandler::onData(const JsonObject& data)
{
    logger_.debug() << "Received handshake from MCU";
    sm_.connected();
}

} // namespace handler
} // namespace dispatcher

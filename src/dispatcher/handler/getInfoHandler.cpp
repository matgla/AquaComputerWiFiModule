#include "dispatcher/handler/getInfoHandler.hpp"

#include <boost/core/ignore_unused.hpp>

namespace dispatcher
{
namespace handler
{

GetInfoHandler::GetInfoHandler(statemachine::McuConnectionFrontEnd& sm)
    : sm_(sm), logger_("HandshakeHandler")
{
}

void GetInfoHandler::onData(const JsonObject& data)
{
    boost::ignore_unused(data);
    logger_.debug() << "Received get info request";
    sm_.sendDeviceInfo();
}

} // namespace handler
} // namespace dispatcher

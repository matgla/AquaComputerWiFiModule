#pragma once

#include "handler/IHandler.hpp"
#include "handler/jsonHandler.hpp"
#include "logger/logger.hpp"

namespace handler
{

class HandshakeHandler : public IHandler
{
public:
    HandshakeHandler(handler::JsonHandler* handler);
    void onData(const JsonObject& data) override;

private:
    handler::JsonHandler* handler_;
    logger::Logger logger_;
};

} // namespace handler

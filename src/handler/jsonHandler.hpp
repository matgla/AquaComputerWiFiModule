#pragma once

#include <map>
#include <string>
#include <vector>

#include "handler/IFrameHandler.hpp"
#include "handler/IMessageHandler.hpp"
#include "logger/logger.hpp"
#include "utils/types.hpp"

namespace handler
{

class JsonHandler : public IFrameHandler
{
public:
    JsonHandler();
    void onRead(std::vector<u8> data) override;

    void addMessageHandler(const std::string& messageName,
                           IMessageHandler::MessageHandlerPtr handler);

protected:
    logger::Logger logger_;

    std::map<std::string, IMessageHandler::MessageHandlerPtr> handlers_;
};

} // namespace handler

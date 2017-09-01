#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "handler/IDataReceiver.hpp"
#include "handler/IFrameHandler.hpp"
#include "logger/logger.hpp"

namespace handler
{
class Dispatcher
{
public:
    using HandlerContainer = std::map<std::string, IFrameHandler::HandlerPtr>;

    Dispatcher();

    void addHandler(IFrameHandler::HandlerPtr receiver, const std::string& name);
    void removeHandler(const std::string& name);

    // void addReceiver(IDataReceiver::RawDataReceiverPtr dataReceiver, const std::string& name);
    // void removeReceiver(const std::string& name);

    // void connect(const std::string& receiverName, const std::string& handlerName);

    const HandlerContainer& getHandlers() const;
    // const ReceiverContainer& getReceivers() const;

    // const ConnectionContainer& getConnections() const;

protected:
    logger::Logger logger_;

    // ReceiverContainer receivers_;
    HandlerContainer handlers_;

    // ConnectionContainer connections_;
};

} // namespace handler

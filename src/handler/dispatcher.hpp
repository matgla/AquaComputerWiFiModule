#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "handler/IDataReceiver.hpp"
#include "handler/IFrameHandler.hpp"
#include "handler/IFrameReceiver.hpp"
#include "logger/logger.hpp"

namespace handler
{
class Dispatcher
{
public:
    using FrameReceiverContainer = std::map<std::string, IFrameReceiver::ReceiverPtr>;
    using RawDataReceiverContainer = std::map<std::string, IDataReceiver::RawDataReceiverPtr>;
    using FrameHandlerContainer = std::map<std::string, IFrameHandler::HandlerPtr>;

    using Connection = std::pair<std::string, std::string>;
    using ConnectionContainer = std::vector<Connection>;

    Dispatcher();

    void addFrameReceiver(IFrameReceiver::ReceiverPtr receiver, const std::string& name);
    void removeFrameReceiver(const std::string& name);

    void addRawDataReceiver(IDataReceiver::RawDataReceiverPtr dataReceiver,
                            const std::string& name);
    void removeRawDataReceiver(const std::string& name);

    void connectReceivers(const std::string& rawDataReceiverName,
                          const std::string& frameReceiverName);

    void addHandler(IFrameHandler::HandlerPtr handler, const std::string& name);
    void removeHandler(const std::string& name);

    void connectHandler(const std::string& frameReceiverName, const std::string& handlerName);

    const FrameReceiverContainer& getFrameReceivers() const;
    const RawDataReceiverContainer& getRawDataReceivers() const;
    const FrameHandlerContainer& getFrameHandlers() const;

    const ConnectionContainer& getRawAndFrameConnections() const;
    const ConnectionContainer& getFrameAndHandlerConnections() const;

protected:
    logger::Logger logger_;

    FrameReceiverContainer frameReceivers_;
    RawDataReceiverContainer rawDataReceivers_;
    FrameHandlerContainer handlers_;

    ConnectionContainer rawAndFrameReceiverConnections_;
    ConnectionContainer frameAndHandlerConnections_;
};

} // namespace handler

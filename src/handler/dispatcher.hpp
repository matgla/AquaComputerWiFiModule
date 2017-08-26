#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "handler/IDataReceiver.hpp"
#include "handler/IFrameReceiver.hpp"
#include "logger/logger.hpp"

namespace handler
{
class Dispatcher
{
public:
    using FrameReceiverContainer = std::map<std::string, IFrameReceiver::ReceiverPtr>;
    using RawDataReceiverContainer = std::map<std::string, IDataReceiver::RawDataReceiverPtr>;

    Dispatcher();

    void addFrameReceiver(IFrameReceiver::ReceiverPtr receiver, const std::string& name);
    void removeFrameReceiver(const std::string& name);

    void addRawDataReceiver(IDataReceiver::RawDataReceiverPtr dataReceiver,
                            const std::string& name);
    void removeRawDataReceiver(const std::string& name);

    void connectReceivers(const std::string& rawDataReceiverName,
                          const std::string& frameReceiverName);

protected:
    logger::Logger logger_;

    FrameReceiverContainer frameReceivers_;
    RawDataReceiverContainer rawDataReceivers_;

    using Connection = std::pair<std::string, std::string>;
    using ConnectionContainer = std::vector<Connection>;

    ConnectionContainer rawAndFrameReceiverConnections_;
};

} // namespace handler

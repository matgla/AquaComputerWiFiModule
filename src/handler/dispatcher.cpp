#include "handler/dispatcher.hpp"

#include <algorithm>

namespace handler
{

Dispatcher::Dispatcher() : logger_("Dispatcher")
{
}

void Dispatcher::addFrameReceiver(IFrameReceiver::ReceiverPtr receiver, const std::string& name)
{
    if (frameReceivers_.find(name) != frameReceivers_.end())
    {
        logger_.warn() << "Frame receiver with name: " << name << " exists...";
        return;
    }

    frameReceivers_[name] = receiver;
    logger_.info() << "Added frame receiver with name: " << name;
}

void Dispatcher::removeFrameReceiver(const std::string& name)
{


    if (frameReceivers_.find(name) != frameReceivers_.end())
    {
        // disconnect
        std::remove_if(rawAndFrameReceiverConnections_.begin(),
                       rawAndFrameReceiverConnections_.end(),
                       [&name](const Connection& connection) { return name == connection.second; });
        //
        logger_.info() << "Removed frame receiver with name: " << name;
    }
    else
    {
        logger_.warn() << "Trying to remove not existing frame receiver: " << name;
    }
}

void Dispatcher::addRawDataReceiver(IDataReceiver::RawDataReceiverPtr dataReceiver,
                                    const std::string& name)
{
    if (rawDataReceivers_.find(name) != rawDataReceivers_.end())
    {
        logger_.warn() << "Raw data receiver with name: " << name << " exists...";
    }

    rawDataReceivers_[name] = dataReceiver;
    logger_.info() << "Added raw data receiver with name: " << name;
}

void Dispatcher::removeRawDataReceiver(const std::string& name)
{
    if (rawDataReceivers_.erase(name))
    {
        logger_.info() << "Removed raw data receiver with name: " << name;
    }
    else
    {
        logger_.warn() << "Trying to remove not existing raw data receiver: " << name;
    }
}

void Dispatcher::connectReceivers(const std::string& rawDataReceiverName,
                                  const std::string& frameReceiverName)
{
    if (frameReceivers_.find(frameReceiverName) == frameReceivers_.end())
    {
        logger_.warn() << "Frame receiver with name: " << frameReceiverName
                       << " not exists. Can't connect!";
        return;
    }

    if (rawDataReceivers_.find(rawDataReceiverName) == rawDataReceivers_.end())
    {
        logger_.warn() << "Raw data receiver with name: " << rawDataReceiverName
                       << " not exists. Can't connect!";
        return;
    }

    rawDataReceivers_[rawDataReceiverName]->setHandler(
        std::bind(&IFrameReceiver::onRead, frameReceivers_[frameReceiverName],
                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    rawAndFrameReceiverConnections_.push_back(
        std::make_pair(rawDataReceiverName, frameReceiverName));
    logger_.info() << "Connected " << frameReceiverName << " to " << rawDataReceiverName;
}

} // namespace handler

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
        for (auto& connection : rawAndFrameReceiverConnections_)
        {
            if (connection.second == name)
            {
                rawDataReceivers_[connection.first]->setHandler(&defaultReader);
            }
        }

        rawAndFrameReceiverConnections_.erase(
            std::remove_if(
                rawAndFrameReceiverConnections_.begin(), rawAndFrameReceiverConnections_.end(),
                [&name](const Connection& connection) { return name == connection.second; }),
            rawAndFrameReceiverConnections_.end());

        frameAndHandlerConnections_.erase(std::remove_if(frameAndHandlerConnections_.begin(),
                                                         frameAndHandlerConnections_.end(),
                                                         [&name](const Connection& connection) {
                                                             return name == connection.first;
                                                         }),
                                          frameAndHandlerConnections_.end());

        frameReceivers_.erase(name);
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
    if (rawDataReceivers_.find(name) != rawDataReceivers_.end())
    {
        logger_.info() << "Removed raw data receiver with name: " << name;

        rawAndFrameReceiverConnections_.erase(
            std::remove_if(
                rawAndFrameReceiverConnections_.begin(), rawAndFrameReceiverConnections_.end(),
                [&name](const Connection& connection) { return name == connection.first; }),
            rawAndFrameReceiverConnections_.end());

        rawDataReceivers_.erase(name);
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

void Dispatcher::addHandler(IFrameHandler::HandlerPtr handler, const std::string& name)
{
    if (handlers_.find(name) != handlers_.end())
    {
        logger_.warn() << "Handler with name: " << name << " exists...";
    }

    handlers_[name] = handler;
    logger_.info() << "Added handler with name: " << name;
}

void Dispatcher::removeHandler(const std::string& name)
{
    if (handlers_.find(name) != handlers_.end())
    {
        // disconnect
        for (auto& connection : frameAndHandlerConnections_)
        {
            if (connection.second == name)
            {
                frameReceivers_[connection.first]->setHandler(IFrameReceiver::DefaultHandler);
            }
        }

        frameAndHandlerConnections_.erase(std::remove_if(frameAndHandlerConnections_.begin(),
                                                         frameAndHandlerConnections_.end(),
                                                         [&name](const Connection& connection) {
                                                             return name == connection.second;
                                                         }),
                                          frameAndHandlerConnections_.end());

        handlers_.erase(name);
        logger_.info() << "Removed handler with name: " << name;
    }
    else
    {
        logger_.warn() << "Trying to remove not existing handler: " << name;
    }
}

void Dispatcher::connectHandler(const std::string& frameReceiverName,
                                const std::string& handlerName)
{
    if (frameReceivers_.find(frameReceiverName) == frameReceivers_.end())
    {
        logger_.warn() << "Frame receiver with name: " << frameReceiverName
                       << " not exists. Can't connect!";
        return;
    }

    if (handlers_.find(handlerName) == handlers_.end())
    {
        logger_.warn() << "Handler with name: " << handlerName << " not exists. Can't connect!";
        return;
    }

    frameReceivers_[frameReceiverName]->setHandler(
        std::bind(&IFrameHandler::onRead, handlers_[handlerName], std::placeholders::_1));

    frameAndHandlerConnections_.push_back(std::make_pair(frameReceiverName, handlerName));
    logger_.info() << "Connected " << handlerName << " to " << frameReceiverName;
}

const Dispatcher::FrameReceiverContainer& Dispatcher::getFrameReceivers() const
{
    return frameReceivers_;
}

const Dispatcher::RawDataReceiverContainer& Dispatcher::getRawDataReceivers() const
{
    return rawDataReceivers_;
}

const Dispatcher::FrameHandlerContainer& Dispatcher::getFrameHandlers() const
{
    return handlers_;
}

const Dispatcher::ConnectionContainer& Dispatcher::getRawAndFrameConnections() const
{
    return rawAndFrameReceiverConnections_;
}
const Dispatcher::ConnectionContainer& Dispatcher::getFrameAndHandlerConnections() const
{
    return frameAndHandlerConnections_;
}

} // namespace handler

#include "handler/dispatcher.hpp"

#include <algorithm>

#include "handler/IFrameHandler.hpp"

namespace handler
{

Dispatcher::Dispatcher() : logger_("Dispatcher")
{
}

void Dispatcher::addHandler(IFrameHandler::HandlerPtr handler, const std::string& name)
{
    if (handlers_.find(name) != handlers_.end())
    {
        logger_.warn() << "Handler with name: " << name << " exists...";
        return;
    }

    handlers_[name] = std::move(handler);
    logger_.info() << "Added handler with name: " << name;
}

// void Dispatcher::addReceiver(IDataReceiver::RawDataReceiverPtr dataReceiver,
//                              const std::string& name)
// {
//     if (receivers_.find(name) != receivers_.end())
//     {
//         logger_.warn() << "Raw data receiver with name: " << name << " exists...";
//     }

//     receivers_[name] = dataReceiver;
//     logger_.info() << "Added raw data receiver with name: " << name;
// }

// void Dispatcher::removeReceiver(const std::string& name)
// {
//     if (receivers_.find(name) != receivers_.end())
//     {
//         logger_.info() << "Removed raw data receiver with name: " << name;

//         connections_.erase(std::remove_if(connections_.begin(), connections_.end(),
//                                           [&name](const Connection& connection) {
//                                               return name == connection.first;
//                                           }),
//                            connections_.end());

//         receivers_.erase(name);
//     }
//     else
//     {
//         logger_.warn() << "Trying to remove not existing raw data receiver: " << name;
//     }
// }

// void Dispatcher::connect(const std::string& rawDataReceiverName,
//                          const std::string& frameReceiverName)
// {
//     if (handlers_.find(frameReceiverName) == handlers_.end())
//     {
//         logger_.warn() << "Frame receiver with name: " << frameReceiverName
//                        << " not exists. Can't connect!";
//         return;
//     }

//     if (receivers_.find(rawDataReceiverName) == receivers_.end())
//     {
//         logger_.warn() << "Raw data receiver with name: " << rawDataReceiverName
//                        << " not exists. Can't connect!";
//         return;
//     }

//     receivers_[rawDataReceiverName]->setHandler(
//         std::bind(&IFrameHandler::onRead, handlers_[frameReceiverName].get(),
//         std::placeholders::_1,
//                   std::placeholders::_2, std::placeholders::_3));

//     connections_.push_back(std::make_pair(rawDataReceiverName, frameReceiverName));
//     logger_.info() << "Connected " << frameReceiverName << " to " << rawDataReceiverName;
// }

void Dispatcher::removeHandler(const std::string& name)
{
    if (handlers_.find(name) != handlers_.end())
    {
        handlers_.erase(name);
        logger_.info() << "Removed handler with name: " << name;
    }
    else
    {
        logger_.warn() << "Trying to remove not existing handler: " << name;
    }
}

const Dispatcher::HandlerContainer& Dispatcher::getHandlers() const
{
    return handlers_;
}

// const Dispatcher::ReceiverContainer& Dispatcher::getReceivers() const
// {
//     return receivers_;
// }

// const Dispatcher::ConnectionContainer& Dispatcher::getConnections() const
// {
//     return connections_;
// }

} // namespace handler

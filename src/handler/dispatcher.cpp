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

} // namespace handler

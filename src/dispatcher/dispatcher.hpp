#pragma once

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "dispatcher/IDataReceiver.hpp"
#include "dispatcher/IFrameHandler.hpp"
#include "logger/logger.hpp"

namespace dispatcher
{
class Dispatcher
{
public:
    using HandlerContainer = std::map<std::string, IFrameHandler::HandlerPtr>;

    Dispatcher();

    void addHandler(IFrameHandler::HandlerPtr handler, const std::string& name);
    void removeHandler(const std::string& name);

    const HandlerContainer& getHandlers() const;

protected:
    logger::Logger logger_;

    HandlerContainer handlers_;
};

} // namespace dispatcher

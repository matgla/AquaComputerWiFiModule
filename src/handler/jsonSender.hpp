#pragma once

#include <memory>

#include "handler/IMessageSender.hpp"

class JsonObject;
class JsonArray;

namespace handler
{

class JsonSender
{
public:
    void setMessageSender();

    void send(const JsonObject& data);
    void send(const JsonArray& data);

protected:
    std::unique_ptr<IMessageSender> messageSender_;
};

} // namespace handler

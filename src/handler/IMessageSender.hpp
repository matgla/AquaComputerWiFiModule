#pragma once

#include <memory>

#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace handler
{

class IMessageSender
{
public:
    virtual ~IMessageSender() = default;

    virtual void setWriter(WriterCallback writer) = 0;
    virtual void send(const std::vector<u8>& data) = 0;
};

} // namespace handler

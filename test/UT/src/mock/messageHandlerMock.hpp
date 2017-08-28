#pragma once

#include "handler/IMessageHandler.hpp"

#include <gmock/gmock.h>

namespace mock
{

struct MessageHandlerMock : public handler::IMessageHandler
{
public:
    MOCK_METHOD1(onData, void(JsonObject&));
};

} // namespace mock

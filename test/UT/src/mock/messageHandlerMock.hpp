#pragma once

#include "handler/IHandler.hpp"

#include <gmock/gmock.h>

namespace mock
{

struct MessageHandlerMock : public handler::IHandler
{
public:
    MOCK_METHOD1(onData, void(const JsonObject&));
};

} // namespace mock

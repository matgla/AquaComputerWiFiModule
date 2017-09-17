#pragma once

#include "dispatcher/IHandler.hpp"

#include <gmock/gmock.h>

namespace mock
{

struct MessageHandlerMock : public dispatcher::IHandler
{
public:
    MOCK_METHOD1(onData, void(const JsonObject&));
};

} // namespace mock

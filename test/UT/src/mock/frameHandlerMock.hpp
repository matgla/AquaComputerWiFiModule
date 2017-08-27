#pragma once

#include "handler/IFrameHandler.hpp"

#include <gmock/gmock.h>

namespace mock
{

class FrameHandlerMock : public handler::IFrameHandler
{
public:
    MOCK_METHOD1(onRead, void(std::vector<u8>));
};

} // namespace mock

#pragma once

#include "handler/IFrameHandler.hpp"

#include <gmock/gmock.h>

#include "utils/types.hpp"

namespace mock
{

class FrameHandlerMock : public handler::IFrameHandler
{
public:
    MOCK_METHOD3(onRead, void(const u8* buffer, std::size_t lenth, handler::WriterCallback writer));
    MOCK_METHOD1(send, void(const DataBuffer& data));
};

} // namespace mock

#pragma once

#include "dispatcher/IFrameHandler.hpp"

#include <gmock/gmock.h>

#include "utils/types.hpp"

namespace mock
{

class FrameHandlerMock : public dispatcher::IFrameHandler
{
public:
    MOCK_METHOD3(onRead, void(const u8* buffer, std::size_t lenth, dispatcher::WriterCallback writer));
    MOCK_METHOD1(send, void(const DataBuffer& data));
};

} // namespace mock

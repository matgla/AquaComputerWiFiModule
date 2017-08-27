#pragma once

#include <gmock/gmock.h>

#include "handler/IFrameReceiver.hpp"

namespace mock
{

class FrameReceiverMock : public handler::IFrameReceiver
{
public:
    MOCK_METHOD3(onRead, void(const u8*, std::size_t, handler::WriterCallback));
    MOCK_METHOD1(setHandler, void(DataHandler));
};

} // namespace mock

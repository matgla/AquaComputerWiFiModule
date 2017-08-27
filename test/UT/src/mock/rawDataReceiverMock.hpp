#pragma once

#include "handler/IDataReceiver.hpp"

#include <gmock/gmock.h>

namespace mock
{

class RawDataReceiverMock : public handler::IDataReceiver
{
public:
    MOCK_METHOD1(setHandler, void(handler::ReaderCallback));
};

} // namespace mock

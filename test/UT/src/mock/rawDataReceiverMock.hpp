#pragma once

#include "dispatcher/IDataReceiver.hpp"

#include <gmock/gmock.h>

namespace mock
{

class RawDataReceiverMock : public dispatcher::IDataReceiver
{
public:
    MOCK_METHOD1(setHandler, void(dispatcher::ReaderCallback));
};

} // namespace mock

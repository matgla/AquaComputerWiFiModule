#pragma once

#include <gmock/gmock.h>

namespace test
{
namespace mock
{

struct WriterHandlerMock
{
    MOCK_METHOD2(doWrite, void(const u8*, std::size_t));
};

} // namespace mock
} // namespace test

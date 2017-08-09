#include "handler/dispatcher.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "matcher/arrayCompare.hpp"
#include "mock/writerHandlerMock.hpp"

using namespace testing;

namespace handler
{

struct ReceiverForTest : public handler::IDataReceiver
{
    void setHandler(ReaderCallback readerCallback) override
    {
        readerCallback_ = readerCallback;
    }

    ReaderCallback readerCallback_;
};

struct BaseHandlerTest : public testing::Test
{
    BaseHandlerTest() : handler_(std::make_unique<ReceiverForTest>())
    {
        receiver_ = static_cast<ReceiverForTest*>(handler_.getReceiver());
    }

    handler::BaseHandler handler_;
    ReceiverForTest* receiver_;
};

TEST_F(BaseHandlerTest, InitializeMembers)
{
    EXPECT_FALSE(handler_.transmissionStarted());
    EXPECT_FALSE(handler_.allDataArrived());
}

TEST_F(BaseHandlerTest, StartTransmissionAndSendAck)
{
    EXPECT_FALSE(handler_.transmissionStarted());

    u8 expectedAnswer[] = {0xff};
    u8 msg[] = {0xA2};

    test::mock::WriterHandlerMock writerMock;

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    EXPECT_TRUE(handler_.transmissionStarted());
}

} // namespace handler

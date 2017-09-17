#include "dispatcher/dispatcher.hpp"

#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mock/frameHandlerMock.hpp"
#include "mock/messageHandlerMock.hpp"
#include "mock/rawDataReceiverMock.hpp"

using namespace testing;

namespace dispatcher
{

class DispatcherShould : public Test
{
public:
    DispatcherShould()
        : firstHandlerName_("FirstHandler"), secondHandlerName_("SecondHandler"),
          handlers_(dispatcher_.getHandlers())
    {
    }

    void SetUp() override
    {
        firstHandler_.reset(new mock::FrameHandlerMock());
        secondHandler_.reset(new mock::FrameHandlerMock());

        firstHandlerPtr_ = firstHandler_.get();
        secondHandlerPtr_ = secondHandler_.get();
    }

protected:
    dispatcher::Dispatcher dispatcher_;

    const std::string firstHandlerName_;
    const std::string secondHandlerName_;

    IFrameHandler::HandlerPtr firstHandler_;
    IFrameHandler::HandlerPtr secondHandler_;

    IFrameHandler* firstHandlerPtr_;
    IFrameHandler* secondHandlerPtr_;

    const Dispatcher::HandlerContainer& handlers_;
};

TEST_F(DispatcherShould, AddHandler)
{
    EXPECT_EQ(0, handlers_.size());

    dispatcher_.addHandler(std::move(firstHandler_), firstHandlerName_);
    EXPECT_EQ(1, handlers_.count(firstHandlerName_));
    EXPECT_EQ(firstHandlerPtr_, handlers_.at(firstHandlerName_).get());

    dispatcher_.addHandler(std::move(secondHandler_), secondHandlerName_);
    EXPECT_EQ(1, handlers_.count(secondHandlerName_));
    EXPECT_EQ(secondHandlerPtr_, handlers_.at(secondHandlerName_).get());
    EXPECT_EQ(2, handlers_.size());
}

TEST_F(DispatcherShould, RemoveHandler)
{
    dispatcher_.addHandler(std::move(firstHandler_), firstHandlerName_);
    EXPECT_EQ(1, handlers_.count(firstHandlerName_));
    EXPECT_EQ(firstHandlerPtr_, handlers_.at(firstHandlerName_).get());

    dispatcher_.addHandler(std::move(secondHandler_), secondHandlerName_);
    EXPECT_EQ(1, handlers_.count(secondHandlerName_));
    EXPECT_EQ(secondHandlerPtr_, handlers_.at(secondHandlerName_).get());
    EXPECT_EQ(2, handlers_.size());

    dispatcher_.removeHandler("NotExist");
    EXPECT_EQ(2, handlers_.size());

    dispatcher_.removeHandler(firstHandlerName_);
    EXPECT_EQ(0, handlers_.count(firstHandlerName_));
    dispatcher_.removeHandler(secondHandlerName_);
    EXPECT_EQ(0, handlers_.count(secondHandlerName_));
}

} // namespace dispatcher

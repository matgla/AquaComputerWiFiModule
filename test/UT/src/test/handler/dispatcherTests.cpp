#include "handler/dispatcher.hpp"

#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mock/frameHandlerMock.hpp"
#include "mock/frameReceiverMock.hpp"
#include "mock/messageHandlerMock.hpp"
#include "mock/rawDataReceiverMock.hpp"

using namespace testing;

namespace handler
{

class DispatcherShould : public Test
{
public:
    DispatcherShould()
        : firstHandlerName_("FirstHandler"), secondHandlerName_("SecondHandler"),
          firstReceiverName_("FirstReceiver"), secondReceiverName_("SecondReceiver"),
          handlers_(dispatcher_.getHandlers()), receivers_(dispatcher_.getReceivers())
    {
    }

    void SetUp() override
    {
        firstHandler_.reset(new mock::FrameHandlerMock());
        secondHandler_.reset(new mock::FrameHandlerMock());
        firstReceiver_.reset(new mock::RawDataReceiverMock());
        secondReceiver_.reset(new mock::RawDataReceiverMock());

        firstHandlerPtr_ = firstHandler_.get();
        secondHandlerPtr_ = secondHandler_.get();
        firstReceiverPtr_ = static_cast<mock::RawDataReceiverMock*>(firstReceiver_.get());
        secondReceiverPtr_ = static_cast<mock::RawDataReceiverMock*>(secondReceiver_.get());
    }

protected:
    handler::Dispatcher dispatcher_;

    const std::string firstHandlerName_;
    const std::string secondHandlerName_;
    const std::string firstReceiverName_;
    const std::string secondReceiverName_;

    IFrameHandler::HandlerPtr firstHandler_;
    IFrameHandler::HandlerPtr secondHandler_;
    IDataReceiver::RawDataReceiverPtr firstReceiver_;
    IDataReceiver::RawDataReceiverPtr secondReceiver_;

    IFrameHandler* firstHandlerPtr_;
    IFrameHandler* secondHandlerPtr_;
    mock::RawDataReceiverMock* firstReceiverPtr_;
    mock::RawDataReceiverMock* secondReceiverPtr_;

    const Dispatcher::HandlerContainer& handlers_;
    const Dispatcher::ReceiverContainer& receivers_;
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

TEST_F(DispatcherShould, AddReceiver)
{
    dispatcher_.addReceiver(firstReceiver_, firstReceiverName_);
    EXPECT_EQ(1, receivers_.count(firstReceiverName_));
    EXPECT_EQ(firstReceiverPtr_, receivers_.at(firstReceiverName_).get());
    dispatcher_.addReceiver(secondReceiver_, secondReceiverName_);
    EXPECT_EQ(1, receivers_.count(secondReceiverName_));
    EXPECT_EQ(secondReceiverPtr_, receivers_.at(secondReceiverName_).get());
}

TEST_F(DispatcherShould, RemoveReceiver)
{
    dispatcher_.addReceiver(firstReceiver_, firstReceiverName_);
    dispatcher_.addReceiver(secondReceiver_, secondReceiverName_);
    EXPECT_EQ(1, receivers_.count(firstReceiverName_));
    EXPECT_EQ(1, receivers_.count(secondReceiverName_));

    EXPECT_EQ(2, receivers_.size());
    dispatcher_.removeReceiver("notexist");
    EXPECT_EQ(2, receivers_.size());

    dispatcher_.removeReceiver(firstReceiverName_);
    EXPECT_EQ(0, receivers_.count(firstReceiverName_));
    dispatcher_.removeReceiver(secondReceiverName_);
    EXPECT_EQ(0, receivers_.count(secondReceiverName_));
}

TEST_F(DispatcherShould, ManageConnection)
{
    dispatcher_.addHandler(std::move(firstHandler_), firstHandlerName_);
    dispatcher_.addHandler(std::move(secondHandler_), secondHandlerName_);
    dispatcher_.addReceiver(firstReceiver_, firstReceiverName_);
    dispatcher_.addReceiver(secondReceiver_, secondReceiverName_);

    const auto& connections = dispatcher_.getConnections();

    {
        EXPECT_CALL(*firstReceiverPtr_, setHandler(_)).Times(1);
        dispatcher_.connect(firstReceiverName_, firstHandlerName_);
    }

    EXPECT_EQ(1, connections.size());
    EXPECT_EQ(std::make_pair(firstReceiverName_, firstHandlerName_), connections.front());

    EXPECT_CALL(*secondReceiverPtr_, setHandler(_)).Times(1);
    dispatcher_.connect(secondReceiverName_, secondHandlerName_);

    EXPECT_EQ(2, connections.size());
    EXPECT_EQ(std::make_pair(secondReceiverName_, secondHandlerName_), connections.back());

    EXPECT_CALL(*firstReceiverPtr_, setHandler(_)).Times(1);
    dispatcher_.removeHandler(firstHandlerName_);

    EXPECT_EQ(1, connections.size());
    EXPECT_EQ(std::make_pair(secondReceiverName_, secondHandlerName_), connections.front());

    EXPECT_CALL(*secondReceiverPtr_, setHandler(_)).Times(1);
    dispatcher_.removeHandler(secondHandlerName_);

    EXPECT_EQ(0, connections.size());
}

} // namespace handler

#include "handler/dispatcher.hpp"

#include <memory>
#include <string>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mock/frameHandlerMock.hpp"
#include "mock/frameReceiverMock.hpp"
#include "mock/rawDataReceiverMock.hpp"

using namespace testing;

namespace handler
{
class DispatcherShould : public Test
{
public:
    DispatcherShould()
    {
    }

protected:
    handler::Dispatcher dispatcher_;
};

TEST_F(DispatcherShould, AddFrameReceiver)
{
    const std::string& frameReceiverName = "FrameReceiver";
    const std::string& secondFrameReceiverName = "SecondFrameReceiver";

    const auto& receivers = dispatcher_.getFrameReceivers();
    EXPECT_EQ(0, receivers.size());
    auto frameReceiver = std::make_shared<mock::FrameReceiverMock>();
    auto secondFrameReceiver = std::make_shared<mock::FrameReceiverMock>();

    dispatcher_.addFrameReceiver(frameReceiver, frameReceiverName);
    EXPECT_EQ(1, receivers.count(frameReceiverName));
    EXPECT_EQ(frameReceiver, receivers.at(frameReceiverName));

    dispatcher_.addFrameReceiver(secondFrameReceiver, secondFrameReceiverName);
    EXPECT_EQ(1, receivers.count(secondFrameReceiverName));
    EXPECT_EQ(secondFrameReceiver, receivers.at(secondFrameReceiverName));
    EXPECT_EQ(2, receivers.size());
}

TEST_F(DispatcherShould, RemoveFrameReceiver)
{
    const std::string& frameReceiverName = "FrameReceiver";
    const std::string& secondFrameReceiverName = "SecondFrameReceiver";

    const auto& receivers = dispatcher_.getFrameReceivers();
    auto frameReceiver = std::make_shared<mock::FrameReceiverMock>();
    auto secondFrameReceiver = std::make_shared<mock::FrameReceiverMock>();

    dispatcher_.addFrameReceiver(frameReceiver, frameReceiverName);
    EXPECT_EQ(1, receivers.count(frameReceiverName));
    EXPECT_EQ(frameReceiver, receivers.at(frameReceiverName));

    dispatcher_.addFrameReceiver(secondFrameReceiver, secondFrameReceiverName);
    EXPECT_EQ(1, receivers.count(secondFrameReceiverName));
    EXPECT_EQ(secondFrameReceiver, receivers.at(secondFrameReceiverName));
    EXPECT_EQ(2, receivers.size());

    dispatcher_.removeFrameReceiver("NotExist");
    EXPECT_EQ(2, receivers.size());

    dispatcher_.removeFrameReceiver(frameReceiverName);
    EXPECT_EQ(0, receivers.count(frameReceiverName));
    dispatcher_.removeFrameReceiver(secondFrameReceiverName);
    EXPECT_EQ(0, receivers.count(secondFrameReceiverName));
}

TEST_F(DispatcherShould, AddRawDataReceiver)
{
    const std::string& firstDataReceiverName = "FirstReceiver";
    const std::string& secondDataReceiverName = "SecondReceiver";
    auto firstReceiver = std::make_shared<mock::RawDataReceiverMock>();
    auto secondReceiver = std::make_shared<mock::RawDataReceiverMock>();

    const auto& receivers = dispatcher_.getRawDataReceivers();

    dispatcher_.addRawDataReceiver(firstReceiver, firstDataReceiverName);
    EXPECT_EQ(1, receivers.count(firstDataReceiverName));
    EXPECT_EQ(firstReceiver, receivers.at(firstDataReceiverName));
    dispatcher_.addRawDataReceiver(secondReceiver, secondDataReceiverName);
    EXPECT_EQ(1, receivers.count(secondDataReceiverName));
    EXPECT_EQ(secondReceiver, receivers.at(secondDataReceiverName));
}

TEST_F(DispatcherShould, RemoveRawDataReceiver)
{
    const std::string& firstDataReceiverName = "FirstReceiver";
    const std::string& secondDataReceiverName = "SecondReceiver";
    auto firstReceiver = std::make_shared<mock::RawDataReceiverMock>();
    auto secondReceiver = std::make_shared<mock::RawDataReceiverMock>();

    const auto& receivers = dispatcher_.getRawDataReceivers();

    dispatcher_.addRawDataReceiver(firstReceiver, firstDataReceiverName);
    dispatcher_.addRawDataReceiver(secondReceiver, secondDataReceiverName);
    EXPECT_EQ(1, receivers.count(firstDataReceiverName));
    EXPECT_EQ(1, receivers.count(secondDataReceiverName));

    EXPECT_EQ(2, receivers.size());
    dispatcher_.removeRawDataReceiver("notexist");
    EXPECT_EQ(2, receivers.size());

    dispatcher_.removeRawDataReceiver(firstDataReceiverName);
    EXPECT_EQ(0, receivers.count(firstDataReceiverName));
    dispatcher_.removeRawDataReceiver(secondDataReceiverName);
    EXPECT_EQ(0, receivers.count(secondDataReceiverName));
}

TEST_F(DispatcherShould, AddHandler)
{
    const std::string& firstHandlerName = "FirstHandler";
    const std::string& secondHandlerName = "SecondHandler";
    auto firstHandler = std::make_shared<mock::FrameHandlerMock>();
    auto secondHandler = std::make_shared<mock::FrameHandlerMock>();

    const auto& handlers = dispatcher_.getFrameHandlers();

    dispatcher_.addHandler(firstHandler, firstHandlerName);
    EXPECT_EQ(1, handlers.count(firstHandlerName));
    EXPECT_EQ(firstHandler, handlers.at(firstHandlerName));
    dispatcher_.addHandler(secondHandler, secondHandlerName);
    EXPECT_EQ(1, handlers.count(secondHandlerName));
    EXPECT_EQ(secondHandler, handlers.at(secondHandlerName));
}

TEST_F(DispatcherShould, RemoveHandler)
{
    const std::string& firstHandlerName = "FirstHandler";
    const std::string& secondHandlerName = "SecondHandler";
    auto firstHandler = std::make_shared<mock::FrameHandlerMock>();
    auto secondHandler = std::make_shared<mock::FrameHandlerMock>();

    const auto& handlers = dispatcher_.getFrameHandlers();

    dispatcher_.addHandler(firstHandler, firstHandlerName);
    dispatcher_.addHandler(secondHandler, secondHandlerName);
    EXPECT_EQ(1, handlers.count(firstHandlerName));
    EXPECT_EQ(1, handlers.count(secondHandlerName));

    EXPECT_EQ(2, handlers.size());
    dispatcher_.removeHandler("notexist");
    EXPECT_EQ(2, handlers.size());

    dispatcher_.removeHandler(firstHandlerName);
    EXPECT_EQ(0, handlers.count(firstHandlerName));
    dispatcher_.removeHandler(secondHandlerName);
    EXPECT_EQ(0, handlers.count(secondHandlerName));
}

TEST_F(DispatcherShould, ManageConnectionBetweenRawAndFrameReceiver)
{
    const std::string& firstDataReceiverName = "FirstReceiver";
    const std::string& secondDataReceiverName = "SecondReceiver";
    const std::string& firstFrameReceiverName = "FrameReceiver";
    const std::string& secondFrameReceiverName = "SecondFrameReceiver";

    auto firstDataReceiver = std::make_shared<mock::RawDataReceiverMock>();
    auto secondDataReceiver = std::make_shared<mock::RawDataReceiverMock>();
    auto firstFrameReceiver = std::make_shared<mock::FrameReceiverMock>();
    auto secondFrameReceiver = std::make_shared<mock::FrameReceiverMock>();

    dispatcher_.addFrameReceiver(firstFrameReceiver, firstFrameReceiverName);
    dispatcher_.addFrameReceiver(secondFrameReceiver, secondFrameReceiverName);
    dispatcher_.addRawDataReceiver(firstDataReceiver, firstDataReceiverName);
    dispatcher_.addRawDataReceiver(secondDataReceiver, secondDataReceiverName);

    const auto& connections = dispatcher_.getRawAndFrameConnections();

    {
        EXPECT_CALL(*firstDataReceiver, setHandler(_)).Times(1);
        dispatcher_.connectReceivers(firstDataReceiverName, firstFrameReceiverName);
    }

    EXPECT_EQ(1, connections.size());
    EXPECT_EQ(std::make_pair(firstDataReceiverName, firstFrameReceiverName), connections.front());

    EXPECT_CALL(*secondDataReceiver, setHandler(_)).Times(1);
    dispatcher_.connectReceivers(secondDataReceiverName, secondFrameReceiverName);

    EXPECT_EQ(2, connections.size());
    EXPECT_EQ(std::make_pair(secondDataReceiverName, secondFrameReceiverName), connections.back());

    EXPECT_CALL(*firstDataReceiver, setHandler(_)).Times(1);
    dispatcher_.removeFrameReceiver(firstFrameReceiverName);

    EXPECT_EQ(1, connections.size());
    EXPECT_EQ(std::make_pair(secondDataReceiverName, secondFrameReceiverName), connections.front());

    EXPECT_CALL(*secondDataReceiver, setHandler(_)).Times(1);
    dispatcher_.removeFrameReceiver(secondFrameReceiverName);

    EXPECT_EQ(0, connections.size());
}

TEST_F(DispatcherShould, ManageConnectionBetweenFrameReceiverAndHandler)
{
    const std::string& firstHandlerName = "FirstHandler";
    const std::string& secondHandlerName = "SecondHandler";
    const std::string& firstFrameReceiverName = "FrameReceiver";
    const std::string& secondFrameReceiverName = "SecondFrameReceiver";

    auto firstHandler = std::make_shared<mock::FrameHandlerMock>();
    auto secondHandler = std::make_shared<mock::FrameHandlerMock>();
    auto firstFrameReceiver = std::make_shared<mock::FrameReceiverMock>();
    auto secondFrameReceiver = std::make_shared<mock::FrameReceiverMock>();

    dispatcher_.addFrameReceiver(firstFrameReceiver, firstFrameReceiverName);
    dispatcher_.addFrameReceiver(secondFrameReceiver, secondFrameReceiverName);
    dispatcher_.addHandler(firstHandler, firstHandlerName);
    dispatcher_.addHandler(secondHandler, secondHandlerName);

    const auto& handlers = dispatcher_.getFrameAndHandlerConnections();

    EXPECT_CALL(*firstFrameReceiver, setHandler(_)).Times(1);
    dispatcher_.connectHandler(firstFrameReceiverName, firstHandlerName);

    EXPECT_EQ(1, handlers.size());
    EXPECT_EQ(std::make_pair(firstFrameReceiverName, firstHandlerName), handlers.front());

    EXPECT_CALL(*secondFrameReceiver, setHandler(_)).Times(1);
    dispatcher_.connectHandler(secondFrameReceiverName, secondHandlerName);
    EXPECT_EQ(2, handlers.size());
    EXPECT_EQ(std::make_pair(secondFrameReceiverName, secondHandlerName), handlers.back());

    EXPECT_CALL(*firstFrameReceiver, setHandler(_)).Times(1);
    dispatcher_.removeHandler(firstHandlerName);
    EXPECT_EQ(1, handlers.size());
    EXPECT_EQ(std::make_pair(secondFrameReceiverName, secondHandlerName), handlers.front());

    EXPECT_CALL(*secondFrameReceiver, setHandler(_)).Times(1);
    dispatcher_.removeHandler(secondHandlerName);
    EXPECT_EQ(0, handlers.size());
}

} // namespace handler

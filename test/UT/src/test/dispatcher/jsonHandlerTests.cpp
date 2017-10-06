// #include "dispatcher/jsonHandler.hpp"

// #include <memory>

// #include <gmock/gmock.h>
// #include <gtest/gtest.h>

// #include "matcher/messageIdCompare.hpp"
// #include "mock/messageHandlerMock.hpp"

// using namespace testing;

// namespace dispatcher
// {

// class JsonHandlerShould : public Test
// {
// public:
//     JsonHandlerShould()
//     {
//     }

//     JsonHandler handler_;
// };

// std::vector<u8> to_message(const std::string& msg)
// {
//     std::vector<u8> payload;
//     for (const auto& c : msg)
//     {
//         payload.push_back(static_cast<u8>(c));
//     }
//     return payload;
// }

// TEST_F(JsonHandlerShould, HandleMessage)
// {
//     auto handlerMock = std::make_unique<mock::MessageHandlerMock>();
//     mock::MessageHandlerMock* mockHandlerPtr = handlerMock.get();
//     const std::string messageId = "GET_STATUS";

//     handler_.addMessageHandler(messageId, std::move(handlerMock));

//     std::string jsonMessage = "{id:\"" + messageId + "\", data: \"some_data\"}";

//     auto payload = to_message(jsonMessage);

//     EXPECT_CALL(*mockHandlerPtr, onData(MessageIdCompare(messageId))).Times(1);
//     handler_.handleData(payload);
// }

// TEST_F(JsonHandlerShould, NotHandleUnregisteredMessage)
// {
//     auto handlerMock = std::make_unique<mock::MessageHandlerMock>();
//     mock::MessageHandlerMock* mockHandlerPtr = handlerMock.get();
//     const std::string messageId = "GET_STATUS";

//     handler_.addMessageHandler(messageId, std::move(handlerMock));

//     std::string jsonMessage = "{id:\"" + messageId + "NEW" + "\", data: \"some_data\"}";

//     auto payload = to_message(jsonMessage);

//     EXPECT_CALL(*mockHandlerPtr, onData(_)).Times(0);
//     handler_.handleData(payload);
// }

// } // namespace dispatcher

#include "handler/dispatcher.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "matcher/arrayCompare.hpp"

// #include <algorithm>
// #include <iterator>
// #include <memory>

// #include "catch.hpp"
// #include "fakeit.hpp"

// #include "matcher/bufferMatcher.hpp"

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


// struct InterfaceForTestingWriter
// {
//     virtual void doWrite(const u8* buffer, std::size_t length) = 0;
// };

struct WriterMock
{
    MOCK_METHOD2(doWrite, void(const u8*, std::size_t));
};

// struct TestingWriter
// {

// public:
//     TestingWriter(ReceiverForTest* receiver) : receiver_(receiver)
//     {
//     }

//     void expectWriteData(u8* expectedData, std::size_t expectedLength, const std::string& file,
//                          const std::string& line)
//     {
//         wasCalled_ = false;
//         expected_ = true;
//         expectedData_ = expectedData;
//         expectedLength_ = expectedLength;
//         file_ = file;
//         line_ = line;
//     }

//     void write(const u8* buf, std::size_t length)
//     {
//         if (!expected_)
//         {
//             FAIL("Unexpected call write");
//         }
//         wasCalled_ = false;
//         expected_ = true;
//         receiver_->readerCallback_(buf, length,
//                                    std::bind(&TestingWriter::writerCallback, this,
//                                              std::placeholders::_1, std::placeholders::_2));
//         REQUIRE(wasCalled_);
//     }

// private:
//     ReceiverForTest* receiver_;
//     bool expected_ = false;
//     bool wasCalled_ = true;
//     void writerCallback(const u8* buf, std::size_t length)
//     {
//         REQUIRE(length == expectedLength_);
//         CHECK_THAT(buf, test::matcher::BufferMatcher(expectedData_, length, file_, line_));
//         wasCalled_ = true;
//     }
//     u8* expectedData_;
//     std::size_t expectedLength_;
//     std::string file_;
//     std::string line_;
// };

// std::string trimPath(const std::string& path)
// {
//     return path.substr(path.find_last_of("/\\") + 1);
// }

// #define EXPECT_WRITER_DATA(writer, expectedAnswer)                                                 \
//     writer.expectWriteData(expectedAnswer, sizeof(expectedAnswer), trimPath(__FILE__),             \
//                            std::to_string(__LINE__))

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

    WriterMock writerMock;

    EXPECT_CALL(writerMock, doWrite(ArrayCompare(expectedAnswer, sizeof(expectedAnswer)), 1));

    receiver_->readerCallback_(msg, sizeof(msg), [&writerMock](const auto* buf, auto len) {
        writerMock.doWrite(buf, len);
    });

    // Verify(Method(writerMock, doWrite).Matching([expectedAnswer](const auto* buf, auto
    // length) {
    //     return std::equal(buf, buf + length, expectedAnswer);
    // }))
    //     .Exactly(Once);

    EXPECT_TRUE(handler_.transmissionStarted());
}

} // namespace handler

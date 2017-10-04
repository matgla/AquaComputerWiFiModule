#pragma nce

#include <map>
#include <queue>
#include <vector>

#include "dispatcher/IDataReceiver.hpp"
#include "dispatcher/IFrameHandler.hpp"
#include "dispatcher/handlers.hpp"
#include "logger/logger.hpp"
#include "utils/types.hpp"

namespace dispatcher
{

class MessageHandler : public IFrameHandler
{
public:
    MessageHandler();
    virtual ~MessageHandler();
    MessageHandler(const MessageHandler&) = delete;
    // MessageHandler(const MessageHandler&&) = delete;
    // MessageHandler& operator=(const MessageHandler&&) = delete;
    // MessageHandler& operator=(const MessageHandler&) = delete;

    bool transmissionStarted();
    u64 lengthToBeReceived();
    void onRead(const u8* buffer, std::size_t length, WriterCallback write) override;
    void send(const DataBuffer& data);
    void send(const std::string& data);
    void setConnection(IDataReceiver::RawDataReceiverPtr dataReceiver);

    virtual void handleData(const DataBuffer& data) = 0;

protected:
    void initializeTransmission(u8 data, WriterCallback& write);
    void receiveMessageLength(u8 data);
    void reply(const u8 answer, WriterCallback& write) const;

    bool messageLengthReceived_;
    bool transmissionStarted_;
    DataBuffer buffer_;

    u64 messageLengthToBeReceived_;
    u64 lengthToBeReceived_;

    std::queue<DataBuffer> transmissionBuffers_;
    IDataReceiver::RawDataReceiverPtr connection_;

    logger::Logger logger_;
};

} // namespace dispatcher

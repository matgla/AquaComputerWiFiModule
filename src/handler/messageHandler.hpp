#pragma once

#include <vector>

#include "handler/IDataHandler.hpp"
#include "handler/IFrameHandler.hpp"
#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace handler
{

class MessageHandler : public IFrameHandler
{
public:
    MessageHandler();
    virtual ~MessageHandler() = default;

    bool transmissionStarted();
    u64 lengthToBeReceived();
    void onRead(const u8* buffer, std::size_t length, WriterCallback write) override;
    void send(const DataBuffer& data);

    virtual void handleData(const DataBuffer& data) = 0;

protected:
    u8 initializeTransmission(u8 data);
    void receiveMessageLength(u8 data);
    void reply(const u8 answer, WriterCallback& write) const;

    bool messageLengthReceived_;
    bool transmissionStarted_;
    DataBuffer buffer_;
    u64 messageLengthToBeReceived_;
    u64 lengthToBeReceived_;
};

} // namespace handler

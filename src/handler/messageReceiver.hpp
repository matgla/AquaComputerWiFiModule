#pragma once

#include <vector>

#include "handler/IFrameReceiver.hpp"
#include "utils/types.hpp"

namespace handler
{

class MessageReceiver : public IFrameReceiver
{
public:
    MessageReceiver(DataHandler handler);

    bool transmissionStarted();
    u64 lengthToBeReceived();
    void onRead(const u8* buffer, std::size_t length, WriterCallback write) override;

protected:
    u8 initializeTransmission(u8 data);
    void receiveMessageLength(u8 data);
    void reply(const u8 answer, WriterCallback& write) const;

    bool messageLengthReceived_;
    bool transmissionStarted_;
    DataHandler handler_;
    std::vector<u8> buffer_;
    u64 messageLengthToBeReceived_;
    u64 lengthToBeReceived_;
};

} // namespace handler

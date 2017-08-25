#pragma once

#include <memory>
#include <vector>

#include "handler/IDataReceiver.hpp"
#include "utils/types.hpp"

namespace handler
{

class MessageReceiver
{
public:
    using ReceiverPtr = std::unique_ptr<IDataReceiver>;
    using DataHandler = std::function<void(std::vector<u8>)>;

    MessageReceiver(ReceiverPtr receiver, DataHandler handler);

    virtual ~MessageReceiver() = default;

    std::vector<u8>& getBuffer();
    IDataReceiver* getReceiver();

    bool transmissionStarted();
    u64 lengthToBeReceived();

protected:
    void receiveMessageLength(u8 data);
    u8 initializeTransmission(u8 data);
    void onRead(const u8* buffer, std::size_t length, WriterCallback write);
    void reply(const u8 answer, WriterCallback& write) const;


    bool messageLengthReceived_;
    bool transmissionStarted_;
    ReceiverPtr receiver_;
    DataHandler handler_;
    std::vector<u8> buffer_;
    u64 messageLengthToBeReceived_;
    u64 lengthToBeReceived_;
};

} // namespace handler

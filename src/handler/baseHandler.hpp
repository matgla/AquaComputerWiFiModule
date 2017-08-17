#pragma once

#include <memory>
#include <vector>

#include "handler/IDataReceiver.hpp"
#include "utils/types.hpp"

namespace handler
{

class BaseHandler
{
public:
    using ReceiverPtr = std::unique_ptr<IDataReceiver>;

    BaseHandler(ReceiverPtr receiver);

    virtual ~BaseHandler() = default;

    std::vector<u8>& getBuffer();
    IDataReceiver* getReceiver();

    bool transmissionStarted();
    bool allDataArrived();
    void transmissionStarted(bool transmissionStarted);
    void allDataArrived(bool transmissionStarted);
    u64 lengthToBeReceived();

protected:
    void receiveMessageLength(u8 data);
    u8 initializeTransmission(u8 data);
    void onRead(const u8* buffer, std::size_t length, WriterCallback write);
    void reply(const u8 answer, WriterCallback& write) const;

    bool messageLengthReceived_;
    bool transmissionStarted_;
    bool allDataArrived_;
    ReceiverPtr receiver_;
    std::vector<u8> buffer_;
    u64 messageLengthToBeReceived_;
    u64 lengthToBeReceived_;
};

} // namespace handler

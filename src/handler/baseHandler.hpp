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

protected:
    void onRead(const u8* buffer, std::size_t length, WriterCallback write);

    bool transmissionStarted_;
    bool allDataArrived_;
    ReceiverPtr receiver_;
    std::vector<u8> buffer_;
};

} // namespace handler

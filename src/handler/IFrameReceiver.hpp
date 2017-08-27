#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "handler/IDataReceiver.hpp"
#include "handler/handlers.hpp"
#include "utils/types.hpp"

namespace handler
{

class IFrameReceiver
{
public:
    using ReceiverPtr = std::shared_ptr<IFrameReceiver>;
    using DataHandler = std::function<void(std::vector<u8>)>;
    static void DefaultHandler(std::vector<u8>){};

    virtual ~IFrameReceiver() = default;
    virtual void onRead(const u8* buffer, std::size_t length, WriterCallback writer) = 0;
    virtual void setHandler(DataHandler handler) = 0;
};

} // namespace handler

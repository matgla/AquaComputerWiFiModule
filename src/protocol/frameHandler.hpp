#pragma once

#include <functional>
#include <map>

#include <boost/sml.hpp>

#include "dispatcher/IDataReceiver.hpp"
#include "dispatcher/IFrameHandler.hpp"
#include "logger/logger.hpp"
#include "protocol/IFrame.hpp"
#include "protocol/frame.hpp"
#include "protocol/messages/control.hpp"
#include "statemachine/helper.hpp"

#define FRAME_SIZE 255

namespace protocol
{

class FrameHandler
{
public:
    using FrameReceiver = std::function<void(const IFrame& frame)>;
    FrameHandler();
    ~FrameHandler();
    FrameHandler(const FrameHandler&) = delete;
    FrameHandler(const FrameHandler&&) = delete;
    FrameHandler& operator=(const FrameHandler&&) = delete;
    FrameHandler& operator=(const FrameHandler&) = delete;

    void setConnection(const dispatcher::IDataReceiver::RawDataReceiverPtr& dataReceiver);

    void send(const IFrame& frame);

    void connect(u16 port, const FrameReceiver& frameReceiver);

protected:
    enum class State
    {
        IDLE,
        LENGTH_TRANSMISSION,
        FRAME_NUMBER_TRANSMISSION,
        PORT_TRANSMISSION,
        CONTROL_TRANSMISSION,
        PAYLOAD_TRANSMISSION,
        CRC_TRANSMISSION,
        END_TRANSMISSION
    };

    void sendReply(messages::Control status);
    void onRead(const BufferSpan& buffer, const WriterCallback& writer);

    Frame<FRAME_SIZE> rxBuffer_;

    State state_;

    u8 rxCrcBytesReceived_;
    u8 rxLength_;
    u16 rxCrc_;

    logger::Logger logger_;

    dispatcher::IDataReceiver::RawDataReceiverPtr connection_;
    std::map<u16, FrameReceiver> receivers_;
};

} // namespace protocol

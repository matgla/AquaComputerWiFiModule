#pragma once

#include <functional>
#include <map>

#include <boost/sml.hpp>

#include "dispatcher/IDataReceiver.hpp"
#include "dispatcher/IFrameHandler.hpp"
#include "logger/logger.hpp"
#include "protocol/IFrame.hpp"
#include "protocol/frame.hpp"
#include "statemachine/helper.hpp"

#define FRAME_SIZE 255

namespace protocol
{

// handler ma obierać całe wiadomości
// pierwsza ramka to otrzymanie headera
// kolejne payload
// ostatnia checksuma

class FrameHandler
{
public:
    using FrameReceiver = std::function<void(const IFrame& frame)>;
    FrameHandler();
    ~FrameHandler();

    void setConnection(dispatcher::IDataReceiver::RawDataReceiverPtr dataReceiver);

    void onRead(const u8* buffer, std::size_t length);
    void send(const IFrame& data);

    void connect(u16 port, FrameReceiver frameReceiver);

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

    Frame<FRAME_SIZE> rxBuffer_;

    State state_;
    bool rxTransmissionOngoing_;
    bool rxLengthKnown_;
    bool rxNumberReceived_;
    bool rxControlByteReceived_;
    bool rxPortReceived_;
    bool rxHeaderReceived_;
    u8 rxCrcBytesReceived_;
    u8 rxLength_;
    u16 rxCrc_;

    logger::Logger logger_;

    dispatcher::IDataReceiver::RawDataReceiverPtr connection_;
    std::map<u16, FrameReceiver> receivers_;
};

} // namespace protocol

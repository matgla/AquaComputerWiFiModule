#pragma once

#include <vector>

#include "logger/logger.hpp"
#include "protocol/IFrame.hpp"
#include "protocol/frameHandler.hpp"
#include "timer/IManager.hpp"
#include "utils/types.hpp"

#define PACKET_SIZE 255

namespace protocol
{

struct TransmissionFrame
{
    explicit TransmissionFrame(IFrame::FramePtr frame) : frame(std::move(frame)), confirmed(false)
    {
    }

    IFrame::FramePtr frame;
    bool confirmed;
};

class PacketHandler
{
public:
    PacketHandler(const u16 port, const dispatcher::IDataReceiver::RawDataReceiverPtr receiver,
                  timer::IManager& timerManager);
    void send(const DataBuffer& data);

protected:
    void onFrame(const IFrame& frame);
    void transmit();
    FrameHandler handler_;
    std::vector<std::pair<DataBuffer, bool>> rxPacketBuffers_;
    std::vector<std::vector<TransmissionFrame>> txPacketBuffers_;
    u8 txIndex_;

    u16 port_;
    u8 rxMessageNumber_;
    u8 txMessageNumber_;
    logger::Logger logger_;
    timer::IManager& timerManager_;
    timer::ITimer::TimerPtr txTimeout_;
};

} // namespace protocol

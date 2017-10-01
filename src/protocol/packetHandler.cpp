#include "protocol/packetHandler.hpp"

#include <functional>

#include <CRC.h>

#include "dispatcher/IDataReceiver.hpp"
#include "protocol/frame.hpp"
#include "protocol/messages/control.hpp"
#include "serializer/serializer.hpp"
#include "timer/IManager.hpp"

namespace protocol
{


PacketHandler::PacketHandler(const u16 port,
                             const dispatcher::IDataReceiver::RawDataReceiverPtr receiver,
                             timer::IManager& timerManager)
    : port_(port), rxMessageNumber_(0), txMessageNumber_(0), logger_("packetHandler"),
      timerManager_(timerManager)
{
    handler_.setConnection(receiver);
    handler_.connect(port, std::bind(&PacketHandler::onFrame, this, std::placeholders::_1));
}


void PacketHandler::send(const DataBuffer& data)
{
    if (data.size() >
        62738) // 254 (max frames - start and control frame) * 247 (max payload in frame)
    {
        logger_.error() << "Packet size above max packet size";
        return;
    }
    u8 frameNumber = 0;
    IFrame::FramePtr header(new Frame<6>());
    header->port(port_);
    header->number(frameNumber);
    header->control(messages::Control::Transmission);

    u8 headerData[6];
    serializer::serialize(headerData, static_cast<u16>(data.size()));
    headerData[2] = 0;
    headerData[3] = 0;

    const u16 crc = CRC::Calculate(headerData, 4, CRC::CRC_16_ARC());
    serializer::serialize(&headerData[4], crc);

    header->payload(headerData, sizeof(headerData));

    txPacketBuffers_.push_back(std::vector<TransmissionFrame>{});
    txPacketBuffers_.back().push_back(TransmissionFrame{std::move(header)});

    for (auto size = 0; size < data.size(); size += MaxPayloadSize)
    {
        IFrame::FramePtr frame(new Frame<>());
        frame->port(port_);
        frame->number(++frameNumber);
        frame->control(messages::Control::Transmission);
        u8 payloadSize = 0;
        if (data.size() - size > MaxPayloadSize)
        {
            payloadSize = MaxPayloadSize;
        }
        else
        {
            payloadSize = data.size() - size;
        }
        logger_.debug() << "Created frame with size: " << static_cast<int>(payloadSize);
        frame->payload(data.data() + size, payloadSize);
        txPacketBuffers_.back().push_back(TransmissionFrame{std::move(frame)});
    }
    const u8 crcSize = 4;

    IFrame::FramePtr crcFrame(new Frame<crcSize>());
    crcFrame->port(port_);
    crcFrame->number(++frameNumber);
    crcFrame->control(messages::Control::Transmission);
    u8 crcPayload[crcSize];
    serializer::serialize(crcPayload, CRC::Calculate(data.data(), data.size(), CRC::CRC_32()));

    crcFrame->payload(crcPayload, sizeof(crcPayload));
    txPacketBuffers_.back().push_back(TransmissionFrame{std::move(crcFrame)});

    txIndex_ = 0;
    transmit();
}

void PacketHandler::onFrame(const IFrame& frame)
{
    logger_.info() << "Received frame " << std::to_string(frame.number());
    if (frame.control() == messages::Control::Success)
    {
        auto& currentFrame = txPacketBuffers_.front().at(txIndex_);
        logger_.info() << "It's ack frame for: " << currentFrame.frame->number();

        // TODO: Test na to
        if (currentFrame.frame->number() == frame.number())
        {
            currentFrame.confirmed = true;
            txTimeout_->cancel();
            ++txIndex_;
            transmit();
        }
    }
}

void PacketHandler::transmit()
{
    logger_.debug() << "Transmitting frame: " << std::to_string(txIndex_);
    auto& frame = txPacketBuffers_.front().at(txIndex_);

    if (!frame.confirmed)
    {
        handler_.send(*frame.frame);
        txTimeout_ = timerManager_.setTimeout(400, std::bind(&PacketHandler::transmit, this));
    }
}
} // namespace protocol

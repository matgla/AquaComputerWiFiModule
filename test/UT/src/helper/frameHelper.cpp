#include "helper/frameHelper.hpp"

#include <cassert>

#include "protocol/frame.hpp"
#include "protocol/messages/control.hpp"
#include "serializer/serializer.hpp"

namespace helper
{

DataBuffer createFrame(const DataBuffer& data, const u8 port, const u8 frameNumber,
                       const int startIndex)
{
    assert(startIndex < data.size() && "StartIndex overflows data");
    u8 frameSize = 0;
    if (data.size() - startIndex > protocol::MaxPayloadSize)
    {
        frameSize = protocol::MaxPayloadSize;
    }
    else
    {
        frameSize = data.size() - startIndex;
    }

    DataBuffer frame = {protocol::FrameByte::Start, frameSize, frameNumber, port,
                        protocol::messages::Control::Transmission};

    frame.insert(frame.end(), data.begin() + startIndex, data.begin() + startIndex + frameSize);
    frame.resize(frame.size() + 2);
    serializer::serialize(frame.data() + frame.size() - 2,
                          CRC::Calculate(data.data() + startIndex, frameSize, CRC::CRC_16_ARC()));
    frame.push_back(protocol::FrameByte::End);

    return frame;
}

DataBuffer createHeader(const DataBuffer& data, const u8 port, const u8 messageNumber,
                        const u8 frameNumber)
{
    const u8 reservedByte = 0;
    const u8 controlByte = protocol::messages::Control::Transmission;
    const u8 messageHeaderSize = 6;
    DataBuffer header;

    const u16 messageLength = data.size();
    header.resize(2);
    serializer::serialize(header.data(), messageLength);

    header.push_back(messageNumber);
    header.push_back(reservedByte);
    header.resize(header.size() + 2);

    serializer::serialize(header.data() + 4, CRC::Calculate(header.data(), 4, CRC::CRC_16_ARC()));

    DataBuffer expectedFrame = {protocol::FrameByte::Start, messageHeaderSize, frameNumber, port,
                                controlByte};

    expectedFrame.insert(expectedFrame.end(), header.begin(), header.end());

    // CRC for 0 payload = 0x0 0x0
    expectedFrame.push_back(0);
    expectedFrame.push_back(0);
    expectedFrame.push_back(protocol::FrameByte::End);

    return expectedFrame;
}

DataBuffer createAck(const u8 port, const u8 frameNumber)
{
    const u8 payloadSize = 0;
    const u8 crc = 0;

    DataBuffer ack = {
        protocol::FrameByte::Start,           payloadSize, frameNumber, port,
        protocol::messages::Control::Success, crc,         crc,         protocol::FrameByte::End};

    return ack;
}

DataBuffer createCrc32Frame(const u8 port, const u8 frameNumber, const DataBuffer& data)
{
    DataBuffer crcFrame = {protocol::FrameByte::Start, 4, frameNumber, port,
                           protocol::messages::Control::Transmission};
    crcFrame.resize(crcFrame.size() + 4);
    serializer::serialize(crcFrame.data() + 5,
                          CRC::Calculate(data.data(), data.size(), CRC::CRC_32()));
    crcFrame.resize(crcFrame.size() + 2);
    serializer::serialize(crcFrame.data() + 9,
                          CRC::Calculate(crcFrame.data() + 5, 4, CRC::CRC_16_ARC()));
    crcFrame.push_back(protocol::FrameByte::End);
    return crcFrame;
}
} // namespace helper
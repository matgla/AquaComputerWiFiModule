#pragma once

#include <array>
#include <cstring>
#include <limits>

#include <CRC.h>

#include "protocol/IFrame.hpp"
#include "utils/types.hpp"

namespace protocol
{

enum FrameByte : u8
{
    Start = 0xaa,
    End = 0x11
};

enum Control : u8
{
    Success = 0x20,
    PortNotConnect,
    CrcChecksumFailed,
    WrongEndByte
};

template <u16 PAYLOAD_SIZE = 255>
class Frame : public IFrame
{
    static_assert((PAYLOAD_SIZE <= std::numeric_limits<u8>::max()), "Payload size must be < 256");

public:
    using PayloadContainer = std::array<u8, PAYLOAD_SIZE>;

    Frame(u8 port = 0, u8 number = 0) : port_(port), number_(number), length_(0)
    {
    }

    void port(u8 port) override
    {
        port_ = port;
    }

    u8 port() const override
    {
        return port_;
    }

    void number(u8 number) override
    {
        number_ = number;
    }

    u8 number() const override
    {
        return number_;
    }

    void control(u8 control) override
    {
        control_ = control;
    }

    u8 control() const override
    {
        return control_;
    }

    void clear() override
    {
        port_ = 0;
        number_ = 0;
        length_ = 0;
        control_ = 0;
    }

    u8 payload(const u8* data, u8 length) override
    {
        if (length + length_ > PAYLOAD_SIZE)
        {
            length = PAYLOAD_SIZE - length_;
        }

        std::memcpy(payload_.data(), data, length);
        length_ += length;
        return length;
    }

    const u8* payload() const override
    {
        return payload_.data();
    }

    u8 length() const override
    {
        return length_;
    }

    u16 crc() const override
    {
        return CRC::Calculate(payload_.data(), length_, CRC::CRC_16_ARC());
    }

    u8 payloadSize() const override
    {
        return PAYLOAD_SIZE;
    }

private:
    u8 port_;
    u8 number_;
    u8 length_;
    u8 control_;
    PayloadContainer payload_;
};

} // namespace protocol

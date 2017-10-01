#pragma once

#include <memory>

#include "utils/types.hpp"

namespace protocol
{

class IFrame
{
public:
    using FramePtr = std::unique_ptr<IFrame>;

    virtual ~IFrame() = default;

    virtual void port(u8 port) = 0;
    virtual u8 port() const = 0;
    virtual void number(u8 number) = 0;
    virtual u8 number() const = 0;
    virtual void control(u8 control) = 0;
    virtual u8 control() const = 0;
    virtual void clear() = 0;
    virtual u8 payload(const u8* data, u8 length) = 0;
    virtual const u8* payload() const = 0;
    virtual u8 length() const = 0;
    virtual u16 crc() const = 0;
    virtual u8 payloadSize() const = 0;
};

} // namespace protocol

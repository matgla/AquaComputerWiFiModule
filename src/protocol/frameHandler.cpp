#include "protocol/frameHandler.hpp"

namespace protocol
{

FrameHandler::FrameHandler()
    : logger_("FrameHandler"), rxTransmissionOngoing_(false), rxLengthKnown_(false),
      rxNumberReceived_(false), rxControlByteReceived_(false), rxPortReceived_(false),
      rxHeaderReceived_(false), rxCrcBytesReceived_(false), rxLength_(0), rxCrc_(0)
{
}

FrameHandler::~FrameHandler()
{
    if (connection_)
    {
        connection_->setHandler(&dispatcher::defaultReader);
    }
}

void FrameHandler::setConnection(dispatcher::IDataReceiver::RawDataReceiverPtr dataReceiver)
{
    connection_ = dataReceiver;
    dataReceiver->setHandler(
        std::bind(&FrameHandler::onRead, this, std::placeholders::_1, std::placeholders::_2));
    logger_.info() << "Connection set";
}

void FrameHandler::onRead(const u8* buffer, std::size_t length)
{
    logger_.info() << "OnRead";
    u8 frameBytesToBeReceived = 0;
    // receive payload
    if (rxLength_ != 0 && rxHeaderReceived_)
    {
        logger_.info() << "Data Receive";
        frameBytesToBeReceived = length > rxLength_ ? rxLength_ : length;
        if (frameBytesToBeReceived != rxBuffer_.payload(buffer, frameBytesToBeReceived))
        {
            logger_.error() << "Received more bytes than frame max size";
            Frame<0> frame;
            frame.port(rxBuffer_.port());
            frame.number(rxBuffer_.number());
            frame.control(Control::FramingError);
            send(frame);
        }
        rxLength_ -= frameBytesToBeReceived;
    }

    for (int i = 0 + frameBytesToBeReceived; i < length; ++i)
    {
        // start byte
        if (!rxTransmissionOngoing_ && FrameByte::Start == buffer[i])
        {
            rxTransmissionOngoing_ = true;
            rxLengthKnown_ = false;
            rxNumberReceived_ = false;
            rxPortReceived_ = false;
            rxLength_ = 0;
            rxCrcBytesReceived_ = 0;
            rxCrc_ = 0;
            rxControlByteReceived_ = false;
            rxHeaderReceived_ = false;
            rxBuffer_.clear();
            logger_.info() << "Transmission started";
            continue;
        }
        // length byte
        if (rxTransmissionOngoing_ && !rxLengthKnown_)
        {
            rxLength_ = buffer[i];
            rxLengthKnown_ = true;
            logger_.info() << "Length received" << std::to_string(rxLength_);
            continue;
        }
        // part number byte
        if (rxTransmissionOngoing_ && !rxNumberReceived_)
        {
            rxBuffer_.number(buffer[i]);
            rxNumberReceived_ = true;
            logger_.info() << "part received";
            continue;
        }
        // port byte
        if (rxTransmissionOngoing_ && !rxPortReceived_)
        {
            rxBuffer_.port(buffer[i]);
            rxPortReceived_ = true;
            logger_.info() << "port received";
            continue;
        }
        // control byte
        if (rxTransmissionOngoing_ && !rxControlByteReceived_)
        {
            rxBuffer_.control(buffer[i]);
            rxControlByteReceived_ = true;
            rxHeaderReceived_ = true;
            logger_.info() << "controll received";
            onRead(buffer + i, length - i);
            break;
        }
        // crc bytes
        if (rxTransmissionOngoing_ && rxLength_ == 0 && rxCrcBytesReceived_ < 2)
        {
            rxCrc_ |= buffer[i] << (8 * (1 - rxCrcBytesReceived_));
            ++rxCrcBytesReceived_;
            logger_.info() << "Crc received";

            continue;
        }
        // end byte
        if (rxTransmissionOngoing_ && rxLength_ == 0 && rxCrcBytesReceived_ == 2)
        {
            // jesli crc jest ok i otrzymano bajt konca
            if (rxBuffer_.crc() == rxCrc_ && buffer[i] == FrameByte::End)
            {
                if (0 == receivers_.count(rxBuffer_.port()))
                {
                    logger_.error() << "Handler for port " << std::to_string(rxBuffer_.port())
                                    << " not exists.";
                    Frame<0> frame;
                    frame.port(rxBuffer_.port());
                    frame.number(rxBuffer_.number());
                    frame.control(Control::PortNotConnect);
                    send(frame);
                }
                else
                {
                    logger_.info() << "Data received";

                    Frame<0> frame;
                    frame.port(rxBuffer_.port());
                    frame.number(rxBuffer_.number());
                    frame.control(Control::Success);
                    send(frame);
                    receivers_.at(rxBuffer_.port())(rxBuffer_);
                }
            }
            continue;
        }
    }
    if (rxLength_ != 0 && rxHeaderReceived_)
    {
        logger_.info() << "Data Receive";
        frameBytesToBeReceived = length > rxLength_ ? rxLength_ : length;
        if (frameBytesToBeReceived != rxBuffer_.payload(buffer, frameBytesToBeReceived))
        {
            logger_.error() << "Received more bytes than frame max size";
            Frame<0> frame;
            frame.port(rxBuffer_.port());
            frame.number(rxBuffer_.number());
            frame.control(Control::FramingError);
            send(frame);
        }
        rxLength_ -= frameBytesToBeReceived;
    }
}

void FrameHandler::send(const IFrame& frame)
{
    if (!connection_)
    {
        logger_.error() << "Trying to send message while connection aren't set";
    }

    connection_->write(FrameByte::Start);
    connection_->write(frame.length());
    connection_->write(frame.number());
    connection_->write(frame.port());
    connection_->write(frame.control());
    connection_->write(frame.payload(), frame.length());
    connection_->write(frame.crc());
    connection_->write(FrameByte::End);
}


} // namespace protocol

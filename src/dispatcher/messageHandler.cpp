#include "dispatcher/messageHandler.hpp"

#include <functional>

#include <sstream>

#include "message/messages.hpp"
#include "serializer/serializer.hpp"

namespace dispatcher
{

const u8 LENGTH_SIZE = 8; // bytes

MessageHandler::MessageHandler()
    : transmissionStarted_(false), lengthToBeReceived_(0), messageLengthReceived_(false),
      messageLengthToBeReceived_(0), logger_("MessageHandler")
{
}

MessageHandler::~MessageHandler()
{
    if (connection_)
    {
        connection_->setHandler(&defaultReader);
    }
}

bool MessageHandler::transmissionStarted()
{
    return transmissionStarted_;
}

u64 MessageHandler::lengthToBeReceived()
{
    return messageLengthToBeReceived_;
}

void MessageHandler::receiveMessageLength(u8 data)
{
    messageLengthToBeReceived_ |= static_cast<u64>(data) << 8 * (LENGTH_SIZE - lengthToBeReceived_);
    --lengthToBeReceived_;
    if (0 == lengthToBeReceived_)
    {
        messageLengthReceived_ = true;
        logger_.info() << "to be received: " << std::to_string(messageLengthToBeReceived_);
        buffer_.reserve(messageLengthToBeReceived_);
    }
}

void MessageHandler::initializeTransmission(u8 data, WriterCallback& write)
{
    if (message::TransmissionId::Start == data)
    {
        transmissionStarted_ = true;
        messageLengthReceived_ = false;
        lengthToBeReceived_ = LENGTH_SIZE;
        messageLengthToBeReceived_ = 0;

        reply(message::TransmissionId::Ack, write);
        return;
    }

    if (message::TransmissionId::Ack == data) // transmission accepted, send queued message
    {
        DataBuffer buffer = transmissionBuffers_.front();
        transmissionBuffers_.pop();

        const u8 messageLengthSize = 8;
        u8 length[messageLengthSize];
        serializer::serialize(length, buffer.size());
        write(length, messageLengthSize);
        write(buffer.data(), buffer.size());
        return;
    }

    reply(message::TransmissionId::Nack, write);
}

void MessageHandler::onRead(const u8* buffer, std::size_t length, WriterCallback write)
{
    logger_.info() << "on read";
    for (std::size_t i = 0; i < length; ++i)
    {
        std::stringstream str;
        str << std::hex << buffer[i];
        logger_.info() << "b: " << buffer[i] << "(" << std::to_string(buffer[i]) << ") = 0x"
                       << str.str();
        if (!transmissionStarted_)
        {
            initializeTransmission(buffer[i], write);
            continue;
        }

        if (!messageLengthReceived_)
        {
            receiveMessageLength(buffer[i]);
            continue;
        }

        buffer_.push_back(buffer[i]);

        if (0 == --messageLengthToBeReceived_)
        {
            // Received whole message
            transmissionStarted_ = false;
            handleData(buffer_);
            buffer_.clear();
        }
    }
}

void MessageHandler::reply(const u8 answer, WriterCallback& write) const
{
    u8 answerMessage[] = {answer};
    write(answerMessage, sizeof(answerMessage));
}

void MessageHandler::setConnection(IDataReceiver::RawDataReceiverPtr dataReceiver)
{
    connection_ = dataReceiver;
    dataReceiver->setHandler(std::bind(&MessageHandler::onRead, this, std::placeholders::_1,
                                       std::placeholders::_2, std::placeholders::_3));
    logger_.info() << "Connection set";
}

void MessageHandler::send(const DataBuffer& data)
{
    if (!connection_)
    {
        logger_.error() << "Trying to send message while connection aren't set";
    }

    if (transmissionStarted_)
    {
        logger_.debug() << "Transmission ongoing";
        return;
    }

    connection_->write(message::TransmissionId::Start);
    transmissionBuffers_.push(data);
    logger_.info() << "Pushed to buffer: " << data.size();
}

void MessageHandler::send(const std::string& data)
{
    DataBuffer buffer;
    std::copy(data.begin(), data.end(), std::back_inserter(buffer));
    send(buffer);
}

} // namespace dispatcher

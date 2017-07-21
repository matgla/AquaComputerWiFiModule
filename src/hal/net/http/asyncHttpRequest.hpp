#pragma once

#include <functional>
#include <memory>
#include <string>

#include "logger/Logger.hpp"
#include "utils/types.hpp"

namespace net
{
namespace http
{

class AsyncHttpRequest
{
public:
    using SendCallback = std::function<void(u16 code, const std::string& type,
                                            const std::string& body)>;

    AsyncHttpRequest()
        : logger_("AsyncHttpRequest"),
          sendCallback_(nullptr)
    {
    }

    ~AsyncHttpRequest()
    {
    }

    void setSendCallback(SendCallback sendCallback)
    {
        sendCallback_ = sendCallback;
    }

    void send(u16 code, const std::string& type, const std::string& msg)
    {
        if (nullptr == sendCallback_)
        {
            logger_.err() << "Callback for send hasn't been set for msg: " << msg << "\n";
            return;
        }
        sendCallback_(code, type, msg);
    }


private:
    logger::Logger logger_;

    SendCallback sendCallback_;
};

} // namespace http
} // namespace net

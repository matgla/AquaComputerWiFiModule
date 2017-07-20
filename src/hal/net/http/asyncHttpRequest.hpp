#pragma once

#include <memory>
#include <string>

#include "utils/types.hpp"

namespace net
{
namespace http
{

class AsyncHttpRequest
{
public:
    AsyncHttpRequest()
        : code_(0),
          type_(""),
          msg_("")
    {
    }

    ~AsyncHttpRequest()
    {
    }
    void send(u16 code, const std::string& type, const std::string& msg)
    {
        code_ = code;
        type_ = type;
        msg_ = msg;
    }
    const std::string& getType()
    {
        return type_;
    }
    const std::string& getMsg()
    {
        return msg_;
    }
    u16 getCode()
    {
        return code_;
    }

private:
    u16 code_;
    std::string type_;
    std::string msg_;
    // class AsyncHttpRequestWrapper;
    //  std::unique_ptr<AsyncHttpRequestWrapper> asyncHttpReqWrapper_;
};

} // namespace http
} // namespace net

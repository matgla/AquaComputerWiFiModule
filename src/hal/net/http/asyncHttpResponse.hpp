#pragma once

#include <functional>
#include <memory>
#include <string>

#include "logger/logger.hpp"
#include "utils/types.hpp"

namespace net
{
namespace http
{

class AsyncHttpResponse
{
public:
    AsyncHttpResponse()
        : logger_("AsyncHttpResponse")
    {
    }

    ~AsyncHttpResponse()
    {
    }

    u16 getCode()
    {
        return code_;
    }

    std::string getType()
    {
        return type_;
    }

    std::string& getMsg()
    {
        return msg_;
    }

    void setCode(u16 code)
    {
        code_ = code;
    }

    void setType(const std::string& type)
    {
        type_ = type;
    }

    void setMsg(const std::string& msg)
    {
        msg_ = msg;
    }

private:
    logger::Logger logger_;
    u16 code_;
    std::string type_;
    std::string msg_;
};

} // namespace http
} // namespace net

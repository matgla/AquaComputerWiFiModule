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
    AsyncHttpResponse() : logger_("AsyncHttpResponse"), code_{0}
    {
    }

    ~AsyncHttpResponse() = default;
    AsyncHttpResponse(const AsyncHttpResponse&) = delete;
    AsyncHttpResponse(const AsyncHttpResponse&&) = delete;
    AsyncHttpResponse& operator=(const AsyncHttpResponse&& other) = delete;
    AsyncHttpResponse& operator=(const AsyncHttpResponse& other) = delete;

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

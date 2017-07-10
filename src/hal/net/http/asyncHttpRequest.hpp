#pragma once

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
    {
    }
    ~AsyncHttpRequest()
    {
    }
    void send(u16 code, const std::string& type, const std::string& msg)
    {
    }

private:
    class AsyncHttpRequestWrapper;
    std::unique_ptr<AsyncHttpRequestWrapper> asyncHttpReqWrapper_;
};

} // namespace http
} // namespace net

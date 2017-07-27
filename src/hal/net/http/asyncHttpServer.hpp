#pragma once

#include <functional>
#include <memory>
#include <string>

#include "hal/net/http/fwd.hpp"
#include "utils/types.hpp"

namespace net
{
namespace http
{

using RequestHandler = std::function<void(AsyncHttpRequest*)>;

class AsyncHttpServer
{
public:
    AsyncHttpServer(u16 port);
    ~AsyncHttpServer();
    void get(const std::string& uri, RequestHandler req);
    void post(const std::string& uri, RequestHandler req);
    void begin();

private:
    u16 port_;

    class AsyncHttpWrapper;
    std::unique_ptr<AsyncHttpWrapper> asyncHttpWrapper_;
};

} // namespace http
} // namespace net

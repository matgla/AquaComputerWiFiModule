#include "hal/net/http/asyncHttpServer.hpp"

#include <beast/core.hpp>
#include <beast/http.hpp>
#include <beast/version.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "hal/net/http/asyncHttpRequest.hpp"
#include "hal/x86/net/http/httpConnection_x86.hpp"

#include "logger/logger.hpp"

using namespace boost;
using namespace boost::asio;

namespace net
{
namespace http
{

auto logger = logger::Logger("HttpServer");

using Handlers = std::map<std::string, RequestHandler>;

class AsyncHttpServer::AsyncHttpWrapper
{
public:
    AsyncHttpWrapper(const std::string& address, u16 port)
        : acceptor_(ioService_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          socket_(ioService_)
    {
    }
    void begin();

    std::map<std::string, RequestHandler> getHandlers_;
    std::map<std::string, RequestHandler> postHandlers_;

private:
    io_service ioService_;
    AsyncHttpWrapper() = delete;
    void loop();
    ip::tcp::acceptor acceptor_;
    ip::tcp::socket socket_;
};

void AsyncHttpServer::AsyncHttpWrapper::begin()
{
    loop();
    ioService_.run();
}

void AsyncHttpServer::AsyncHttpWrapper::loop()
{
    acceptor_.async_accept(socket_, [&](beast::error_code ec) {
        if (!ec)
        {
            std::make_shared<HttpConnection>(std::move(socket_), getHandlers_, postHandlers_)->start();
        }
        loop();
    });
}

AsyncHttpServer::AsyncHttpServer(u16 port)
    : port_(port), asyncHttpWrapper_(new AsyncHttpWrapper("127.0.0.1", port))
{
}

AsyncHttpServer::~AsyncHttpServer() = default;


void AsyncHttpServer::get(const std::string& uri, RequestHandler handler)
{
    asyncHttpWrapper_->getHandlers_[uri] = handler;
}

void AsyncHttpServer::post(const std::string& uri, RequestHandler handler)
{
    asyncHttpWrapper_->postHandlers_[uri] = handler;
}

void AsyncHttpServer::begin()
{
    logger.info() << "Starting....\n";
    asyncHttpWrapper_->begin();
}

} // namespace http
} // namespace net
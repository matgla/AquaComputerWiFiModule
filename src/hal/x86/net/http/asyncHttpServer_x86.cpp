#include "hal/net/http/asyncHttpServer.hpp"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>

#include "hal/net/http/asyncHttpRequest.hpp"
#include "hal/x86/net/http/httpConnection_x86.hpp"

#include "logger/logger.hpp"

using namespace boost;
using namespace boost::asio;

namespace net
{
namespace http
{

using Handlers = std::map<std::string, RequestHandler>;

class AsyncHttpServer::AsyncHttpWrapper
{
public:
    AsyncHttpWrapper(u16 port)
        : acceptor_(ioService_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          socket_(ioService_), logger_("HttpServer")
    {
    }
    AsyncHttpWrapper() = delete;

    void begin();

    std::map<std::string, RequestHandler> getHandlers_;
    std::map<std::string, RequestHandler> postHandlers_;
    logger::Logger logger_;

private:
    io_service ioService_;
    void loop();
    ip::tcp::acceptor acceptor_;
    ip::tcp::socket socket_;
};

void AsyncHttpServer::AsyncHttpWrapper::begin()
{
    std::thread{std::bind(&AsyncHttpServer::AsyncHttpWrapper::loop, this)}.detach();
}

void AsyncHttpServer::AsyncHttpWrapper::loop()
{
    while (true)
    {
        acceptor_.accept(socket_);
        std::make_shared<HttpConnection>(std::move(socket_), getHandlers_, postHandlers_)->start();
    }
}

AsyncHttpServer::AsyncHttpServer(u16 port)
    : port_(port), asyncHttpWrapper_(new AsyncHttpWrapper(port))
{
}

AsyncHttpServer::~AsyncHttpServer() = default;


void AsyncHttpServer::get(const std::string& uri, RequestHandler req)
{
    asyncHttpWrapper_->getHandlers_[uri] = std::move(req);
}

void AsyncHttpServer::post(const std::string& uri, RequestHandler req)
{
    asyncHttpWrapper_->postHandlers_[uri] = std::move(req);
}

void AsyncHttpServer::begin()
{
    asyncHttpWrapper_->logger_.info() << "Starting....";
    asyncHttpWrapper_->begin();
}

void waitForBreak()
{
    boost::asio::io_service ios{1};
    boost::asio::signal_set signals(ios, SIGINT, SIGTERM);
    signals.async_wait([&](boost::system::error_code const&, int) {});
    ios.run();
}


} // namespace http
} // namespace net

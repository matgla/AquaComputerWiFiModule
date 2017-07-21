#pragma once

#include <beast/core.hpp>
#include <beast/http.hpp>
#include <boost/asio.hpp>

#include <map>
#include <memory>
#include <string>

#include "hal/net/http/asyncHttpServer.hpp"
#include "logger/logger.hpp"

namespace net
{
namespace http
{
using Handlers = std::map<std::string, RequestType>;

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
    HttpConnection(boost::asio::ip::tcp::socket socket,
                   Handlers& handlers);

    void start();

private:
    void getCallback(u16 code, const std::string& type, const std::string& body);
    void readRequest();
    void processRequest();
    void createGetResponse();
    void writeResponse();

    boost::asio::ip::tcp::socket socket_;

    beast::flat_buffer buffer_{8192};
    beast::http::request<beast::http::dynamic_body> request_;
    beast::http::response<beast::http::dynamic_body> response_;

    Handlers& getHandlers_;
    logger::Logger logger_;
};

} // namespace http
} // namespace net

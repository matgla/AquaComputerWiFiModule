#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <map>
#include <memory>
#include <string>

#include "hal/net/http/asyncHttpRequest.hpp"
#include "hal/net/http/asyncHttpServer.hpp"
#include "logger/logger.hpp"

namespace net
{
namespace http
{
using Handlers = std::map<std::string, RequestHandler>;

class HttpConnection : public std::enable_shared_from_this<HttpConnection>
{
public:
    HttpConnection(boost::asio::ip::tcp::socket socket, Handlers& getHandlers,
                   Handlers& postHandlers);

    void start();

private:
    void getCallback(u16 code, const std::string& type, const std::string& body);
    void readRequest();
    void processRequest(boost::beast::error_code& ec);
    void createGetResponse();
    void writeResponse(boost::beast::error_code& ec);
    void handlePost();
    std::string getBodyCallback();
    std::unique_ptr<AsyncHttpResponse>
        chunkedResponseCallback(const std::string& type,
                                const AsyncHttpRequest::ChunkedResponseParseCallback& callback);

    boost::asio::ip::tcp::socket socket_;

    boost::beast::flat_buffer buffer_{8192};
    boost::beast::http::request<boost::beast::http::dynamic_body> request_;
    boost::beast::http::response<boost::beast::http::dynamic_body> response_;

    Handlers& getHandlers_;
    Handlers& postHandlers_;
    logger::Logger logger_;
};

} // namespace http
} // namespace net

#include "hal/x86/net/http/httpConnection_x86.hpp"

#include <beast/version.hpp>

#include "hal/net/http/asyncHttpRequest.hpp"
#include "hal/net/http/asyncHttpResponse.hpp"

#include <iostream>

namespace net
{
namespace http
{

HttpConnection::HttpConnection(boost::asio::ip::tcp::socket socket, Handlers& handlers)
    : socket_(std::move(socket)),
      getHandlers_(handlers), logger_("HttpConnection")
{
}

void HttpConnection::start()
{
    readRequest();
}

void HttpConnection::getCallback(u16 code, const std::string& type, const std::string& body)
{
    response_.set(beast::http::field::content_type, type);
    response_.result(code);
    beast::ostream(response_.body) << body;
};

std::unique_ptr<AsyncHttpResponse> HttpConnection::chunkedResponseCallback(const std::string& type,
                                                                           AsyncHttpRequest::ChunkedResponseParseCallback callback)
{
    std::unique_ptr<u8[]> buffer(new u8[1001]);
    std::unique_ptr<AsyncHttpResponse> response(new AsyncHttpResponse());
    response->setType(type);
    response->setCode(static_cast<u16>(beast::http::status::ok));

    std::size_t index = 0;
    std::size_t readedBytes = 1;
    while (readedBytes != 0)
    {
        readedBytes = callback(buffer.get(), 1000, index);
        index += readedBytes;
        response->getMsg() += reinterpret_cast<char*>(buffer.get());
        std::memset(buffer.get(), 0, sizeof(u8) * 1001);
    }
    return response;
};

void HttpConnection::readRequest()
{
    auto self = shared_from_this();

    beast::http::async_read(
        socket_,
        buffer_,
        request_,
        [self](beast::error_code ec) {
            if (!ec)
                self->processRequest();
        });
}

void HttpConnection::processRequest()
{
    response_.version = 11;
    response_.set(beast::http::field::connection, "close");

    switch (request_.method())
    {
        case beast::http::verb::get:
            response_.result(beast::http::status::internal_server_error);
            response_.set(beast::http::field::server, "AquaComputerServer");
            createGetResponse();
            break;

        default:

            response_.result(beast::http::status::bad_request);
            response_.set(beast::http::field::content_type, "text/plain");
            beast::ostream(response_.body)
                << "Invalid request-method '"
                << request_.method_string().to_string()
                << "'";
            break;
    }

    writeResponse();
}

void HttpConnection::createGetResponse()
{
    auto handler = getHandlers_.find(request_.target().to_string());
    if (handler != getHandlers_.end())
    {
        std::unique_ptr<AsyncHttpRequest> req(new AsyncHttpRequest());

        req->setSendCallback(std::bind(&getCallback,
                                       this, std::placeholders::_1,
                                       std::placeholders::_2,
                                       std::placeholders::_3));

        req->setChunkedResponseCallback(std::bind(&chunkedResponseCallback,
                                                  this, std::placeholders::_1,
                                                  std::placeholders::_2));

        handler->second(req.get());
    }
    else
    {
        response_.result(beast::http::status::not_found);
        response_.set(beast::http::field::content_type, "text/plain");
        beast::ostream(response_.body) << "File not found\r\n";
    }
}

void HttpConnection::writeResponse()
{
    auto self = shared_from_this();

    response_.set(beast::http::field::content_length, response_.body.size());

    beast::http::async_write(
        socket_,
        response_,
        [self](beast::error_code ec) {
            self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        });
}

} // namespace http
} // namespace net

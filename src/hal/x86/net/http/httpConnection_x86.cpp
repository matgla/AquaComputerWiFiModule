#include "hal/x86/net/http/httpConnection_x86.hpp"

#include <thread>

#include <boost/beast/version.hpp>

#include "hal/net/http/asyncHttpRequest.hpp"
#include "hal/net/http/asyncHttpResponse.hpp"

#include <iostream>

namespace net
{
namespace http
{

HttpConnection::HttpConnection(boost::asio::ip::tcp::socket socket, Handlers& getHandlers,
                               Handlers& postHandlers)
    : socket_(std::move(socket)), getHandlers_(getHandlers), postHandlers_(postHandlers),
      logger_("HttpConnection")
{
}

void HttpConnection::start()
{
    std::thread{&HttpConnection::readRequest, shared_from_this()}.detach();
}

void HttpConnection::getCallback(u16 code, const std::string& type, const std::string& body)
{
    response_.set(boost::beast::http::field::content_type, type);
    response_.result(code);
    boost::beast::ostream(response_.body) << body;
};

std::string HttpConnection::getBodyCallback()
{
    std::stringstream ss;
    ss << boost::beast::buffers(request_.body.data());
    return ss.str();
}

std::unique_ptr<AsyncHttpResponse> HttpConnection::chunkedResponseCallback(
    const std::string& type, const AsyncHttpRequest::ChunkedResponseParseCallback& callback)
{
    std::unique_ptr<u8[]> buffer(new u8[1001]);
    std::unique_ptr<AsyncHttpResponse> response(new AsyncHttpResponse());
    response->setType(type);
    response->setCode(static_cast<u16>(boost::beast::http::status::ok));

    std::size_t index = 0;
    std::size_t readedBytes = 1;
    while (readedBytes != 0)
    {
        readedBytes = callback(buffer.get(), 1000, index);
        index += readedBytes;
        response->getMsg() += reinterpret_cast<char*>(buffer.get()); // NOLINT
        std::memset(buffer.get(), 0, sizeof(u8) * 1001);
    }
    return response;
};

void HttpConnection::readRequest()
{
    boost::beast::error_code ec;
    boost::beast::flat_buffer buffer;

    boost::beast::http::read(socket_, buffer, request_, ec);
    if (ec == boost::beast::http::error::end_of_stream)
    {
        return;
    }
    if (ec != nullptr)
    {
        throw "Unexpected Error in Http connection";
    }
    auto self = shared_from_this();

    self->processRequest(ec);
}

void HttpConnection::processRequest(boost::beast::error_code& ec)
{
    response_.version = 11;
    response_.set(boost::beast::http::field::connection, "close");

    switch (request_.method())
    {
        case boost::beast::http::verb::get:
            response_.result(boost::beast::http::status::internal_server_error);
            response_.set(boost::beast::http::field::server, "AquaComputerServer");
            createGetResponse();
            break;
        case boost::beast::http::verb::post:
            handlePost();
            break;
        default:

            response_.result(boost::beast::http::status::bad_request);
            response_.set(boost::beast::http::field::content_type, "text/plain");
            boost::beast::ostream(response_.body)
                << "Invalid request-method '" << request_.method_string().to_string() << "'";
            break;
    }

    writeResponse(ec);
}

void HttpConnection::handlePost()
{
    auto handler = postHandlers_.find(request_.target().to_string());
    if (handler != postHandlers_.end())
    {
        std::unique_ptr<AsyncHttpRequest> req(new AsyncHttpRequest());

        req->setGetBodyCallback(std::bind(&HttpConnection::getBodyCallback, this));

        handler->second(req.get());
    }
    else
    {
        response_.result(boost::beast::http::status::not_found);
        response_.set(boost::beast::http::field::content_type, "text/plain");
        boost::beast::ostream(response_.body) << "File not found\r\n";
    }
}

void HttpConnection::createGetResponse()
{
    auto handler = getHandlers_.find(request_.target().to_string());
    if (handler != getHandlers_.end())
    {
        std::unique_ptr<AsyncHttpRequest> req(new AsyncHttpRequest());

        req->setSendCallback(std::bind(&HttpConnection::getCallback, this, std::placeholders::_1,
                                       std::placeholders::_2, std::placeholders::_3));

        req->setChunkedResponseCallback(std::bind(&HttpConnection::chunkedResponseCallback, this,
                                                  std::placeholders::_1, std::placeholders::_2));

        handler->second(req.get());
    }
    else
    {
        response_.result(boost::beast::http::status::not_found);
        response_.set(boost::beast::http::field::content_type, "text/plain");
        boost::beast::ostream(response_.body) << "File not found\r\n";
    }
}

void HttpConnection::writeResponse(boost::beast::error_code& ec)
{
    response_.set(boost::beast::http::field::content_length, response_.body.size());

    boost::beast::http::write(socket_, response_, ec);
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
}

} // namespace http
} // namespace net

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

#include "logger/logger.hpp"

using namespace boost;
using namespace boost::asio;

namespace net
{
namespace http
{

auto logger = logger::Logger("HttpServer");

class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
    http_connection(ip::tcp::socket socket, std::map<std::string, RequestType>& handlers)
        : socket_(std::move(socket)),
          handlers_(handlers)
    {
    }

    // Initiate the asynchronous operations associated with the connection.
    void start()
    {
        read_request();
    }

private:
    // The socket for the currently connected client.
    ip::tcp::socket socket_;

    // The buffer for performing reads.
    beast::flat_buffer buffer_{8192};

    // The request message.
    beast::http::request<beast::http::dynamic_body> request_;

    // The response message.
    beast::http::response<beast::http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> deadline_{
        socket_.get_io_service(), std::chrono::seconds(60)};
    std::map<std::string, RequestType>& handlers_;
    // Asynchronously receive a complete request message.
    void read_request()
    {
        logger.info() << "Read request\n";
        auto self = shared_from_this();

        beast::http::async_read(
            socket_,
            buffer_,
            request_,
            [self](beast::error_code ec) {
                if (!ec)
                    self->process_request();
            });
    }

    // Determine what needs to be done with the request message.
    void process_request()
    {
        response_.version = 11;
        response_.set(beast::http::field::connection, "close");

        switch (request_.method())
        {
            case beast::http::verb::get:
                response_.result(beast::http::status::ok);
                response_.set(beast::http::field::server, "Beast");
                create_response();
                break;

            default:
                // We return responses indicating an error if
                // we do not recognize the request method.
                response_.result(beast::http::status::bad_request);
                response_.set(beast::http::field::content_type, "text/plain");
                beast::ostream(response_.body)
                    << "Invalid request-method '"
                    << request_.method_string().to_string()
                    << "'";
                break;
        }

        write_response();
    }

    // Construct a response message based on the program state.
    void create_response()
    {
        std::string target = request_.target().to_string();
        auto handler = handlers_.find(target);
        if (handler != handlers_.end())
        {
            std::unique_ptr<AsyncHttpRequest> req(new AsyncHttpRequest());
            handler->second(req.get());
            logger.info() << req->getType() << "\n";
            response_.set(beast::http::field::content_type, req->getType());
            beast::ostream(response_.body)
                << req->getMsg();
        }
        else
        {
            response_.result(beast::http::status::not_found);
            response_.set(beast::http::field::content_type, "text/plain");
            beast::ostream(response_.body) << "File not found\r\n";
        }
    }

    // Asynchronously transmit the response message.
    void write_response()
    {
        auto self = shared_from_this();

        response_.set(beast::http::field::content_length, response_.body.size());

        beast::http::async_write(
            socket_,
            response_,
            [self](beast::error_code ec) {
                self->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
                self->deadline_.cancel();
            });
    }

    // Check whether we have spent enough time on this connection.
    void check_deadline()
    {
    }
};

class AsyncHttpServer::AsyncHttpWrapper
{
public:
    AsyncHttpWrapper(const std::string& address, u16 port)
        : acceptor_(ioService_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          socket_(ioService_)
    {
    }
    void begin();

    std::map<std::string, RequestType> reqHandlers_;

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
        logger.info() << "I've got some message\n";
        if (!ec)
        {
            std::make_shared<http_connection>(std::move(socket_), reqHandlers_)->start();
        }
        loop();
    });
}

AsyncHttpServer::AsyncHttpServer(u16 port)
    : port_(port), asyncHttpWrapper_(new AsyncHttpWrapper("127.0.0.1", port))
{
}

AsyncHttpServer::~AsyncHttpServer() = default;


void AsyncHttpServer::get(const std::string& uri, RequestType request)
{
    asyncHttpWrapper_->reqHandlers_[uri] = request;
}

void AsyncHttpServer::begin()
{
    logger.info() << "Starting....\n";
    asyncHttpWrapper_->begin();
}

} // namespace http
} // namespace net
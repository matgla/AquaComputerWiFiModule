#include "hal/net/http/asyncHttpServer.hpp"

#include <beast/core.hpp>
#include <beast/http.hpp>
#include <beast/version.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <memory>
#include <string>

using namespace boost;
using namespace boost::asio;

namespace net
{
namespace http
{

class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
    http_connection(ip::tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    // Initiate the asynchronous operations associated with the connection.
    void
        start()
    {
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

    // Asynchronously receive a complete request message.
    void
        read_request()
    {
    }

    // Determine what needs to be done with the request message.
    void
        process_request()
    {
    }

    // Construct a response message based on the program state.
    void
        create_response()
    {
    }

    // Asynchronously transmit the response message.
    void
        write_response()
    {
    }

    // Check whether we have spent enough time on this connection.
    void
        check_deadline()
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

private:
    io_service ioService_;
    AsyncHttpWrapper() = delete;
    void loop();
    ip::tcp::acceptor acceptor_;
    ip::tcp::socket socket_;
};

AsyncHttpServer::AsyncHttpServer(u16 port)
    : port_(port), asyncHttpWrapper_(new AsyncHttpWrapper("127.0.0.1", port))
{
}

AsyncHttpServer::~AsyncHttpServer() = default;

void AsyncHttpServer::get(const std::string& uri, std::function<void(AsyncHttpRequest*)> request)
{
}

void AsyncHttpServer::AsyncHttpWrapper::loop()
{
    acceptor_.async_accept(socket_, [&](beast::error_code ec) {
        if (!ec)
        {
            std::make_shared<http_connection>(std::move(socket_))->start();
        }
        loop();
    });
}

} // namespace http
} // namespace net
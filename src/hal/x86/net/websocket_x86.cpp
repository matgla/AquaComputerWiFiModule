#include "hal/net/websocket.hpp"

#include <thread>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "logger/logger.hpp"

namespace net
{
logger::Logger logger("WS");

typedef websocketpp::server<websocketpp::config::asio> Server;

class WebSocket::WebSocketWrapper
{
public:
    WebSocketWrapper(const std::string& uri, u16 port)
        : uri_(uri), port_(port)
    {
        server_.init_asio();
        server_.set_message_handler(websocketpp::lib::bind(&WebSocketWrapper::onMessage, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
        server_.listen(port_);
    }

    void start()
    {
        server_.start_accept();
        server_.run();
    }

protected:
    void onMessage(websocketpp::connection_hdl hdl, Server::message_ptr msg)
    {
        logger.debug() << "Received payload: " << msg->get_payload();
    }


    Server server_;
    const std::string uri_;
    const u16 port_;
};

WebSocket::WebSocket(const std::string& uri, u16 port)
    : webSocketWrapper_(new WebSocketWrapper(uri, port))
{
}

WebSocket::~WebSocket() = default;

void WebSocket::start()
{
    webSocketWrapper_->start();
}

} // namespace net

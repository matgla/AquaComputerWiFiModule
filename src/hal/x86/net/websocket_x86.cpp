#include "hal/net/websocket.hpp"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

namespace net
{

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

void onMessage(server*)
{
}

class WebSocket::WebSocketWrapper
{
public:
    WebSocketWrapper(const std::string& uri, u16 port)
        : uri_(uri), port_(port)
    {
    }

    const std::string uri_;
    const u16 port_;
};

WebSocket::WebSocket(const std::string& uri, u16 port)
    : webSocketWrapper_(new WebSocketWrapper(uri, port))
{
}

WebSocket::~WebSocket() = default;

} // namespace net

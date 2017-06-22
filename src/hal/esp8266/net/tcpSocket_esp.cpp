#include "hal/net/tcpSocket.hpp"

#include <string>

#include <Arduino.h>
#include <ESP8266WiFi.h>

namespace net
{

class TcpSocket::SocketWrapper
{
public:
    WiFiClient client_;
};

TcpSocket::TcpSocket() : socketWrapper_(new SocketWrapper())
{
}

TcpSocket::~TcpSocket() = default;

void TcpSocket::connect(const std::string& host, u16 port)
{
    socketWrapper_->client_.connect(host.c_str(), static_cast<int>(port));
}

void TcpSocket::write(const std::string& data)
{
    socketWrapper_->client_.print(String(data.c_str()));
}

}  // namespace net
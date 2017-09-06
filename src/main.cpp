#include <string>
/*************** [PLATFORM DEPENDENT INIT] ******************/
#ifdef X86_ARCH

void setup();
void loop();

int main()
{
    setup();
    while (true)
    {
        loop();
    }
}

#elif ESP8266_ARCH
#include "ArduinoFixed.hpp"
#endif // X86_ARCH

/************************ [MAIN] **************************/
#include <memory>

#include <ArduinoJson.h>

#include "hal/fs/file.hpp"
#include "hal/fs/filesystem.hpp"
#include "hal/net/socket/tcpServer.hpp"
#include "hal/serial/serialPort.hpp"
#include "hal/time/sleep.hpp"
#include "handler/dispatcher.hpp"
#include "handler/handshakeHandler.hpp"
#include "handler/jsonHandler.hpp"
#include "handler/messageHandler.hpp"
#include "logger/fileLogger.hpp"
#include "logger/logger.hpp"
#include "logger/loggerConf.hpp"
#include "logger/stdOutLogger.hpp"
#include "settings/settings.hpp"
#include "statemachine/mcuConnection.hpp"

namespace
{
// hal::serial::SerialPort serial("");
std::shared_ptr<hal::net::socket::TcpServer> messageServer(new hal::net::socket::TcpServer(1010));
handler::Dispatcher dispatcher;
std::unique_ptr<handler::JsonHandler> jsonHandler(new handler::JsonHandler());

auto serialPort = std::make_shared<hal::serial::SerialPort>(
    settings::Settings::db()["serial"]["port"].as<char*>(), 9600);

const std::string& handlerName = "SerialHandler";

logger::Logger mcuConnectionLogger("McuConnection");

boost::sml::sm<statemachine::McuConnection> mcuConnection{jsonHandler.get(), mcuConnectionLogger};
}

void setup()
{
    logger::Logger logger("Main");
    for (auto& logger : settings::Settings::db()["loggers"].as<JsonArray>())
    {
        if ("stdout" == logger["type"])
        {
            logger::LoggerConf::get().add(logger::StdOutLogger{});
        }
        else if ("file" == logger["type"])
        {
            hal::fs::FileSystem::removeFile(logger["path"].as<const char*>());
            logger::LoggerConf::get().add(logger::FileLogger{logger["path"].as<const char*>()});
        }
    }

    logger.info() << "System booting up";
    jsonHandler->setConnection(serialPort);

    handler::IHandler::HandlerPtr handshakeHandler(
        new handler::HandshakeHandler(jsonHandler.get()));

    jsonHandler->addMessageHandler("handshake", std::move(handshakeHandler));
    dispatcher.addHandler(std::move(jsonHandler), handlerName);

    logger.info() << "Handlers setup finished";
}

void loop()
{
    logger::Logger logger("loop");
    serialPort->process();
    if (mcuConnection.is(boost::sml::state<statemachine::states::NotConnected>))
    {
        logger.info() << "Process connect";
        mcuConnection.process_event(statemachine::events::Connect{});
    }
    hal::time::msleep(100);
}

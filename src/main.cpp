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

#include "dispatcher/dispatcher.hpp"
#include "dispatcher/handler/getInfoHandler.hpp"
#include "dispatcher/handler/handshakeHandler.hpp"
#include "hal/fs/file.hpp"
#include "hal/fs/filesystem.hpp"
#include "hal/net/socket/tcpServer.hpp"
#include "hal/serial/serialPort.hpp"
#include "hal/time/sleep.hpp"
#include "logger/fileLogger.hpp"
#include "logger/logger.hpp"
#include "logger/loggerConf.hpp"
#include "logger/stdOutLogger.hpp"
#include "message/messages.hpp"
#include "settings/settings.hpp"
#include "statemachine/mcuConnectionFrontEnd.hpp"

namespace
{
// hal::serial::SerialPort serial("");
std::shared_ptr<hal::net::socket::TcpServer> messageServer(new hal::net::socket::TcpServer(1010));
dispatcher::Dispatcher dispatch;
// std::unique_ptr<dispatcher::JsonHandler> jsonHandler(new dispatcher::JsonHandler());

auto serialPort = std::make_shared<hal::serial::SerialPort>(
    settings::Settings::db()["serial"]["port"].as<char*>(), 9600);

const std::string& handlerName = "SerialHandler";

logger::Logger mcuConnectionLogger("McuConnection");

// boost::sml::sm<statemachine::McuConnection> mcuConnection{jsonHandler.get(),
// mcuConnectionLogger};
// statemachine::McuConnectionFrontEnd mcuSM(*jsonHandler);
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
    // jsonHandler->setConnection(serialPort);

    // dispatcher::IHandler::HandlerPtr handshakeHandler(
    //     new dispatcher::handler::HandshakeHandler(mcuSM));
    // dispatcher::IHandler::HandlerPtr getInfoHandler(new
    // dispatcher::handler::GetInfoHandler(mcuSM));

    // jsonHandler->addMessageHandler(message::Handshake, std::move(handshakeHandler));
    // jsonHandler->addMessageHandler(message::GetInfo, std::move(getInfoHandler));
    // dispatch.addHandler(std::move(jsonHandler), handlerName);

    logger.info() << "Handlers setup finished";
}

void loop()
{
    logger::Logger logger("loop");
    serialPort->process();
    // if (mcuSM.backend().is(boost::sml::state<statemachine::states::NotConnected>))
    // {
    //     logger.info() << "Process connect";
    //     mcuSM.connect();
    // }
    hal::time::msleep(100);
}

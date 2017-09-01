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
#include "hal/net/socket/tcpServer.hpp"
#include "hal/serial/serialPort.hpp"
#include "hal/time/sleep.hpp"
#include "handler/dispatcher.hpp"
#include "handler/jsonHandler.hpp"
#include "handler/messageHandler.hpp"
#include "logger/logger.hpp"
#include "logger/loggerConf.hpp"
#include "logger/stdOutLogger.hpp"
#include "settings/settings.hpp"

namespace
{
// hal::serial::SerialPort serial("");
std::shared_ptr<hal::net::socket::TcpServer> messageServer(new hal::net::socket::TcpServer(1010));
handler::Dispatcher dispatcher;
std::unique_ptr<handler::JsonHandler> jsonHandler(new handler::JsonHandler());

auto serialPort = std::make_shared<hal::serial::SerialPort>(
    settings::Settings::db()["serial"]["port"].as<char*>(), 9600);

const std::string& handlerName = "SerialHandler";
}

void setup()
{
    serialPort->write("Hejo :)\n");
    logger::Logger logger("Main");
    for (auto& loggerType : settings::Settings::db()["loggers"].as<JsonArray>())
    {
        if ("stdout" == loggerType)
        {
            logger::LoggerConf::get().add(logger::StdOutLogger{});
        }
    }

    logger.info() << "System booting up";
    jsonHandler->setConnection(serialPort);

    dispatcher.addHandler(std::move(jsonHandler), handlerName);

    logger.info() << "Handlers setup finished";
}

void loop()
{
    // serial.write("a\n");
    hal::time::sleep(1);
}

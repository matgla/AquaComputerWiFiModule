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
#include "handler/messageReceiver.hpp"
#include "logger/logger.hpp"
#include "logger/loggerConf.hpp"
#include "logger/stdOutLogger.hpp"
#include "settings/settings.hpp"

namespace
{
// hal::serial::SerialPort serial("");
std::shared_ptr<hal::net::socket::TcpServer> messageServer(new hal::net::socket::TcpServer(1010));
handler::Dispatcher dispatcher;
auto receiver = std::make_shared<handler::MessageReceiver>();
auto jsonHandler = std::make_shared<handler::JsonHandler>();

const std::string& receiverName = "MessageFrameReceiver";
const std::string& handlerName = "JsonHandler";
const std::string& dataReceiverName = "TcpReceiver";
}

void setup()
{
    logger::Logger logger("Main");
    // std::cout << settings::Settings::db()["Loggers"] << std::endl;
    for (auto& loggerType : settings::Settings::db()["Loggers"].as<JsonArray>())
    {
        if ("stdout" == loggerType)
        {
            logger::LoggerConf::get().add(logger::StdOutLogger{});
        }
    }
    logger.info() << "System booting up";
    dispatcher.addRawDataReceiver(messageServer, dataReceiverName);
    dispatcher.addFrameReceiver(receiver, receiverName);
    dispatcher.addHandler(jsonHandler, handlerName);

    dispatcher.connectReceivers(dataReceiverName, receiverName);
    dispatcher.connectHandler(receiverName, handlerName);
    logger.info() << "Handlers setup finished";
}

void loop()
{
    // serial.write("a\n");
    hal::time::sleep(1);
}

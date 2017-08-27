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

#include <ArduinoJson.h>

#include "hal/fs/file.hpp"
#include "hal/net/socket/tcpServer.hpp"
#include "hal/serial/serialPort.hpp"
#include "hal/time/sleep.hpp"
#include "logger/logger.hpp"
#include "logger/loggerConf.hpp"
#include "logger/stdOutLogger.hpp"
#include "settings/settings.hpp"

namespace
{
// hal::serial::SerialPort serial("");
hal::net::socket::TcpServer messageServer(1010);
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
}

void loop()
{
    // serial.write("a\n");
    hal::time::sleep(1);
}

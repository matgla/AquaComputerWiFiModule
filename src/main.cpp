/*************** [PLATFORM DEPENDENT INIT] ******************/
#ifdef X86_ARCH

void setup();
void loop();

int main()
{
    setup();
    loop();
}

#elif ESP8266_ARCH
#include <Arduino.h>

#endif // X86_ARCH

/************************ [MAIN] **************************/

#include "hal/serial/serialPort.hpp"
#include "hal/time/sleep.hpp"

namespace
{
    hal::serial::SerialPort serial("");
}

void setup()
{
    serial.write("Hello\n");
}

void loop()
{
    serial.write("a\n");
    hal::time::sleep(1);
}

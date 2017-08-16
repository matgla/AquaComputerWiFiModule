// #ifdef X86_ARCH

// void setup();
// void loop();

// int main()
// {
//     setup();
//     loop();
// }
// #else
// #include <Arduino.h>
// #endif

// #include "hal/serial/serialPort.hpp"
// #include "hal/time/sleep.hpp"

// // hal::serial::SerialPort serialPort("");

// void setup()
// {
//     Serial.begin(115200);
//     Serial.println();
//     delay(1000);
//     Serial.println("Dlaczemu:(");

//     // serialPort.write("Setupuje sie\n");
// }

// void loop()
// {
//     // Serial.write("Dzialam\n");
//     delay(1000);
//     Serial.println("Dlaczemu:(");
//     // serialPort.write("Dzialam\n");
//     // /hal::time::sleep(1);
// }


#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    Serial.write("Hello\n");
}

void loop()
{
    Serial.write("a\n");
    delay(1000);
}

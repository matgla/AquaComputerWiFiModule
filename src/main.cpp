// #include <ArduinoOTA.h>
// // #include <ESP8266WebServer.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266mDNS.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Hash.h>
// // #include <SPIFFSEditor.h>

// #include "helper\fileHandler.hpp"
// #include "helper\loggerHelper.hpp"
// #include "server\ftp.hpp"
// #include <SoftwareSerial.h>
// // #include <ESP8266FtpServer.h>
// // SKETCH BEGIN
// AsyncWebServer server(80);

// // AsyncWebSocket ws("/ws");
// // AsyncEventSource events("/events");

// const char* ssid = SSID_NAME;
// const char* password = SSID_PASSWORD;

// const char* hostName = "esp-async";
// const char* http_username = "admin";
// const char* http_password = "admin";

// // SD card chip select pin.

// uint16_t n = 0;
// const uint16_t nMax = 10;

// uint16_t dirIndex[nMax];

// SoftwareSerial mySerial(2, 5); // RX, TX

// void setup()
// {
//     Serial.begin(115200);
//     Serial.setDebugOutput(true);
//     mySerial.begin(9600);
//     initSd();
//     WiFi.hostname(hostName);
//     WiFi.mode(WIFI_AP_STA);
//     WiFi.softAP(hostName);
//     WiFi.begin(ssid, password);
//     if (WiFi.waitForConnectResult() != WL_CONNECTED)
//     {
//         Serial.printf("STA: Failed!\n");
//         WiFi.disconnect(false);
//         delay(1000);
//         WiFi.begin(ssid, password);
//     }
//     initFtp();
//     log("started");

//     //Send OTA events to the browser
//     ArduinoOTA.onStart([]() { Serial.println("Ota update start"); });
//     ArduinoOTA.onEnd([]() { Serial.println("Ota update end"); });
//     ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//         Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
//         //events.send(p, "ota");
//     });
//     ArduinoOTA.onError([](ota_error_t error) {
//         Serial.println("OTA Error");
//         //if (error == OTA_AUTH_ERROR)
//         //else if (error == OTA_BEGIN_ERROR) else if (error == OTA_CONNECT_ERROR) else if (error == OTA_RECEIVE_ERROR) else if (error == OTA_END_ERROR)
//     });
//     ArduinoOTA.setHostname(hostName);
//     ArduinoOTA.begin();

//     MDNS.addService("http", "tcp", 80);

//     // SPIFFS.begin();

//     // ws.onEvent(onWsEvent);
//     // server.addHandler(&ws);

//     // events.onConnect([](AsyncEventSourceClient * client) {
//     //     client->send("hello!", NULL, millis(), 1000);
//     // });
//     // server.addHandler(&events);

//     // server.addHandler(new SPIFFSEditor(http_username, http_password));

//     // // if (!dirFile.open("/", fat::O_READ)) {
//     // //     sd.errorHalt("open root failed");
//     // // }
//     // //
//     // // while (n < nMax && file.openNext(&dirFile, fat::O_READ)) {
//     // //
//     // //     // Save dirIndex of file in directory.
//     // //     dirIndex[n] = file.dirIndex();
//     // //
//     // //     // Print the file number and name.
//     // //     Serial.print(n++);
//     // //     Serial.write(' ');
//     // //     file.printName(&Serial);
//     // //     Serial.println();
//     // //
//     // //     file.close();
//     // // }
//     // //

//     Serial.println("Jadymy z koksem");
//     log("jadymy z koksem");

//     server.on("/heap", HTTP_GET, [](AsyncWebServerRequest* request) {
//         request->send(200, "text/plain", String(ESP.getFreeHeap()));
//     });

//     server.on("/", HTTP_ANY, [](AsyncWebServerRequest* request) {
//         Serial.printf("Received get: %s\n", request->url().c_str());
//         AsyncWebServerResponse* response = request->beginChunkedResponse("text/html", [request](uint8_t* buffer, size_t maxLen, size_t index) {
//             // sd::File file = SD.open(path);
//             // file.seek(index);
//             // size_t readedSize = file.read(buffer, maxLen);
//             // file.close();
//             return 0; //readedSize;
//         });

//         request->send(response);
//     });

//     server.on("/index", HTTP_ANY, [](AsyncWebServerRequest* request) {
//         Serial.printf("Received get: %s\n", request->url().c_str());
//         AsyncWebServerResponse* response = request->beginChunkedResponse("text/html", std::bind(&handleGetIndexServer,
//                                                                                                 request->url(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

//         request->send(response);
//     });

//     server.on("/css", HTTP_ANY, [](AsyncWebServerRequest* request) {
//         Serial.printf("Received get: %s\n", request->url().c_str());
//         AsyncWebServerResponse* response = request->beginChunkedResponse("text/css", std::bind(&handleGetFileServer,
//                                                                                                request->url(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

//         request->send(response);
//     });

//     server.on("/js", HTTP_ANY, [](AsyncWebServerRequest* request) {
//         Serial.printf("Received get: %s\n", request->url().c_str());
//         AsyncWebServerResponse* response = request->beginChunkedResponse("text/script", std::bind(&handleGetFileServer,
//                                                                                                   request->url(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

//         request->send(response);
//     });

//     server.on("/logs", HTTP_ANY, [](AsyncWebServerRequest* request) {
//         Serial.println("logs request");
//         AsyncWebServerResponse* response
//             = request->beginChunkedResponse("text/plain", std::bind(&handleGetLogs,
//             std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

//         request->send(response);
//     });

//     server.onNotFound([](AsyncWebServerRequest* request) {
//         Serial.printf("NOT_FOUND: ");
//         if (request->method() == HTTP_GET)
//             Serial.printf("GET");
//         else if (request->method() == HTTP_POST)
//             Serial.printf("POST");
//         else if (request->method() == HTTP_DELETE)
//             Serial.printf("DELETE");
//         else if (request->method() == HTTP_PUT)
//             Serial.printf("PUT");
//         else if (request->method() == HTTP_PATCH)
//             Serial.printf("PATCH");
//         else if (request->method() == HTTP_HEAD)
//             Serial.printf("HEAD");
//         else if (request->method() == HTTP_OPTIONS)
//             Serial.printf("OPTIONS");
//         else
//             Serial.printf("UNKNOWN");
//         Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

//         if (request->contentLength())
//         {
//             Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
//             Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
//         }

//         int headers = request->headers();
//         int i;
//         for (i = 0; i < headers; i++)
//         {
//             AsyncWebHeader* h = request->getHeader(i);
//             Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
//         }

//         int params = request->params();
//         for (i = 0; i < params; i++)
//         {
//             AsyncWebParameter* p = request->getParam(i);
//             if (p->isFile())
//             {
//                 Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
//             }
//             else if (p->isPost())
//             {
//                 Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
//             }
//             else
//             {
//                 Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
//             }
//         }

//         request->send(404);
//     });
//     server.onFileUpload([](AsyncWebServerRequest* request, const String& filename, size_t index, uint8_t* data, size_t len, bool final) {
//         if (!index)
//             Serial.printf("UploadStart: %s\n", filename.c_str());
//         Serial.printf("%s", (const char*)data);
//         if (final)
//             Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
//     });
//     server.onRequestBody([](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
//         if (!index)
//             Serial.printf("BodyStart: %u\n", total);
//         Serial.printf("%s", (const char*)data);
//         if (index + len == total)
//             Serial.printf("BodyEnd: %u\n", total);
//     });


//     server.begin();
//     mySerial.write(0x7f);
//     //mySerial.write((u8)0);
//     //%mySerial.write(0xff);
//     // while (!mySerial.available())
//     // {
//     // }
//     u8 data = mySerial.read();
//     mySerial.write("JAZZZDAAAAAA\n");
//     Serial.printf("Readed data: %d\n", data);
//     // data = mySerial.read();
//     // Serial.printf("Readed data: %d\n", data);
//     // data = mySerial.read();
//     // Serial.printf("Readed data: %d\n", data);
//     // data = mySerial.read();
//     // Serial.printf("Readed data: %d\n", data);
//     // data = mySerial.read();
//     // Serial.printf("Readed data: %d\n", data);
//     // data = mySerial.read();
//     // Serial.printf("Readed data: %d\n", data);
// }

// void loop()
// {
//     handleFtp();
//     ArduinoOTA.handle();
//     if (mySerial.available())
//     {
//         mySerial.printf("received: %d\n", mySerial.read());
//     }
// }

// void printInfo()
// {
//     Serial.printf("ESP STARTED!\n");
//     Serial.printf("Heap: %d\n", ESP.getFreeHeap());
//     Serial.printf("SDK: %s\n", ESP.getSdkVersion());
//     Serial.printf("Cpu_freq: %d\n", ESP.getCpuFreqMHz());
//     Serial.printf("Flash size: %d\n", ESP.getFlashChipRealSize());
//     Serial.printf("Sketch size: %d\n", ESP.getSketchSize());
//     Serial.printf("Sketch free size: %d\n", ESP.getFreeSketchSpace());
// }

#ifndef X86_ARCH
#include "Arduino.h"

namespace settings
{
const char* path = "esp8266_settings.json";
}


#else

#include <iostream>
#include <string>

namespace settings
{
const char* path = "x86_settings.json";
}

void setup();
void loop();

int main()
{
    setup();
    loop();
}

#endif

#include "hal/fs/file.hpp"
#include "hal/net/http/asyncHttpRequest.hpp"
#include "hal/net/http/asyncHttpResponse.hpp"
#include "hal/net/http/asyncHttpServer.hpp"
#include "hal/net/socket/tcpClient.hpp"
#include "hal/net/socket/tcpServer.hpp"
#include "hal/net/socket/websocket.hpp"
#include "handler/handlers.hpp"

#include "hal/serial/serialPort.hpp"
#include "hal/time/sleep.hpp"
#include "logger/logger.hpp"
#include "logger/loggerConf.hpp"
#include "logger/socketLogger.hpp"
#include "logger/stdOutLogger.hpp"

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

net::http::AsyncHttpServer server(80);
hal::net::socket::WebSocket ws("/ws", 9002);

void setup()
{
    logger::LoggerConf::get().add(logger::StdOutLogger{});
    auto logger = logger::Logger("main");
    //    logger.add(logger::SocketLogger("127.0.0.1", 1234, "main"));
    // logger.info() << "Test"
    //               << " Proba mikrofonu";
    //    logger.warn() << "I do pliczku dziala tez\n";
    // serial::SerialPort port("COM4");
    // std::cout << "wyszedlem za konstruktor" << std::endl;
    // port.write("Jazda jazda jazda\n");
    // while (true)
    // {
    //     std::this_thread::sleep_for(std::chrono::seconds(1));
    //     if (port.isDataToRecive())
    //     {
    //         size_t len = port.isDataToRecive();
    //         char buf[1024];
    //         port.read((u8*)buf, len);
    //         buf[len] = 0;
    //         std::cout << buf << std::endl;
    //     }
    // }
    // net::TcpSocket s;
    // s.connect("127.0.0.1", 1234);
    // s.write("Panie dzialaj pan\n");
    // using namespace net::http;
    // server.get("/test", [](AsyncHttpRequest* request) {
    //     request->send(200, "text/plain", "Cos tam dzialam");
    // });

    // server.get("/index", [](AsyncHttpRequest* request) {
    //     std::unique_ptr<AsyncHttpResponse> response(std::move(
    //         request->beginChunkedResponse("text/html",
    //                                       [](uint8_t* buffer, size_t maxLen, size_t index) {
    //                                           fs::File file;
    //                                           file.open("index.html");
    //                                           if (!file.isOpen())
    //                                           {
    //                                               logger::Logger logger("IndexResp");
    //                                               logger.err() << "Can't open file: index.html";
    //                                               return std::size_t(0);
    //                                           }
    //                                           file.seek(index);
    //                                           size_t readedSize = file.read(reinterpret_cast<char*>(buffer),
    //                                                                         maxLen);
    //                                           file.close();
    //                                           return readedSize;
    //                                       })));

    //     request->send(response.get());
    // });

    // server.post("/software_to_update", [](AsyncHttpRequest* request) {
    //     fs::File file;
    //     file.open("sw.bin");
    //     file.write(request->getBody());
    //     file.close();
    //     logger::Logger logger("file");
    //     logger.info() << "File saved";
    // });

    // server.begin();
    logger.debug() << "begin end";
    // ws.start();
}

void loop()
{
    hal::net::socket::TcpServer serv(1234, [](const u8* buf, std::size_t len, handler::WriterCallback write) {
        logger::Logger logger("Server");
        char message[1000];
        memcpy(message, buf, len);
        message[len + 1] = 0;
        logger.info() << "Read: " << buf;
        const char* msg = "No witam";
        write(reinterpret_cast<const u8*>(msg), 8);
    });
    serv.start();

    auto logger = logger::Logger("main");
    {
        hal::net::socket::TcpClient client("127.0.0.1", 1234, [](const u8* buf, std::size_t len, handler::WriterCallback write) {
            logger::Logger logger("Client1");
            char message[1000];
            memcpy(message, buf, len);
            message[len + 1] = 0;
            logger.info() << "Read: " << message;
        });
        hal::net::socket::TcpClient client2("127.0.0.1", 1234);
        client.start();
        client.write("Welcome my friend");
        //client.write(69);
        client2.start();
        client2.write("client 2 sie lonczy");
        // client.write("dalej dzialam\0");
        // client2.write("i ja tez\0");
        net::http::waitForBreak();
    }
    logger.info() << "exiting..";
}

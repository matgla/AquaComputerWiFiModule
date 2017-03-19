
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>

// SKETCH BEGIN
AsyncWebServer server(80);

const char* ssid = SSID_NAME;
const char* password = SSID_PASSWORD;

const char* hostName = "esp-async";
const char* http_username = "admin";
const char* http_password = "admin";

void printInfo()
{
    Serial.printf("ESP STARTED!\n");
    Serial.printf("Heap: %d\n", ESP.getFreeHeap());
    Serial.printf("SDK: %s\n", ESP.getSdkVersion());
    Serial.printf("Cpu_freq: %d\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash size: %d\n", ESP.getFlashChipRealSize());
    Serial.printf("Sketch size: %d\n", ESP.getSketchSize());
    Serial.printf("Sketch free size: %d\n", ESP.getFreeSketchSpace());
}

String getShortFileName(String filename)
{
    
}

void setup(){
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  WiFi.hostname(hostName);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(hostName);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("STA: Failed!\n");
    WiFi.disconnect(false);
    delay(1000);
    WiFi.begin(ssid, password);
  }

  if(!SD.begin(4))
    {
        Serial.println("SD initialization failed\n");
        return;
    };

    ArduinoOTA.onStart([]() {
     Serial.println("Start");
     });
     ArduinoOTA.onEnd([]() {
     Serial.println("\nEnd");
     });
     ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
     Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
     });
     ArduinoOTA.onError([](ota_error_t error) {
     Serial.printf("Error[%u]: ", error);
     if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
     else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
     else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
     else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
     else if (error == OTA_END_ERROR) Serial.println("End Failed");
     });
     ArduinoOTA.begin();

  MDNS.addService("http","tcp",80);

  server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", String(ESP.getFreeHeap()));
  });



  server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request){
      Serial.printf("Received get: %s\n", request->url().c_str());
      AsyncWebServerResponse *response = request->beginChunkedResponse("text/html",  [](uint8_t* buffer, size_t maxLen, size_t index) {
          sd::File file = SD.open("/www/index.html");
          file.seek(index);
          size_t readedSize = file.read(buffer, maxLen);
          file.close();
          return readedSize;
      });

      request->send(response);
  });

  // server.on("/index", HTTP_ANY, [](AsyncWebServerRequest *request){
  //     Serial.printf("Received get: %s\n", request->url().c_str());
  //     AsyncWebServerResponse *response = request->beginChunkedResponse("text/html",  [](uint8_t* buffer, size_t maxLen, size_t index) {
  //         sd::File file = SD.open("/WWW/INDEX~1.HTM");
  //         file.seek(index);
  //         size_t readedSize = file.read(buffer, maxLen);
  //         file.close();
  //         return readedSize;
  //     });
  //
  //     request->send(response);
  // });

  // server.on("/css/foundation.css", HTTP_ANY, [](AsyncWebServerRequest *request){
  //     AsyncWebServerResponse *response = request->beginChunkedResponse("text/css",  [](uint8_t* buffer, size_t maxLen, size_t index) {
  //         sd::File file = SD.open("/WWW/CSS/FOUNDA~2.CSS");
  //         file.seek(index);
  //         size_t readedSize = file.read(buffer, maxLen);
  //         file.close();
  //         return readedSize;
  //     });
  //
  //     request->send(response);
  // });
  // server.on("/js/app.js", HTTP_ANY, [](AsyncWebServerRequest *request){
  //     AsyncWebServerResponse *response = request->beginChunkedResponse("text/html",  [](uint8_t* buffer, size_t maxLen, size_t index) {
  //         sd::File file = SD.open("/WWW/JS/APP.JS");
  //         file.seek(index);
  //         size_t readedSize = file.read(buffer, maxLen);
  //         file.close();
  //         return readedSize;
  //     });
  //
  //     request->send(response);
  // });
  //
  // server.on("/js/vendor/jquery.js", HTTP_ANY, [](AsyncWebServerRequest *request){
  //     AsyncWebServerResponse *response = request->beginChunkedResponse("text/html",  [](uint8_t* buffer, size_t maxLen, size_t index) {
  //         sd::File file = SD.open("/WWW/JS/VENDOR/JQUERY.JS");
  //         file.seek(index);
  //         size_t readedSize = file.read(buffer, maxLen);
  //         file.close();
  //         return readedSize;
  //     });
  //
  //     request->send(response);
  // });
  //
  // server.on("/js/vendor/foundation.js", HTTP_ANY, [](AsyncWebServerRequest *request){
  //     AsyncWebServerResponse *response = request->beginChunkedResponse("text/html",  [](uint8_t* buffer, size_t maxLen, size_t index) {
  //         sd::File file = SD.open("/WWW/JS/VENDOR/FOUNDA~2.JS");
  //         file.seek(index);
  //         size_t readedSize = file.read(buffer, maxLen);
  //         file.close();
  //         return readedSize;
  //     });
  //
  //     request->send(response);
  // });
  //
  // server.on("/css/app.css", HTTP_ANY, [](AsyncWebServerRequest *request){
  //     AsyncWebServerResponse *response = request->beginChunkedResponse("text/css",  [](uint8_t* buffer, size_t maxLen, size_t index) {
  //         sd::File file = SD.open("/WWW/CSS/APP.CSS");
  //         file.seek(index);
  //         size_t readedSize = file.read(buffer, maxLen);
  //         file.close();
  //         return readedSize;
  //     });
  //
  //     request->send(response);
  // });
  //
  // server.on("/js/vendor/what-input.js", HTTP_ANY, [](AsyncWebServerRequest *request){
  //     AsyncWebServerResponse *response = request->beginChunkedResponse("text/html",  [](uint8_t* buffer, size_t maxLen, size_t index) {
  //         sd::File file = SD.open("/WWW/JS/VENDOR/WHAT-I~1.JS");
  //         file.seek(index);
  //         size_t readedSize = file.read(buffer, maxLen);
  //         file.close();
  //         return readedSize;
  //     });
  //
  //     request->send(response);
  // });



  server.onNotFound([](AsyncWebServerRequest *request){
    Serial.printf("NOT_FOUND: ");
    if(request->method() == HTTP_GET)
      Serial.printf("GET");
    else if(request->method() == HTTP_POST)
      Serial.printf("POST");
    else if(request->method() == HTTP_DELETE)
      Serial.printf("DELETE");
    else if(request->method() == HTTP_PUT)
      Serial.printf("PUT");
    else if(request->method() == HTTP_PATCH)
      Serial.printf("PATCH");
    else if(request->method() == HTTP_HEAD)
      Serial.printf("HEAD");
    else if(request->method() == HTTP_OPTIONS)
      Serial.printf("OPTIONS");
    else
      Serial.printf("UNKNOWN");
    Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

    if(request->contentLength()){
      Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
      Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
    }

    int headers = request->headers();
    int i;
    for(i=0;i<headers;i++){
      AsyncWebHeader* h = request->getHeader(i);
      Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
    }

    int params = request->params();
    for(i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isFile()){
        Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
      } else if(p->isPost()){
        Serial.printf("_POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }

    request->send(404);
  });
  server.onFileUpload([](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
    if(!index)
      Serial.printf("UploadStart: %s\n", filename.c_str());
    Serial.printf("%s", (const char*)data);
    if(final)
      Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index+len);
  });
  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    if(!index)
      Serial.printf("BodyStart: %u\n", total);
    Serial.printf("%s", (const char*)data);
    if(index + len == total)
      Serial.printf("BodyEnd: %u\n", total);
  });
  server.begin();
  printInfo();
}

void loop(){
  ArduinoOTA.handle();
}
//
//
// #include <ESP8266WiFi.h>
// #include <ESP8266mDNS.h>
// #include <WiFiUdp.h>
// #include <ArduinoOTA.h>
//
// const char* ssid = SSID_NAME;
// const char* password = SSID_PASSWORD;
//
// void setup() {
//  Serial.begin(115200);
//  Serial.println("Booting");
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//  Serial.println("Connection Failed! Rebooting...");
//  delay(5000);
//  ESP.restart();
//  }
//
// // Port defaults to 8266
//  // ArduinoOTA.setPort(8266);
//
// // Hostname defaults to esp8266-[ChipID]
//  // ArduinoOTA.setHostname("myesp8266");
//
// // No authentication by default
//  // ArduinoOTA.setPassword((const char *)"123");
//
// ArduinoOTA.onStart([]() {
//  Serial.println("Start");
//  });
//  ArduinoOTA.onEnd([]() {
//  Serial.println("\nEnd");
//  });
//  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
//  });
//  ArduinoOTA.onError([](ota_error_t error) {
//  Serial.printf("Error[%u]: ", error);
//  if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
//  else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
//  else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
//  else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
//  else if (error == OTA_END_ERROR) Serial.println("End Failed");
//  });
//  ArduinoOTA.begin();
//  Serial.println("Ready");
//  Serial.print("IPess: ");
//  Serial.println(WiFi.localIP());
// }
//
// void loop() {
//  ArduinoOTA.handle();
// }

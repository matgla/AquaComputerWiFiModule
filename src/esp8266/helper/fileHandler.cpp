#include "fileHandler.hpp"
#include "SdFat.h"
#include "ftp/FatLib.h"
const uint8_t SD_CS_PIN = 4;
#define CS_SDCARD 4

//ExtFat sd;
SdFile file;
SdFile dirFile;

void initSd()
{
    if (!FAT.begin(CS_SDCARD, SPI_FULL_SPEED))
    {
        Serial.println("Unable to mount SD card");
    }
    // if (!sd1.begin(SD_CS_PIN))
    // {
    //     sd1.initErrorHalt();
    // }
}

size_t handleGetIndexServer(String reqPath, uint8_t* buffer, size_t maxLen, size_t index)
{
    String path = "www";
    path += reqPath;
    path += ".html";
    SdFile file;
    if (!file.open(path.c_str(), O_READ))
    {
        Serial.println(String("Opening file failed") + path);
    }
    file.seekSet(index);
    size_t readedSize = file.read(buffer, maxLen);
    file.close();
    //Serial.printf("Sent %d on %d of file: %s\n", readedSize, index, path.c_str());
    return readedSize;
    //
    // return 0;
}

size_t handleGetFileServer(String reqPath, uint8_t* buffer, size_t maxLen, size_t index)
{
    String path = "www";
    path += reqPath;
    SdFile file;
    if (!file.open(path.c_str(), O_READ))
    {
        Serial.println(String("Opening file failed") + path);
    }
    file.seekSet(index);
    size_t readedSize = file.read(buffer, maxLen);
    file.close();
    //Serial.printf("Sent %d on %d of file: %s\n", readedSize, index, path.c_str());
    return readedSize;
    //
    // return 0;
}

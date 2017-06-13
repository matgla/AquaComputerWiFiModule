#include "helper/loggerHelper.hpp"

#include <SPIFFSEditor.h>

size_t handleGetLogs(uint8_t* buffer, size_t maxLen, size_t index)
{
    File f = SPIFFS.open("/target.log", "r");
    f.seek(index, SeekSet);
    size_t readedSize = f.read(buffer, maxLen);
    f.close();
    return readedSize;
}

void log(String log)
{
    File f = SPIFFS.open("/target.log", "w");
    f.println(log);
    f.close();
}

#ifndef FILE_HEADER_HPP
#define FILE_HEADER_HPP

#include <Arduino.h>

size_t handleGetIndexServer(String reqPath, uint8_t* buffer, size_t maxLen, size_t index);
size_t handleGetFileServer(String reqPath, uint8_t* buffer, size_t maxLen, size_t index);
void initSd();
#endif /* end of include guard: FILE_HEADER_HPP */

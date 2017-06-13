#ifndef LOGGER_HEADER_HPP
#define LOGGER_HEADER_HPP

#include <Arduino.h>

size_t handleGetLogs(uint8_t* buffer, size_t maxLen, size_t index);

void log(String log);

#endif /* end of include guard: FILE_HEADER_HPP */

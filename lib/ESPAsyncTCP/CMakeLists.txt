file(GLOB_RECURSE esp_async_tcp_srcs "ESPAsyncTCP/src/*.cpp" "ESPAsyncTCP/src/*.c")
file(GLOB_RECURSE esp_async_tcp_incs "ESPAsyncTCP/src/*.hpp" "ESPAsyncTCP/src/*.h")

set(ESPAsyncTCP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/ESPAsyncTCP/ESPAsyncTCP/src")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-empty-body ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-empty-body ")

add_library(espasynctcp STATIC ${esp_async_tcp_srcs} ${esp_async_tcp_incs})

target_include_directories(
    espasynctcp SYSTEM PUBLIC
    ${ARDUINO_ESP8266_DIR}/tools/sdk/include
    ${ARDUINO_ESP8266_DIR}/tools/sdk/lwip/include
    ${ARDUINO_ESP8266_DIR}/cores/esp8266
    ${ARDUINO_ESP8266_DIR}/variants/nodemcu
    ${ARDUINO_ESP8266_DIR}/libraries/ESP8266WiFi/src
    ${PROJECT_SOURCE_DIR}/lib/ESPAsyncTCP/ESPAsyncTCP/src
)
target_link_libraries(espasynctcp PUBLIC arduino lwip_gcc)


set(ESP_SRC_DIR "${PROJECT_SOURCE_DIR}/src/hal/esp8266")

set(esp8266_srcs
    ${ESP_SRC_DIR}/stubs/exceptionStubs.cpp
    ${ESP_SRC_DIR}/net/socket/tcpClient_esp.cpp
    ${ESP_SRC_DIR}/serial/serialPort_esp.cpp
    ${ESP_SRC_DIR}/time/sleep_esp.cpp
    ${ESP_SRC_DIR}/fs/file_esp.cpp
    ${ESP_SRC_DIR}/net/http/asyncHttpServer_esp.cpp
    ${ESP_SRC_DIR}/net/socket/tcpClient_esp.cpp
    ${ESP_SRC_DIR}/net/socket/tcpServer_esp.cpp
    ${ESP_SRC_DIR}/net/socket/websocket_esp.cpp
    ${ESP_SRC_DIR}/serial/serialPort_esp.cpp
    ${ESP_SRC_DIR}/time/sleep_esp.cpp
    ${ESP_SRC_DIR}/time/time_esp.cpp
)

set(esp8266_incs
)

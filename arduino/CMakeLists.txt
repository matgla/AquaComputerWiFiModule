add_library(ESP8266_SDK INTERFACE)

# find_library(ESP8266_SDK_LIB_AT at ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_AXTLS axtls ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_CRYPTO crypto ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_ESPNOW espnow ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_HAL hal ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_JSON json ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_LWIP lwip ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_MAIN main ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_NET80211 net80211 ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_PHY phy ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_PP pp ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_PWM pwm ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_SMARTCONFIG smartconfig ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_SSL ssl ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_UPGRADE upgrade ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_WPA wpa ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)
# find_library(ESP8266_SDK_LIB_WPS wps ${ARDUINO_ESP8266_DIR}/tools/sdk/lib)

target_link_libraries(ESP8266_SDK INTERFACE
    m
    c
    gcc
    stdc++
#     ${ESP8266_SDK_LIB_AT}
#     ${ESP8266_SDK_LIB_AXTLS}
#     ${ESP8266_SDK_LIB_CRYPTO}
#     ${ESP8266_SDK_LIB_ESPNOW}
#     ${ESP8266_SDK_LIB_HAL}
#     ${ESP8266_SDK_LIB_JSON}
#     ${ESP8266_SDK_LIB_LWIP}
#     ${ESP8266_SDK_LIB_MAIN}
#     ${ESP8266_SDK_LIB_NET80211}
#     ${ESP8266_SDK_LIB_PHY}
#     ${ESP8266_SDK_LIB_PP}
#     ${ESP8266_SDK_LIB_PWM}
#     ${ESP8266_SDK_LIB_SMARTCONFIG}
#     ${ESP8266_SDK_LIB_SSL}
#     ${ESP8266_SDK_LIB_UPGRADE}
#     ${ESP8266_SDK_LIB_WPA}
#     ${ESP8266_SDK_LIB_WPS}
)

file(GLOB_RECURSE ARDUINO_SRC
    ${ARDUINO_ESP8266_DIR}/cores/esp8266/*.c
    ${ARDUINO_ESP8266_DIR}/cores/esp8266/spiffs/*.c
    ${ARDUINO_ESP8266_DIR}/cores/esp8266/*.cpp
)

file(GLOB ARDUINO_SRC_ASM
    ${ARDUINO_ESP8266_DIR}/cores/esp8266/*.S
)

file(GLOB ARDUINO_INCS
    ${ARDUINO_ESP8266_DIR}/cores/esp8266/*.h
    ${ARDUINO_ESP8266_DIR}/cores/esp8266/*.hpp
)

set_source_files_properties(${ARDUINO_SRC_ASM} PROPERTIES
    LANGUAGE ASM
    COMPILE_FLAGS "${CMAKE_C_FLAGS}"
)

add_library(arduino STATIC
    ${ARDUINO_SRC}
    ${ARDUINO_SRC_ASM}
    ${ARDUINO_INCS}
)

target_include_directories(arduino PUBLIC
    ${ARDUINO_ESP8266_DIR}/cores/esp8266
    ${ARDUINO_ESP8266_DIR}/tools/sdk/include
    ${ARDUINO_ESP8266_DIR}/variants/generic
)

target_compile_definitions(arduino PUBLIC
    -DF_CPU=80000000L
    -DARDUINO=10606
    -DARDUINO_ESP8266_ESP01
    -DARDUINO_ARCH_ESP8266
    -DESP8266
)

target_compile_options(arduino PUBLIC
    -U__STRICT_ANSI__
)

target_link_libraries(arduino PUBLIC
    ESP8266_SDK
)

add_subdirectory(libraries)

# add_custom_target(flash COMMAND ${ESP8266_ESPTOOL} -vv -cd ck -cb 115200 -cp ${ESP8266_ESPTOOL_COM_PORT} -ca 0x00000 -cf firmware.bin)

# add_dependencies(flash firmware_binary)
include(cmake/common_sources.cmake)

set(production_srcs ${common_srcs} ${common_incs} main.cpp)

include_directories(${NLOHMANN_JSON_SOURCE_DIR})
message("Source dir: ${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

#set(target_libs ${target_libs} nlohmann_json)
set(target_name AquaLampServer)

if (${ARCH} STREQUAL "X86")
    include(cmake/x86_sources.cmake)
    find_package(Boost 1.58 COMPONENTS system program_options REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    include_directories(${Beast_INCLUDE_DIR})
    set(production_srcs ${production_srcs} ${x86_srcs} ${x86_incs})

    set(target_libs ${target_libs} ${Boost_LIBRARIES} beast)

    add_definitions(-DX86_ARCH)

    if (WIN32)
        set(target_libs ${target_libs} ws2_32 wsock32)
    elseif (UNIX)
        set(target_libs ${target_libs} pthread)
    endif (WIN32)
elseif (${ARCH} STREQUAL "ESP8266")
    include(cmake/esp8266_sources.cmake)
    set(production_srcs ${production_srcs} ${esp8266_srcs} ${esp8266_incs})

    set(target_libs ${target_libs} arduino)
    include_directories(
        ${ARDUINO_ESP8266_DIR}/cores/esp8266
        ${ARDUINO_ESP8266_DIR}/tools/sdk/include
        ${ARDUINO_ESP8266_DIR}/variants/generic
    )

    set(target_name firmware_lib)
    message("${production_srcs}")

    # set_target_properties(${target_name} PROPERTIES
    #     LINKER_LANGUAGE C
    # )



endif (${ARCH} STREQUAL "X86")

message("${target_name} be created from: ${production_srcs}")
message("linking ${target_name} with ${target_libs} ${common_libs}")

add_library(${target_name} STATIC ${production_srcs})
target_link_libraries(${target_name} PUBLIC ${target_libs} ${common_libs})

file(WRITE null.cpp "")
add_executable(firmware null.cpp)
target_link_libraries(firmware ${target_name})

if (${ARCH} STREQUAL "ESP8266")
    add_custom_target(
        firmware_binary ALL
        COMMAND ${ESP8266_ESPTOOL} -eo \"${ARDUINO_ESP8266_DIR}/bootloaders/eboot/eboot.elf\" -bo firmware.bin -bm dio -bf 40 -bz 4M -bs .text -bp 4096 -ec -eo \"$<TARGET_FILE:firmware>\" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec
    )
    message("${ESP8266_ESPTOOL} -eo \"${ARDUINO_ESP8266_DIR}/bootloaders/eboot/eboot.elf\" -bo firmware.bin -bm dio -bf 40 -bz 4M -bs .text -bp 4096 -ec -eo \"$<TARGET_FILE:firmware>\" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec")
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES firmware.bin)
    add_dependencies(firmware_binary firmware)
    message("binary ${PROJECT_BINARY_DIR}")
    add_custom_target(flash COMMAND ${ESP8266_ESPTOOL} -vv -cd nodemcu -cb 460800 -cp ${ESP8266_ESPTOOL_COM_PORT} -cf firmware.bin)
    add_dependencies(flash firmware_binary)

    message("${XTENSA_TOOLCHAIN_PATH}")
    add_custom_command(TARGET firmware
        POST_BUILD
        COMMAND ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-size -B -d firmware.elf
    )
endif (${ARCH} STREQUAL "ESP8266")

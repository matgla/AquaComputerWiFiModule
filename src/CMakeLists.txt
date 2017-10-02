include(cmake/common_sources.cmake)

set(production_srcs ${common_srcs} ${common_incs} main.cpp)

include_directories("${PROJECT_SOURCE_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(target_libs ${target_libs} ArduinoJson sml crcpp)
set(target_name AquaLampServer)
set(target_name_lib firmware)

if (${ARCH} STREQUAL "X86")
    include(cmake/x86_sources.cmake)
    find_package(Boost 1.58 COMPONENTS system program_options REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    set(production_srcs ${production_srcs} ${x86_srcs} ${x86_incs})

    set(target_libs ${target_libs} ${Boost_LIBRARIES} beast stdc++fs)

    add_definitions(-DX86_ARCH)

    if (WIN32)
        set(target_libs ${target_libs} ws2_32 wsock32)
    elseif (UNIX)
        set(target_libs ${target_libs} pthread)
    endif (WIN32)
elseif (${ARCH} STREQUAL "ESP8266")
    add_definitions(-DESP8266_ARCH)
    include(cmake/esp8266_sources.cmake)
    set(production_srcs ${production_srcs} ${esp8266_srcs} ${esp8266_incs})
    set(target_libs ${target_libs} arduino espasynctcp)
    include_directories(
        ${ARDUINO_ESP8266_DIR}/cores/esp8266
        ${ARDUINO_ESP8266_DIR}/tools/sdk/include
        ${ARDUINO_ESP8266_DIR}/variants/generic
    )

endif (${ARCH} STREQUAL "X86")

add_library(${target_name_lib} STATIC ${production_srcs})
set(clang_tidy_srcs ${clang_tidy_srcs} ${common_srcs} ${x86_srcs} CACHE INTERNAL "clang_tidy_srcs")

target_link_libraries(${target_name_lib} PUBLIC ${target_libs} ${common_libs})

file(WRITE ${PROJECT_BINARY_DIR}/null.cpp "")
add_executable(${target_name} ${PROJECT_BINARY_DIR}/null.cpp)
target_link_libraries(${target_name} PUBLIC ${target_name_lib})

if (${ARCH} STREQUAL "ESP8266")
    add_custom_command(TARGET ${target_name}
        POST_BUILD
        COMMAND ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-size -B -d ${target_name}${CMAKE_EXECUTABLE_SUFFIX}
    )

    add_custom_target(
        firmware_binary ALL
        COMMAND ${ESP8266_ESPTOOL} -eo \"${ARDUINO_ESP8266_DIR}/bootloaders/eboot/eboot.elf\" -bo ${target_name}.bin -bm dio -bf 40 -bz 4M -bs .text -bp 4096 -ec -eo \"$<TARGET_FILE:${target_name}>\" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec
    )
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${target_name}.bin)
    add_dependencies(firmware_binary ${target_name})
    add_custom_target(flash COMMAND ${ESP8266_ESPTOOL} -vv -cd nodemcu -cb 460800 -cp ${ESP8266_ESPTOOL_COM_PORT} -cf ${target_name}.bin)
    add_dependencies(flash firmware_binary)
endif (${ARCH} STREQUAL "ESP8266")

set(CMAKE_SYSTEM_NAME       ESP8266)
set(CMAKE_SYSTEM_VERSION    1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

file(TO_CMAKE_PATH $ENV{XTENSA_TOOLCHAIN} XTENSA_TOOLCHAIN_PATH_CONVERTED)
set(XTENSA_TOOLCHAIN_PATH ${XTENSA_TOOLCHAIN_PATH_CONVERTED} CACHE PATH "Path to Xtensa toolchain" FORCE)
message("-- Xtensa toolchain: ${XTENSA_TOOLCHAIN_PATH}")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

if (WIN32)
    set(CMAKE_ASM_COMPILER ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-gcc.exe)
    set(CMAKE_C_COMPILER ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-gcc.exe)
    set(CMAKE_CXX_COMPILER ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-g++.exe)
elseif(UNIX)
    set(CMAKE_ASM_COMPILER ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-gcc)
    set(CMAKE_C_COMPILER ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-gcc)
    set(CMAKE_CXX_COMPILER ${XTENSA_TOOLCHAIN_PATH}/xtensa-lx106-elf-g++)
endif (WIN32)

file(TO_CMAKE_PATH $ENV{ARDUINO_ESP8266_ROOT} ARDUINO_ESP8266_ROOT_CONVERTED)
set(ARDUINO_ESP8266_DIR ${ARDUINO_ESP8266_ROOT_CONVERTED} CACHE PATH "Path to ESP8266 Arduino" FORCE)

if (ARDUINO_ESP8266_DIR STREQUAL "")
    message(FATAL_ERROR "Add ARDUINO_ESP8266_ROOT to environment variables.")
endif ()

set (ESP8266_FLASH_SIZE "4M" CACHE STRING "Size of flash" FORCE)
set (ESP8266_LINKER_SCRIPT "eagle.flash.4m.ld" CACHE STRING "Used linker script" FORCE)
set (ESP8266_ESPTOOL ${ARDUINO_ESP8266_DIR}/tools/esptool/esptool)
set (ESP8266_ESPTOOL_COM_PORT "COM6")
set(CMAKE_BUILD_TYPE Release CACHE STRING "")

link_directories(${ARDUINO_ESP8266_DIR}/tools/sdk/lib)

set(LINKER_SCRIPTS_PATH "${PROJECT_SOURCE_DIR}/ld" CACHE STRING "Path to linker scripts directory" FORCE)

set(C_OPTIMIZATION_FLAGS "-Os -fno-threadsafe-statics")
set(C_WARNING_FLAGS "-Wpointer-arith")
set(C_ERROR_FLAGS "")

set(CMAKE_C_FLAGS "-std=gnu99 -Wpointer-arith -Wl,-EL -Wno-implicit-function-declaration -fno-inline-functions -nostdlib -Os -mlongcalls -mtext-section-literals -falign-functions=4 -ffunction-sections -fdata-sections -U__STRICT_ANSI__" CACHE STRING "C compiler flags" FORCE)
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -std=c++11 -Os -mlongcalls -mtext-section-literals -falign-functions=4 -ffunction-sections -fdata-sections -U__STRICT_ANSI__" CACHE STRING "CXX compiler flags" FORCE)
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp -Os -mlongcalls -mtext-section-literals -falign-functions=4 -U__STRICT_ANSI__ -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "-Os -nostdlib -Wl,--no-check-sections -u call_user_start -u _printf_float -u _scanf_float -Wl,-static -Wl,--gc-sections -T${ESP8266_LINKER_SCRIPT} -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read " CACHE STRING "Linker flags" FORCE)

link_directories(
    ${ARDUINO_ESP8266_DIR}/tools/sdk/lib
    ${ARDUINO_ESP8266_DIR}/tools/sdk/ld
    ${ARDUINO_ESP8266_DIR}/tools/sdk/libc/xtensa-lx106-elf/lib
)
set(ESP8266_SDK_LIBRARIES "-lmesh -lwpa2 -lsmartconfig -lespnow -lpp -lmain -lwpa -llwip_gcc -lnet80211 -lwps -lcrypto -lphy -lhal -laxtls -lgcc -lm -lc -lstdc++")

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> -o <TARGET> <LINK_FLAGS> <OBJECTS> -Wl,--start-group <LINK_LIBRARIES> ${ESP8266_SDK_LIBRARIES} -Wl,--end-group")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> -o <TARGET> <LINK_FLAGS> <OBJECTS> -Wl,--start-group <LINK_LIBRARIES> ${ESP8266_SDK_LIBRARIES} -Wl,--end-group")

add_definitions(
    -DARDUINO=10600
    -DLWIP_OPEN_SRC
    -DF_CPU=160000000L
    -D__ets__
    -DICACHE_FLASH
    -DESP8266
    -DARDUINO_ARCH_ESP8266
    -DARDUINO_ESP8266_NODEMCU
)


include_directories( SYSTEM
    ${ARDUINO_ESP8266_DIR}/tools/sdk/include
    ${ARDUINO_ESP8266_DIR}/tools/sdk/lwip/include
    ${ARDUINO_ESP8266_DIR}/tools/sdk/libc/xtensa-lx106-elf
    ${ARDUINO_ESP8266_DIR}/tools/sdk/libc/xtensa-lx106-elf/include
)

set(CMAKE_EXECUTABLE_SUFFIX .elf)
set(CMAKE_BUILD_TYPE Release FORCE)

set(CMAKE_SYSTEM_NAME       Generic)
set(CMAKE_SYSTEM_VERSION    1)
set(CMAKE_SYSTEM_PROCESSOR  arm-eabi)

file(TO_CMAKE_PATH $ENV{XTENSA_TOOLCHAIN} XTENSA_TOOLCHAIN_PATH_CONVERTED)
set(XTENSA_TOOLCHAIN_PATH ${XTENSA_TOOLCHAIN_PATH_CONVERTED} CACHE PATH "Path to Xtensa toolchain" FORCE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_ASM_COMPILER xtensa-lx106-elf-gcc)
set(CMAKE_C_COMPILER xtensa-lx106-elf-gcc)
set(CMAKE_CXX_COMPILER xtensa-lx106-elf-g++)

file(TO_CMAKE_PATH $ENV{ARDUINO_ESP8266_ROOT} ARDUINO_ESP8266_ROOT_CONVERTED)
set(ARDUINO_ESP8266_DIR ${ARDUINO_ESP8266_ROOT_CONVERTED} CACHE PATH "Path to ESP8266 Arduino" FORCE)

if (ARDUINO_ESP8266_DIR STREQUAL "")
    message(FATAL_ERROR "Add ARDUINO_ESP8266_ROOT to environment variables.")
endif ()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/Modules")

set (ESP8266_FLASH_SIZE "2m" CACHE STRING "Size of flash")
set (ESP8266_LINKER_SCRIPT "eagle.flash.${ESP8266_FLASH_SIZE}.ld")

set(CMAKE_BUILD_TYPE Release CACHE STRING "")

link_directories(${ARDUINO_ESP8266_DIR}/tools/sdk/lib)

set(CMAKE_C_FLAGS "-std=gnu99 -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -Os -mlongcalls -mtext-section-literals -falign-functions=4 -ffunction-sections -fdata-sections -fno-exceptions -Wall -Wextra -Os -DLWIP_OPEN_SRC -D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__" CACHE STRING "C compiler flags" FORCE)
set(CMAKE_CXX_FLAGS "-fno-rtti -fno-exceptions -std=c++1y -Os -mlongcalls -mtext-section-literals -falign-functions=4 -ffunction-sections -fdata-sections -fno-exceptions -Wall -Wextra -Os -DLWIP_OPEN_SRC -D__ets__ -DICACHE_FLASH -DPLATFORMIO=30500 -U__STRICT_ANSI__" CACHE STRING "CXX compiler flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "-Os -nostdlib -Wl,--no-check-sections -u call_user_start -u _printf_float -u _scanf_float -Wl,-static -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read -L\"${ARDUINO_ESP8266_DIR}/tools/sdk/ld\" -Teagle.flash.${ESP8266_FLASH_SIZE}.ld -Wl,--start-group -lmesh -lwpa2 -lsmartconfig -lespnow -lpp -lmain -lwpa -llwip_gcc -lnet80211 -lwps -lcrypto -lphy -lhal -laxtls -lgcc -lm -lc -lstdc++ -Wl,--end-group " CACHE STRING "linker flags" FORCE)

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> -Wl,--start-group <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group" CACHE STRING "C linker invocation")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> -Wl,--start-group <OBJECTS> <LINK_LIBRARIES> -Wl,--end-group" CACHE STRING "CXX linker invocation")

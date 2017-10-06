cmake_minimum_required(VERSION 3.1)

set(ARCH "X86" CACHE STRING "Target architecture (X86|ESP8266)")
set(BUILD_TESTS OFF CACHE STRING "Build all available tests")
set(BUILD_TARGET OFF CACHE STRING "Build target")

set(ASAN_ENABLED OFF CACHE STRING "Enable address sanitizer")
set(LSAN_ENABLED OFF CACHE STRING "Enable leak sanitizer")
set(TSAN_ENABLED OFF CACHE STRING "Enable thread sanitizer")
set(USAN_ENABLED OFF CACHE STRING "Enable undefined sanitizer")
set(CLANG_TIDY   OFF CACHE STRING "Enable clang tidy")
set(CPPCHECK     OFF CACHE STRING "Enable cppcheck")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (${ARCH} STREQUAL "ESP8266")
    message("-- Include toolchain for ESP8266")
    include("cmake/ESP8266Toolchain.cmake")
endif (${ARCH} STREQUAL "ESP8266")

project(AquaLampServer C CXX ASM)

if (ASAN_ENABLED)
    if (${ARCH} STREQUAL "ESP8266")
        message(WARNING "ESP8266 not support address sanitizer")
    endif (${ARCH} STREQUAL "ESP8266")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif (ASAN_ENABLED)

if (LSAN_ENABLED)
    if (${ARCH} STREQUAL "ESP8266")
        message(WARNING "ESP8266 not support leak sanitizer")
    endif (${ARCH} STREQUAL "ESP8266")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
endif (LSAN_ENABLED)

if (TSAN_ENABLED)
    if (${ARCH} STREQUAL "ESP8266")
        message(WARNING "ESP8266 not support thread sanitizer")
    endif (${ARCH} STREQUAL "ESP8266")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif (TSAN_ENABLED)

if(ENABLE_USAN)
    if (${ARCH} STREQUAL "ESP8266")
        message(WARNING "ESP8266 not support undefined behaviour sanitizer")
    endif (${ARCH} STREQUAL "ESP8266")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
endif()

set (clang_tidy_srcs "" CACHE INTERNAL "clang tidy sources")

if (CLANG_TIDY)
    find_program(CLANG_TIDY_BIN clang-tidy-5.0)

    if(CLANG_TIDY_BIN STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
        message(FATAL_ERROR "unable to locate clang-tidy-5.0")
    endif()

    set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)

endif (CLANG_TIDY)

if(CPPCHECK)

    list(APPEND CPPCHECK_CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
    )

    ExternalProject_Add(
        cppcheck
        GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
        GIT_TAG             1.79
        GIT_SHALLOW         1
        CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
        PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
        TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
        STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
        DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
        SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
        BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS
        --enable=warning,style,performance,portability,unusedFunction
        --std=c++11
        --verbose
        --error-exitcode=1
        --language=c++
        -DMAIN=main
        -I ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/test/*.cpp
        ${CMAKE_SOURCE_DIR}/test/*.h
        ${CMAKE_SOURCE_DIR}/test/*.h
        ${CMAKE_SOURCE_DIR}/test/*.c
    )

    add_custom_target(
        check
        COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
        COMMENT "running cppcheck"
    )

endif()

set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --track-fds=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes")
set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1")

if (GCOV)
    if (${ARCH} STREQUAL "ESP8266")
        message(WARNING "ESP8266 not support coverage")
    endif (${ARCH} STREQUAL "ESP8266")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif (GCOV)

if (BUILD_TARGET)
    add_subdirectory(src)
endif (BUILD_TARGET)

if (BUILD_TESTS)
    message("Building Tests")
    if (NOT ${ARCH} STREQUAL "X86")
        message(FATAL_ERROR "Tests can be build only for X86")
    endif (NOT ${ARCH} STREQUAL "X86")

    add_subdirectory(test)
endif (BUILD_TESTS)

add_subdirectory(lib)

if (CLANG_TIDY)
    add_custom_target(
        clang_tidy
        COMMAND ${CLANG_TIDY_BIN} -checks=clan*,cert*,misc*,perf*,cppc*,read*,mode*,-cert-err58-cpp,-cert-dcl58-cpp,-misc-noexcept-move-constructor -header-filter="src" -p . ${clang_tidy_srcs}
        COMMENT "running clang tidy"
    )
endif (CLANG_TIDY)

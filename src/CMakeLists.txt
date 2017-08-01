include(cmake/common_sources.cmake)

set(production_srcs ${common_srcs} ${common_incs})

include_directories("${PROJECT_SOURCE_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

if (${ARCH} STREQUAL "X86")
    include(cmake/x86_sources.cmake)
    find_package(Boost 1.58 COMPONENTS system program_options REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    message("Beast libs: ${Beast_INCLUDE_DIR}")
    include_directories(${Beast_INCLUDE_DIR})
    include_directories(${WEBSOCKETPP_ROOT})
    set(production_srcs ${production_srcs} ${x86_srcs} ${x86_incs})

    set(target_libs ${target_libs} ${Boost_LIBRARIES} beast)

    add_definitions(-DX86_ARCH)

    if (WIN32)
        set(target_libs ${target_libs} ws2_32 wsock32)
    elseif (UNIX)
        set(target_libs ${target_libs} pthread)
    endif (WIN32)
elseif (${ARCH} strequal "ESP8266")
    message("ESP8266 is not currently supported")
endif (${ARCH} STREQUAL "X86")

set(target_libs ${target_libs} nlohmann_json)

set(target_name AquaLampServer)

add_executable(${target_name} ${production_srcs})

target_link_libraries(${target_name} ${target_libs} ${common_libs})

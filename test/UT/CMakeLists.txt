enable_testing()

include(cmake/ut_sources.cmake)
include(${PROJECT_SOURCE_DIR}/src/cmake/common_sources.cmake)
include(${PROJECT_SOURCE_DIR}/src/cmake/x86_sources.cmake)

include_directories(src)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/test/UT/src)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
find_package(Boost 1.58 COMPONENTS system program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
include_directories(${Beast_INCLUDE_DIR})
set(target_libs ${target_libs} ${Boost_LIBRARIES} beast ArduinoJson stdc++fs)

add_definitions(-DX86_ARCH)

if (WIN32)
    set(target_libs ${target_libs} ws2_32 wsock32)
elseif (UNIX)
    set(target_libs ${target_libs} pthread)
endif (WIN32)

add_executable(AquaLampServerUTs ${ut_srcs}
                                 ${ut_incs}
                                 ${common_srcs}
                                 ${common_incs}
                                 ${x86_srcs}
                                 ${x86_incs}
)

set(testing_libs ${testing_libs} gmock gtest pthread)

target_link_libraries(AquaLampServerUTs ${target_libs} ${common_libs} ${testing_libs})

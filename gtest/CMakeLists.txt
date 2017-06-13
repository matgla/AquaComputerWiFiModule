cmake_minimum_required (VERSION 2.8)

project(AquaServerTests)

include_directories(UnitTest++)
add_definitions(-DUNIT_TEST)

enable_testing()

file (GLOB SRCS
    runner.cpp
    pathConverterTests.cpp)

file (GLOB_RECURSE ADAPTER_SRCS ${PROJECT_SOURCE_DIR}/adapters/*.c
                                ${PROJECT_SOURCE_DIR}/adapters/*.cpp
                                ${PROJECT_SOURCE_DIR}/adapters/*.h
                                ${PROJECT_SOURCE_DIR}/adapters/*.hpp)

file (GLOB_RECURSE UNITTEST_SRCS ${PROJECT_SOURCE_DIR}/UnitTest++/*.c
                                 ${PROJECT_SOURCE_DIR}/UnitTest++/*.cpp
                                 ${PROJECT_SOURCE_DIR}/UnitTest++/*.h
                                 ${PROJECT_SOURCE_DIR}/UnitTest++/*.hpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

file (GLOB PROD_SRCS    ${PROJECT_SOURCE_DIR}/../src/helper/pathConverter.hpp
                        ${PROJECT_SOURCE_DIR}/../src/helper/pathConverter.cpp)

add_library (unittestlib ${UNITTEST_SRCS})

include_directories(${PROJECT_SOURCE_DIR}/../src)
include_directories(${PROJECT_SOURCE_DIR}/adapters)

add_executable (AquaServer_UTs ${SRCS} ${PROD_SRCS} ${STUB_SRCS} ${ADAPTER_SRCS})

target_link_libraries(AquaServer_UTs unittestlib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

enable_testing()

include(cmake/ut_sources.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/common_sources.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/x86_sources.cmake)

include_directories(src)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(AquaLampServerUTs ${ut_srcs} 
                                 ${ut_incs}
                                 ${common_srcs} 
                                 ${common_incs}
                                 ${x86_srcs} 
                                 ${x86_incs}
)

target_link_libraries(AquaLampServerUTs gmock gtest gtest_main pthread ${x86_libs})

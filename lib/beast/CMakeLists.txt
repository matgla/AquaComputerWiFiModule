file(GLOB_RECURSE beast_srcs "*.hpp" "*.h")

set(Beast_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/beast/src" CACHE STRING "path to Beast sources")

add_library(beast INTERFACE)
target_sources(beast INTERFACE ${beast_srcs})
target_include_directories(beast INTERFACE src/)
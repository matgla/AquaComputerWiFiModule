file(GLOB_RECURSE beast_srcs "*.hpp" "*.h")

set(Beast_INCLUDE_DIR "src")

add_library(beast INTERFACE)
target_sources(beast INTERFACE ${beast_srcs})
target_include_directories(beast INTERFACE src/)
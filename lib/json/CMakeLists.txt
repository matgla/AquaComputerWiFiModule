file(GLOB_RECURSE arduino_json_srcs "json/src/*.hpp" "json/src/*.cpp" "json/src/*.h" "json/src/*.c")

set(ArduinoJson_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/json/json/src" CACHE STRING "Path to ArduinoJson sources")

add_library(ArduinoJson STATIC ${arduino_json_srcs})

target_include_directories(ArduinoJson PUBLIC
    ${ArduinoJson_INCLUDE_DIR}
)

set_target_properties(ArduinoJson PROPERTIES LINKER_LANGUAGE CXX)

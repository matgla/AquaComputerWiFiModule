file(GLOB_RECURSE boost_sml_srcs
    "sml/include/*.hpp"
    "sml/include/*.h"
    "sml/include/*.ipp"
)

set(SML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/boost/sml/include/" CACHE STRING "path to Boost SML sources" FORCE)

add_library(sml INTERFACE)
target_sources(sml INTERFACE ${boost_sml_srcs})
target_include_directories(sml INTERFACE ${SML_INCLUDE_DIR})

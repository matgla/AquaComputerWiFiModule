file(GLOB_RECURSE crcpp_srcs
    "*.hpp"
    "*.h"
    "*.ipp")

set(CRCpp_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/CRCpp/CRCpp/inc" CACHE STRING "path to CRCpp sources" FORCE)

add_library(crcpp INTERFACE)
target_sources(crcpp INTERFACE ${crcpp_srcs})
target_include_directories(crcpp SYSTEM INTERFACE ${CRCpp_INCLUDE_DIR})

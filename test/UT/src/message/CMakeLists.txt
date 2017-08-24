set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/lib/nanopb/extra)
set(PROTOBUF_PROTOC_EXECUTABLE ${PROJECT_SOURCE_DIR}/lib/nanopb/generator-bin/protoc)
find_package(Nanopb REQUIRED)
include(proto_sources.cmake)
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${proto_sources})

set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS}
    PROPERTIES GENERATED TRUE
)

add_library(testing_messages STATIC ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(testing_messages PUBLIC ${NANOPB_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

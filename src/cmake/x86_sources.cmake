set(X86_SRC_DIR "${PROJECT_SOURCE_DIR}/src/hal/x86")

set(x86_srcs
    ${X86_SRC_DIR}/net/tcpServer_x86.cpp
    ${X86_SRC_DIR}/net/tcpClient_x86.cpp
    ${X86_SRC_DIR}/net/websocket_x86.cpp
    ${X86_SRC_DIR}/net/http/asyncHttpServer_x86.cpp
    ${X86_SRC_DIR}/net/http/httpConnection_x86.cpp
    ${X86_SRC_DIR}/serial/serialPort_x86.cpp
    ${X86_SRC_DIR}/fs/file_x86.cpp
    ${X86_SRC_DIR}/time/sleep_x86.cpp
)

set(x86_incs
    ${X86_SRC_DIR}/net/http/httpConnection_x86.hpp
)

set(X86_SRC_DIR "${PROJECT_SOURCE_DIR}/src/hal/x86")

set(target_srcs
    ${X86_SRC_DIR}/fs/file_x86.cpp
    ${X86_SRC_DIR}/fs/filesystem_x86.cpp
    ${X86_SRC_DIR}/net/http/asyncHttpServer_x86.cpp
    ${X86_SRC_DIR}/net/http/httpConnection_x86.cpp
    ${X86_SRC_DIR}/net/socket/tcpClient_x86.cpp
    ${X86_SRC_DIR}/net/socket/tcpServer_x86.cpp
    ${X86_SRC_DIR}/net/socket/tcpSession.cpp
    ${X86_SRC_DIR}/net/socket/websocket_x86.cpp
    ${X86_SRC_DIR}/serial/serialPort_x86.cpp
    ${X86_SRC_DIR}/time/sleep_x86.cpp
)

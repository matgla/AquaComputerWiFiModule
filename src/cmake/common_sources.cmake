set(COMMON_SRC_DIR "${PROJECT_SOURCE_DIR}/src")

set(common_srcs
    ${COMMON_SRC_DIR}/logger/logger.cpp
    ${COMMON_SRC_DIR}/logger/loggerBase.cpp
    ${COMMON_SRC_DIR}/logger/loggerConf.cpp
    ${COMMON_SRC_DIR}/logger/socketLogger.cpp
    ${COMMON_SRC_DIR}/logger/stdOutLogger.cpp
    ${COMMON_SRC_DIR}/stream/socketBuffer.cpp
    ${COMMON_SRC_DIR}/stream/socketStream.cpp
    ${COMMON_SRC_DIR}/main.cpp
)

set(common_incs
    ${COMMON_SRC_DIR}/hal/net/tcpSocket.hpp
    ${COMMON_SRC_DIR}/hal/net/http/asyncHttpRequest.hpp
    ${COMMON_SRC_DIR}/hal/net/http/asyncHttpServer.hpp
    ${COMMON_SRC_DIR}/hal/net/http/fwd.hpp
    ${COMMON_SRC_DIR}/hal/net/http/httpMethod.hpp
    ${COMMON_SRC_DIR}/hal/serial/serialPort.hpp
    ${COMMON_SRC_DIR}/logger/ILogger.hpp
    ${COMMON_SRC_DIR}/logger/logger.hpp
    ${COMMON_SRC_DIR}/logger/loggerConf.hpp
    ${COMMON_SRC_DIR}/logger/loggerBase.hpp
    ${COMMON_SRC_DIR}/logger/socketLogger.hpp
    ${COMMON_SRC_DIR}/logger/stdOutLogger.hpp
    ${COMMON_SRC_DIR}/stream/socketBuffer.hpp
    ${COMMON_SRC_DIR}/stream/socketStream.hpp
    ${COMMON_SRC_DIR}/utils/types.hpp
)

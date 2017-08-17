set(COMMON_SRC_DIR "${PROJECT_SOURCE_DIR}/src")

set(common_srcs
     #${COMMON_SRC_DIR}/handler/handlers.cpp
     #${COMMON_SRC_DIR}/handler/baseHandler.cpp
     #${COMMON_SRC_DIR}/handler/dispatcher.cpp
     #${COMMON_SRC_DIR}/logger/logger.cpp
     #${COMMON_SRC_DIR}/logger/loggerBase.cpp
     #${COMMON_SRC_DIR}/logger/loggerConf.cpp
     #${COMMON_SRC_DIR}/logger/socketLogger.cpp
     #${COMMON_SRC_DIR}/logger/stdOutLogger.cpp
     #${COMMON_SRC_DIR}/stream/socketBuffer.cpp
     #${COMMON_SRC_DIR}/stream/socketStream.cpp
)

set(common_incs
    # ${COMMON_SRC_DIR}/container/buffer.hpp
    # ${COMMON_SRC_DIR}/hal/fs/file.hpp
    # ${COMMON_SRC_DIR}/hal/net/http/asyncHttpRequest.hpp
    # ${COMMON_SRC_DIR}/hal/net/http/asyncHttpServer.hpp
    # ${COMMON_SRC_DIR}/hal/net/http/fwd.hpp
    # ${COMMON_SRC_DIR}/hal/net/http/httpMethod.hpp
    # ${COMMON_SRC_DIR}/hal/net/socket/tcpClient.hpp
    # ${COMMON_SRC_DIR}/hal/net/socket/tcpServer.hpp
    # ${COMMON_SRC_DIR}/hal/net/socket/websocket.hpp
     ${COMMON_SRC_DIR}/hal/serial/serialPort.hpp
     ${COMMON_SRC_DIR}/hal/time/sleep.hpp
    # ${COMMON_SRC_DIR}/handler/baseHandler.hpp
    # ${COMMON_SRC_DIR}/handler/dispatcher.hpp
    # ${COMMON_SRC_DIR}/handler/IDataReceiver.hpp
    # ${COMMON_SRC_DIR}/handler/handlers.hpp
    # ${COMMON_SRC_DIR}/logger/ILogger.hpp
    # ${COMMON_SRC_DIR}/logger/logger.hpp
    # ${COMMON_SRC_DIR}/logger/loggerBase.hpp
    # ${COMMON_SRC_DIR}/logger/loggerConf.hpp
    # ${COMMON_SRC_DIR}/logger/socketLogger.hpp
    # ${COMMON_SRC_DIR}/logger/stdOutLogger.hpp
    # ${COMMON_SRC_DIR}/message/messages.hpp
    # ${COMMON_SRC_DIR}/serializer/serializer.hpp
    # ${COMMON_SRC_DIR}/stream/socketBuffer.hpp
    # ${COMMON_SRC_DIR}/stream/socketStream.hpp
    ${COMMON_SRC_DIR}/utils/types.hpp
)

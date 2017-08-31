set(UT_SRC_DIR "${PROJECT_SOURCE_DIR}/test/UT/src")

set(ut_srcs
    ${UT_SRC_DIR}/test/handler/messageHandlerTests.cpp
    ${UT_SRC_DIR}/test/serializer/serializerTests.cpp
    ${UT_SRC_DIR}/test/handler/dispatcherTests.cpp
    ${UT_SRC_DIR}/test/handler/jsonHandlerTests.cpp
    ${UT_SRC_DIR}/test/testMain.cpp
    
)

set(ut_incs
    ${UT_SRC_DIR}/matcher/arrayCompare.hpp
    ${UT_SRC_DIR}/matcher/messageIdCompare.hpp
    ${UT_SRC_DIR}/mock/writerHandlerMock.hpp
    ${UT_SRC_DIR}/mock/frameReceiverMock.hpp
    ${UT_SRC_DIR}/mock/rawDataReceiverMock.hpp
    ${UT_SRC_DIR}/mock/frameHandlerMock.hpp
    ${UT_SRC_DIR}/mock/messageHandlerMock.hpp
)

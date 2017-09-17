set(UT_SRC_DIR "${PROJECT_SOURCE_DIR}/test/UT/src")

set(ut_srcs
    ${UT_SRC_DIR}/test/dispatcher/messageHandlerTests.cpp
    ${UT_SRC_DIR}/test/serializer/serializerTests.cpp
    ${UT_SRC_DIR}/test/dispatcher/dispatcherTests.cpp
    ${UT_SRC_DIR}/test/dispatcher/jsonHandlerTests.cpp
    ${UT_SRC_DIR}/test/protocol/frameTests.cpp
    ${UT_SRC_DIR}/test/protocol/frameHandlerTests.cpp
    ${UT_SRC_DIR}/test/testMain.cpp

)

set(ut_incs
    ${UT_SRC_DIR}/matcher/arrayCompare.hpp
    ${UT_SRC_DIR}/matcher/messageIdCompare.hpp
    ${UT_SRC_DIR}/mock/writerHandlerMock.hpp
    ${UT_SRC_DIR}/mock/rawDataReceiverMock.hpp
    ${UT_SRC_DIR}/mock/messageHandlerMock.hpp
    ${UT_SRC_DIR}/stub/receiverStub.hpp
)

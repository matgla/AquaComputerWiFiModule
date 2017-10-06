set(UT_SRC_DIR "${PROJECT_SOURCE_DIR}/test/UT/src")

set(ut_srcs
    ${UT_SRC_DIR}/test/serializer/serializerTests.cpp
    ${UT_SRC_DIR}/test/dispatcher/dispatcherTests.cpp
    ${UT_SRC_DIR}/test/dispatcher/jsonHandlerTests.cpp
    ${UT_SRC_DIR}/test/protocol/frameTests.cpp
    ${UT_SRC_DIR}/test/protocol/frameHandlerTests.cpp
    ${UT_SRC_DIR}/test/protocol/packetHandlerTests.cpp
    ${UT_SRC_DIR}/test/timer/intervalTimerTests.cpp
    ${UT_SRC_DIR}/test/timer/managerTests.cpp
    ${UT_SRC_DIR}/test/timer/timeoutTimerTests.cpp
    ${UT_SRC_DIR}/test/testMain.cpp

    ${UT_SRC_DIR}/stub/timeStub.cpp
    ${UT_SRC_DIR}/helper/frameHelper.cpp

)

set(ut_incs
    ${UT_SRC_DIR}/matcher/arrayCompare.hpp
    ${UT_SRC_DIR}/matcher/messageIdCompare.hpp
    ${UT_SRC_DIR}/mock/writerHandlerMock.hpp
    ${UT_SRC_DIR}/mock/rawDataReceiverMock.hpp
    ${UT_SRC_DIR}/stub/receiverStub.hpp
    ${UT_SRC_DIR}/stub/timeStub.hpp
    ${UT_SRC_DIR}/helper/frameHelper.hpp
)

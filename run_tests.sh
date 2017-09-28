#!/bin/sh

git submodule update --init
if [ $RESULT -neq 0]; then
    return -1
fi

mkdir -p bin_test
if [ $RESULT -neq 0]; then
    return -1
fi

cd bin_test
if [ $RESULT -neq 0]; then
    return -1
fi

cmake .. -DBUILD_TESTS=ON -DARCH=X86 -GNinja
if [ $RESULT -neq 0]; then
    return -1
fi

ninja
if [ $RESULT -neq 0]; then
    return -1
fi

test/UT/AquaLampServerUTs

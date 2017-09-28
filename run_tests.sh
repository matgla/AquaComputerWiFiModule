#!/bin/sh

git submodule update --init
if [ $? -neq 0]; then
    return -1
fi

mkdir -p bin_test
if [ $? -neq 0]; then
    return -1
fi

cd bin_test
if [ $? -neq 0]; then
    return -1
fi

cmake .. -DBUILD_TESTS=ON -DARCH=X86 -GNinja
if [ $? -neq 0]; then
    return -1
fi

ninja
if [ $? -neq 0]; then
    return -1
fi

test/UT/AquaLampServerUTs

#!/bin/sh

mkdir -p bin_test
if [ $? -ne 0 ]; then
    return -1
fi

cd bin_test
if [ $? -ne 0 ]; then
    return -1
fi

cmake .. -DBUILD_TESTS=ON -DGCOV=ON -DARCH=X86 -GNinja
if [ $? -ne 0 ]; then
    return -1
fi

ninja
if [ $? -ne 0 ]; then
    return -1
fi

chmod -R 777 .

test/UT/AquaLampServerUTs

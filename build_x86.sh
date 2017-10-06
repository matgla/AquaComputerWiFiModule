#!/bin/sh

mkdir -p bin_x86
if [ $? -ne 0 ]; then
    return -1
fi

cd bin_x86
if [ $? -ne 0 ]; then
    return -1
fi

cmake .. -DBUILD_TARGET=ON -DARCH=X86 -GNinja
if [ $? -ne 0 ]; then
    return -1
fi

ninja
if [ $? -ne 0 ]; then
    return -1
fi

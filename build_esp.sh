#!/bin/sh

mkdir -p bin_esp
if [ $? -ne 0 ]; then
    return -1
fi

cd bin_esp
if [ $? -ne 0 ]; then
    return -1
fi

source ~/esp_sdk.env && cmake .. -DBUILD_TARGET=ON -DARCH=ESP8266 -GNinja
if [ $? -ne 0 ]; then
    return -1
fi

source ~/esp_sdk.env && ninja
if [ $? -ne 0 ]; then
    return -1
fi

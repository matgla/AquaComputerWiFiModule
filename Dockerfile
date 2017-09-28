# base image
FROM alpine:latest

# install base toolchain

RUN apk add --update
RUN apk add --no-cache gcc g++ cmake
RUN apk add --no-cache python python-dev py-pip build-base
RUN apk add --no-cache boost boost-dev
RUN apk add --no-cache git
RUN apk add --no-cache ninja make openssl


RUN mkdir -p /opt

# install esp8266 toolchain

RUN cd /opt && git clone https://github.com/esp8266/Arduino.git esp8266_arduino
RUN cd /opt/esp8266_arduino/tools && python get.py

# create toolchain environment configuration file

RUN touch ~/esp_sdk.env
RUN echo 'export ARDUINO_ESP8266_ROOT="/opt/esp8266_arduino"' >> ~/esp_sdk.env
RUN echo 'export XTENSA_TOOLCHAIN="/opt/esp8266_arduino/tools/xtensa-lx106-elf/bin"' >> ~/esp_sdk.env

RUN source ~/esp_sdk.env

# create virtual serial port

RUN apk add --no-cache socat

RUN echo '#!/bin/bash' >> /usr/bin/startup.sh
RUN echo 'socat PTY,link=/dev/ttyS10,raw PTY,link=/dev/ttyS11,raw &' >> /usr/bin/startup.sh
RUN echo 'sleep 1' >> /usr/bin/startup.sh
RUN echo 'stty -F /dev/ttyS10 -onlcr && stty -F /dev/ttyS11 -onlcr' >> /usr/bin/startup.sh
RUN chmod +x /usr/bin/startup.sh

CMD /usr/bin/startup.sh

# CI configuration
RUN apk add --no-cache libffi-dev libffi openssl-dev
RUN pip install --user cpp-coveralls

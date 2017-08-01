if (${ARCH} STREQUAL "X86")
    add_subdirectory(beast)
    add_subdirectory(websocketpp)
    add_subdirectory(json)
endif (${ARCH} STREQUAL "X86")

if (${ARCH} STREQUAL "ESP8266")

endif (${ARCH} STREQUAL "ESP8266")


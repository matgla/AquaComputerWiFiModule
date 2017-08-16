file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/null.c)

add_library(arduinoVariant STATIC
    ${CMAKE_CURRENT_BINARY_DIR}/null.c
)

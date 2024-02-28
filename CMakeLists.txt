cmake_minimum_required(VERSION 3.18)
project(Sampler VERSION 1.0 DESCRIPTION "SFU: as2 for CMPT433")

set(CMAKE_C_STANDARD 11)
add_compile_options(
    -g
    -Wall
    -Werror
    -Wpedantic
    -Wextra
    -D_POSIX_C_SOURCE=200809L
)

# Define source files
set(SOURCES 
    app/blink.c
    app/lightReader.c
    app/main.c
    hal/led.c
    hal/sensor.c
)

# Define header files
set(HEADERS
    app/blink.h
    app/lightReader.h
    app/main.h
    hal/led.h
    hal/sensor.h
)

# Add executable
add_executable(as2 ${SOURCES} ${HEADERS})

# Copy executable to final location
add_custom_command(TARGET as2 POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:as2>"
     "~/cmpt433/public/myApps/as2" 
  COMMENT "Copying ARM executable to public NFS directory")

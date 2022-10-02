# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
## Uncomment and set custom path if needed
# set(PICO_SDK_PATH "$(set_path_here)/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(UARTTerminal C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(UARTTerminal 
UARTTerminal.c
        src/interface/ili9341.c
        src/gameshared.c
        src/gpu.c
        src/interface.c
        src/mainLoop.c
        src/terminal.c
        )

pico_set_program_name(UARTTerminal "UARTTerminal")
pico_set_program_version(UARTTerminal "0.1")

pico_enable_stdio_uart(UARTTerminal 0)
pico_enable_stdio_usb(UARTTerminal 1)

# Add the standard library to the build
target_link_libraries(UARTTerminal pico_stdlib)

# Add any user requested libraries
target_link_libraries(UARTTerminal
        hardware_spi
        )

pico_add_extra_outputs(UARTTerminal)


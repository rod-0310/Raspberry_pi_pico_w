cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(flashprint C CXX ASM)
set(CMAKE_C STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(uart_led
    pico_c.c
)
pico_enable_stdio_usb(uart_led 1)
pico_enable_stdio_uart(uart_led 0)
pico_add_extra_outputs(uart_led)
target_include_directories(uart_led PRIVATE ${CMAKE_CURRENT_LIST_DIR} )
target_link_libraries(uart_led pico_cyw43_arch_lwip_threadsafe_background pico_stdlib)
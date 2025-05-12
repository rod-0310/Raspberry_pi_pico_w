cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(pwm_pico_w C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pwm_pico_w
    pwm_pico_w.c
)

pico_enable_stdio_usb(pwm_pico_w 1)
pico_enable_stdio_uart(pwm_pico_w 0)

pico_add_extra_outputs(pwm_pico_w)

target_link_libraries(pwm_pico_w pico_stdlib hardware_pwm)

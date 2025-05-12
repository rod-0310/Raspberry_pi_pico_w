cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(adc_pwm_example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(adc_pwm_example
    adc_pwm_example.c
)

pico_enable_stdio_usb(adc_pwm_example 1)
pico_enable_stdio_uart(adc_pwm_example 0)

pico_add_extra_outputs(adc_pwm_example)

target_link_libraries(adc_pwm_example pico_stdlib hardware_pwm hardware_adc)

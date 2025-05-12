cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(adc_pwm_led C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(adc_pwm_led
    adc_pwm_led.c
)

pico_enable_stdio_usb(adc_pwm_led 1)
pico_enable_stdio_uart(adc_pwm_led 0)

pico_add_extra_outputs(adc_pwm_led)

target_link_libraries(adc_pwm_led
    pico_stdlib
    hardware_pwm
    hardware_adc
    hardware_gpio
    pico_cyw43_arch_none
)

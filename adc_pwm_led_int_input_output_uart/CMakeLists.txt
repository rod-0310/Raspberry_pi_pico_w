cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(adc_pwm_led_button C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Inicializa el SDK de Pico
pico_sdk_init()

# Crea el ejecutable a partir del archivo fuente
add_executable(adc_pwm_led_button
    adc_pwm_led_button.c
)

# Habilita la salida por USB
pico_enable_stdio_usb(adc_pwm_led_button 1)
# Desactiva la UART por defecto (puedes poner en 1 si la usas)
pico_enable_stdio_uart(adc_pwm_led_button 0)

# Agrega la salida bin/uf2
pico_add_extra_outputs(adc_pwm_led_button)

# Enlaza librerías necesarias
target_link_libraries(adc_pwm_led_button
    pico_stdlib
    hardware_pwm
    hardware_adc
    hardware_gpio
    pico_cyw43_arch_none
)

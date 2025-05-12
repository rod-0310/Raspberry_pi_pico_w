#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM_GPIO 16  // Pin donde se genera la señal PWM

int main() {
    stdio_init_all();  // Habilita salida por USB (puerto serie)
    printf("Iniciando PWM en GPIO %d...\n", PWM_GPIO);

    // Configura el GPIO para ser controlado por el PWM
    gpio_set_function(PWM_GPIO, GPIO_FUNC_PWM);

    // Obtiene el número de slice PWM asociado al pin
    uint slice_num = pwm_gpio_to_slice_num(PWM_GPIO);

    // Establece el período de PWM (resolución de 16 bits es suficiente para muchos casos)
    pwm_set_wrap(slice_num, 1000);  // Cuenta hasta 1000 -> periodo base
    pwm_set_enabled(slice_num, true);

    while (true) {
        for (int duty = 0; duty <= 1000; duty += 100) {
            pwm_set_chan_level(slice_num, pwm_gpio_to_channel(PWM_GPIO), duty);
            printf("PWM duty: %.1f%%\n", duty / 10.0);
            sleep_ms(500);
        }
    }
}

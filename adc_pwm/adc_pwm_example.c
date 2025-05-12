#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#define ADC_GPIO 26  // Pin donde está conectado el sensor analógico (potenciómetro)
#define PWM_GPIO 16  // Pin donde se genera la señal PWM (LED, motor, etc.)

int main() {
    stdio_init_all();  // Inicializa la salida por USB (puerto serie)
    printf("Iniciando ADC y PWM en Raspberry Pi Pico W...\n");

    // Configuración del ADC
    adc_init();  // Inicializa el ADC
    adc_gpio_init(ADC_GPIO);  // Configura el pin como entrada de ADC
    adc_select_input(0);  // Selecciona la entrada ADC0 (GPIO26)

    // Configuración del PWM
    gpio_set_function(PWM_GPIO, GPIO_FUNC_PWM);  // Configura el pin para PWM
    uint slice_num = pwm_gpio_to_slice_num(PWM_GPIO);  // Obtiene el slice asociado al pin
    pwm_set_wrap(slice_num, 1000);  // Establece el período del PWM (resolución de 16 bits)
    pwm_set_enabled(slice_num, true);  // Habilita el PWM

    while (true) {
        // Lee el valor del ADC (12 bits, entre 0 y 4095)
        uint16_t adc_value = adc_read();
        
        // Mapea el valor del ADC (0-4095) al rango de ciclo de trabajo del PWM (0-1000)
        uint16_t pwm_value = adc_value / 4;  // Escala de 12 bits a 10 bits

        // Ajusta el ciclo de trabajo del PWM en función del valor leído por el ADC
        pwm_set_chan_level(slice_num, pwm_gpio_to_channel(PWM_GPIO), pwm_value);
        
        // Imprime el valor del ADC y el ciclo de trabajo del PWM
        printf("ADC: %u, PWM Duty: %u\n", adc_value, pwm_value);
        
        sleep_ms(100);  // Espera un poco antes de la siguiente lectura
    }
}

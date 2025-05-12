#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"  // Para controlar el LED de la Pico W (GPIO 0 en el módulo CYW43)

//-------------------------------Funciones de configuración----------
static void config_adc(void);
static void config_pwm(void);
static int config_led_int(void);  // Debe devolver int por el return
static void inic_uart(void);

//---------------------------Funciones a usar------------------------
void on_off_led_int(void);
void lectura_adc(void);
void lectura_pwm(void);
void envio_datos_uart(void);

//---------------------------Variables para ADC-------------------------------------
#define ADC_GPIO 26  // GPIO26 es ADC0
uint16_t adc_value;

//---------------------------Variables PWM-----------------------------------------
#define PWM_GPIO 16      // GPIO para salida PWM
#define PWM_TOP 1000     // Ciclo PWM de 0-1000 (no frecuencia directamente)
uint16_t pwm_value;
uint slice_num;

int main() {
    inic_uart();

    if (config_led_int() != 0) {
        printf("Fallo en la inicialización del LED CYW43\n");
        return 1;
    }

    config_adc();
    config_pwm();

    while (true) {
        lectura_adc();
        lectura_pwm();
        envio_datos_uart();

        // Parpadeo del LED CYW43
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(100);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(100);
    }
}

void envio_datos_uart(void) {
    printf("ADC: %u, PWM Duty: %u\n", adc_value, pwm_value);
}

void lectura_pwm(void) {
    pwm_value = adc_value / 4;  // Escala 0–4095 a 0–1023 aprox
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(PWM_GPIO), pwm_value);
}

void lectura_adc(void) {
    adc_value = adc_read();
}

static void config_adc(void) {
    adc_init();
    adc_gpio_init(ADC_GPIO);
    adc_select_input(0);  // GPIO26 es canal 0
}

static void config_pwm(void) {
    gpio_set_function(PWM_GPIO, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(PWM_GPIO);

    pwm_set_wrap(slice_num, PWM_TOP);  // Define resolución (TOP)
    pwm_set_enabled(slice_num, true);
}

static void inic_uart(void) {
    stdio_init_all();
    printf("Iniciando ADC, PWM y LED en Raspberry Pi Pico W...\n");
}

static int config_led_int(void) {
    if (cyw43_arch_init()) {
        return -1;
    }
    return 0;
}

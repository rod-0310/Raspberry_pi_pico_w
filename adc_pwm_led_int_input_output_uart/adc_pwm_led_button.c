#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
//-------------------------------Configuraciones iniciales----------
static void config_adc(void);
static void config_pwm(void);
static int config_led_int(void);
static void config_led_button(void);
static void inic_uart(void);

//---------------------------Funciones a usar------------------------
void lectura_adc(void);
void lectura_pwm(void);
void envio_datos_uart(void);
void leer_boton_control_led(void);

//---------------------------Variables para ADC---------------------
#define ADC_GPIO 26
uint16_t adc_value;

//---------------------------Variables PWM--------------------------
#define PWM_GPIO 16
#define PWM_TOP 1000
uint16_t pwm_value;
uint slice_num;

//---------------------------Botón y LED adicional-------------------
#define BUTTON_GPIO 15
#define LED_EXTRA_GPIO 17

int main() {
    inic_uart();

    if (config_led_int() != 0) {
        printf("Fallo en la inicialización del LED CYW43\n");
        return 1;
    }

    config_adc();
    config_pwm();
    config_led_button();

    while (true) {
        lectura_adc();
        lectura_pwm();
        envio_datos_uart();
        leer_boton_control_led();

        // LED interno CYW43 parpadeando
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(100);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(100);
    }
}

//---------------------------Funciones principales---------------------

void envio_datos_uart(void) {
    printf("ADC: %u, PWM Duty: %u\n", adc_value, pwm_value);
}

void lectura_pwm(void) {
    pwm_value = adc_value / 4;
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(PWM_GPIO), pwm_value);
}

void lectura_adc(void) {
    adc_value = adc_read();
}

void leer_boton_control_led(void) {
    if (gpio_get(BUTTON_GPIO) == 0) {  // Botón presionado (activo bajo)
        gpio_put(LED_EXTRA_GPIO, 1);
    } else {
        gpio_put(LED_EXTRA_GPIO, 0);
    }
}

//---------------------------Configuración de periféricos---------------------

static void config_adc(void) {
    adc_init();
    adc_gpio_init(ADC_GPIO);
    adc_select_input(0);
}

static void config_pwm(void) {
    gpio_set_function(PWM_GPIO, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(PWM_GPIO);
    pwm_set_wrap(slice_num, PWM_TOP);
    pwm_set_enabled(slice_num, true);
}

static void config_led_button(void) {
    // Configura el botón
    gpio_init(BUTTON_GPIO);
    gpio_set_dir(BUTTON_GPIO, GPIO_IN);
    gpio_pull_up(BUTTON_GPIO);  // Pull-up interno

    // Configura el LED adicional
    gpio_init(LED_EXTRA_GPIO);
    gpio_set_dir(LED_EXTRA_GPIO, GPIO_OUT);
}

static void inic_uart(void) {
    stdio_init_all();
    printf("Iniciando ADC, PWM, LEDs y botón en Raspberry Pi Pico W...\n");
}

static int config_led_int(void) {
    return cyw43_arch_init();
}

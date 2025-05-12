#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"
#include "pico/time.h"

// -------------------- CONFIGURACIONES --------------------
#define ADC_GPIO 26         // ADC0
#define PWM_GPIO 16         // Salida PWM
#define LED_EXT_GPIO 17     // LED externo
#define LED2_EXT_GPIO 18    // LED externo
#define BUTTON_GPIO 15      // Botón
#define PWM_TOP 1000
#define TIMER_INTERVAL_MS 500  // Intervalo para interrupción de timer

// -------------------- VARIABLES GLOBALES --------------------
uint16_t adc_value, pwm_value;
uint slice_num;
volatile bool led2_on = false;
volatile bool led_ext_on = false;
volatile bool aux = false;  // Estado del LED interno

// -------------------- DECLARACIÓN DE FUNCIONES --------------------
static void config_adc(void);
static void config_pwm(void);
static int config_led_int(void);
static void config_button_interrupt(void);
static void inic_uart(void);
static void config_timer_interrupt(void);
static void config_GPIO(void);
void lectura_adc(void);
void lectura_pwm(void);
void envio_datos_uart(void);
bool repeating_timer_callback(struct repeating_timer *t);  // ISR timer
void button_isr(uint gpio, uint32_t events);               // ISR botón

// -------------------- FUNCIÓN PRINCIPAL --------------------
int main() {
    inic_uart();

    if (config_led_int() != 0) {
        printf("Error inicializando LED interno\n");
        return 1;
    }
    config_GPIO();
    config_adc();
    config_pwm();
    config_button_interrupt();
    config_timer_interrupt();

    while (true) {
        lectura_adc();
        lectura_pwm();
        envio_datos_uart();

        // Actualiza LED externo (encendido si se presiona el botón)
        gpio_put(LED_EXT_GPIO, led_ext_on);
        if(aux){
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        }
        else{
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        }
        sleep_ms(10);  // Pequeño delay para estabilidad
    }
}
// -------------------- INTERRUPCIONES --------------------
void button_isr(uint gpio, uint32_t events) {
    if (gpio == BUTTON_GPIO) {
        if (events & GPIO_IRQ_EDGE_FALL) {
            led_ext_on = true;
        } else if (events & GPIO_IRQ_EDGE_RISE) {
            led_ext_on = false;
        }
    }
}

bool repeating_timer_callback(struct repeating_timer *t) {
    led2_on = !led2_on;
    aux = !aux;
    gpio_put(LED2_EXT_GPIO, led2_on);
    return true;  // Repetir la interrupción
}
// -------------------- FUNCIONES DE LÓGICA --------------------
void lectura_adc(void) {
    adc_value = adc_read();
}

void lectura_pwm(void) {
    pwm_value = adc_value / 4;
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(PWM_GPIO), pwm_value);
}

void envio_datos_uart(void) {
    printf("ADC: %u, PWM: %u\n", adc_value, pwm_value);
}

// -------------------- FUNCIONES DE CONFIGURACIÓN --------------------
static void inic_uart(void) {
    stdio_init_all();
    printf("Iniciando sistema...\n");
}

static int config_led_int(void) {
    if (cyw43_arch_init()) return -1;
    return 0;
}

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

static void config_GPIO(void) {
    gpio_init(LED_EXT_GPIO);
    gpio_init(LED2_EXT_GPIO);
    gpio_set_dir(LED_EXT_GPIO, GPIO_OUT);
    gpio_set_dir(LED2_EXT_GPIO, GPIO_OUT);
    gpio_put(LED_EXT_GPIO, 0);
    gpio_put(LED2_EXT_GPIO, 0);
}

static void config_button_interrupt(void) {
    gpio_init(BUTTON_GPIO);
    gpio_set_dir(BUTTON_GPIO, GPIO_IN);
    gpio_pull_up(BUTTON_GPIO);

    // Habilita interrupciones por flanco de bajada y subida
    gpio_set_irq_enabled_with_callback(BUTTON_GPIO, 
        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, 
        true, 
        &button_isr);
}

static void config_timer_interrupt(void) {
    static struct repeating_timer timer;
    add_repeating_timer_ms(TIMER_INTERVAL_MS, repeating_timer_callback, NULL, &timer);
}
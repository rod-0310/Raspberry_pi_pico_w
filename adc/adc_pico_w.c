#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

int main() {
    stdio_init_all();  // Inicializa salida por USB (UART virtual)
    sleep_ms(2000);    // Espera para asegurar conexión de consola serial
    printf("Iniciando lectura de ADC en GPIO26 (canal 0)...\n");

    adc_init();                 // Inicializa el ADC
    adc_gpio_init(26);         // Prepara GPIO26 para entrada analógica
    adc_select_input(0);       // Selecciona canal 0 (GPIO26)

    const float conversion_factor = 3.3f / (1 << 12);  // 3.3V / 4096

    while (true) {
        uint16_t resultado = adc_read();  // Lectura cruda de 12 bits
        float voltaje = resultado * conversion_factor;
        printf("Valor ADC: %u -> Voltaje: %.2f V\n", resultado, voltaje);
        sleep_ms(500);
    }
}

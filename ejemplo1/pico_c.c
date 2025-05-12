#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {
    stdio_init_all();

    // Inicializa el chip WiFi y el LED integrado
    if (cyw43_arch_init()) {
        printf("Error inicializando cyw43\n");
        return -1;
    }

    while (true) {
        printf("LED on\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(2500);

        printf("LED off\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(2500);
    }

    cyw43_arch_deinit();  // Aunque no se alcanza, es buena práctica
    return 0;
}

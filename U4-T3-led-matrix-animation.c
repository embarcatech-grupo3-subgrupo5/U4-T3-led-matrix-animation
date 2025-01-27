#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

#include "animations.h"

// Inclusão do arquivo .pio
#include "pio_matrix.pio.h"

// Definição do número de LEDs
#define NUM_LEDS 25

// Definição do pino de saída
#define OUT_PIN 15

// Definição dos pinos do teclado matricial - Geison
#define ROW1 26
#define ROW2 22
#define ROW3 23
#define ROW4 20
#define COL1 19
#define COL2 18
#define COL3 17
#define COL4 16

#define DEBOUNCE_TIME 60 // Tempo de debounce em ms, para evitar leituras incorretas ao pressionar rapidamente as teclas - Geison

// Função para inicializar o teclado matricial - Geison
void init_keypad() {
    gpio_init(ROW1); gpio_set_dir(ROW1, GPIO_OUT); gpio_put(ROW1, true);
    gpio_init(ROW2); gpio_set_dir(ROW2, GPIO_OUT); gpio_put(ROW2, true);
    gpio_init(ROW3); gpio_set_dir(ROW3, GPIO_OUT); gpio_put(ROW3, true);
    gpio_init(ROW4); gpio_set_dir(ROW4, GPIO_OUT); gpio_put(ROW4, true);

    gpio_init(COL1); gpio_set_dir(COL1, GPIO_IN); gpio_pull_up(COL1);
    gpio_init(COL2); gpio_set_dir(COL2, GPIO_IN); gpio_pull_up(COL2);
    gpio_init(COL3); gpio_set_dir(COL3, GPIO_IN); gpio_pull_up(COL3);
    gpio_init(COL4); gpio_set_dir(COL4, GPIO_IN); gpio_pull_up(COL4);
}

// Função para ler a tecla pressionada no teclado matricial - Geison
char read_keypad() {
    const char keys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int row = 0; row < 4; row++) {
        gpio_put(ROW1, true);
        gpio_put(ROW2, true);
        gpio_put(ROW3, true);
        gpio_put(ROW4, true);

        if (row == 0) gpio_put(ROW1, false);
        if (row == 1) gpio_put(ROW2, false);
        if (row == 2) gpio_put(ROW3, false);
        if (row == 3) gpio_put(ROW4, false);

        if (!gpio_get(COL1)) return keys[row][0];
        if (!gpio_get(COL2)) return keys[row][1];
        if (!gpio_get(COL3)) return keys[row][2];
        if (!gpio_get(COL4)) return keys[row][3];
    }

    return ' ';
}

// Função para converter valores RGB em um único valor de 32 bits
uint32_t matrix_rgb(double b, double r, double g) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Função para apagar a matriz de LEDs
void apagar_matriz(uint32_t valor_led, PIO pio, uint sm) {
    for (int16_t i = 0; i < NUM_LEDS; i++) {
        valor_led = matrix_rgb(0.0, 0.0, 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função principal
int main() {
    PIO pio = pio0;
    bool ok;
    uint32_t valor_led;
    double r = 0.0, g = 0.0, b = 0.0;

    // Configuração do clock
    ok = set_sys_clock_khz(128000, false);
    stdio_init_all();

    printf("Iniciando a transmissão PIO\n");
    if (ok) printf("Clock set to %ld\n", clock_get_hz(clk_sys));

    // Configuração da PIO
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Inicializa o teclado matricial
    init_keypad();

    while (1) {
        char key = read_keypad();
        if (key != ' ') {
            printf("Tecla pressionada: %c\n", key);
            apagar_matriz(valor_led, pio, sm);

            switch (key) {
                case '1':
                    play_skull_animation(valor_led, pio, sm);
                    break;
                case '4':
                    play_animation4(valor_led, pio, sm);
                    break;
                case 'A':
                    apagar_matriz(valor_led, pio, sm);
                    break;
                case 'B':
                    ligar_leds_azuis(valor_led, pio, sm);
                    break;
                case 'D':
                    ligar_leds_verdes(valor_led, pio, sm);
                    break;
                default:
                    printf("Tecla não mapeada\n");
            }
        }

        sleep_ms(DEBOUNCE_TIME);
    }
}

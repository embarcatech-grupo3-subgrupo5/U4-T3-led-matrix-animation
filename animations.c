#define _POSIX_C_SOURCE 199309L
#include "animations.h"
#include <unistd.h>  // Para usar o sleep
#include <time.h>    // Para usar nanosleep
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// Frames da Animação 1 - Geison
double animation1_frame1[25] = {
    0, 1, 0, 1, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0
};

double animation1_frame2[25] = {
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0
};

double animation1_frame3[25] = {
    0, 1, 1, 1, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 1, 1, 1, 0,
    0, 0, 1, 0, 0
};


// Funções para reproduzir as animações
void play_animation1(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int i = 0; i < 5; i++) {
        desenho_pio(animation1_frame1, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_pio(animation1_frame2, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
        desenho_pio(animation1_frame3, valor_led, pio, sm, r, g, b);
        sleep_ms(200);
    }
}

//rotina para acionar a matrix de leds - ws2812b
// Função para desenhar na matriz de LEDs
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    for (int16_t i = 0; i < 25; i++) {
        valor_led = matrix_rgb(
            desenho[i] * b, 
            desenho[i] * r, 
            desenho[i] * g
        );
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}
void desenho_pio_blue(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void desenho_pio_red(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <stdint.h>
#include "hardware/pio.h"

// Frames para a animação da caveira
extern const double skull_frames[10][25];

// Função para desenhar no PIO
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

// Função para converter valores RGB para formato utilizado na matriz
uint32_t matrix_rgb(double b, double r, double g);

// Funções para reproduzir as animações
void play_skull_animation(uint32_t valor_led, PIO pio, uint sm);

void play_animation4(uint32_t valor_led, PIO pio, uint sm);

void apagar_matriz(uint32_t valor_led, PIO pio, uint sm);

void ligar_leds_azuis(uint32_t valor_led, PIO pio, uint sm);

#endif // ANIMATIONS_H
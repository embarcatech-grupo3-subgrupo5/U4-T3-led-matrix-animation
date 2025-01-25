#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <stdint.h>
#include "hardware/pio.h"

// Frames para a animação 1 - Geison
extern double animation1_frame1[25];
extern double animation1_frame2[25];
extern double animation1_frame3[25];

// Função para desenhar no PIO
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

// Função para converter valores RGB para formato utilizado na matriz
uint32_t matrix_rgb(double b, double r, double g);

// Funções para reproduzir as animações
void play_animation1(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

#endif // ANIMATIONS_H

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
const double skull_frames[10][25] = {
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0
    },
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 1.0, 0.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0
    },
    {
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0
    },
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0
    },
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0
    },
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0
    },
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0
    },
    {
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0
    },
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0
    }
};

const double animation4_frames[5][25] = {
    {0.0, 0.9, 0.0, 0.9, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.9, 0.9, 0.9, 0.0,
     0.9, 0.0, 0.0, 0.0, 0.9,
    },//Rosto triste
    {0.0, 0.0, 1.0, 0.0, 0.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 1.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 0.0,
    },//Seta 
    {0.0, 1.0, 0.5, 1.0, 0.0,
     1.0, 0.0, 1.0, 0.0, 1.0,
     0.0, 1.0, 0.5, 1.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,       
     0.0, 0.0, 0.0, 0.0, 0.0
    },//Borboleta
    {0.0, 0.9, 0.0, 0.9, 0.0,
     0.9, 0.0, 0.9, 0.0, 0.9,
     0.9, 0.0, 0.0, 0.0, 0.9,
     0.0, 0.9, 0.0, 0.9, 0.0,
     0.0, 0.0, 0.9, 0.0, 0.0,     
    },//Coração
    {0.0, 1.0, 0.0, 1.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0     
    }//Rosto Feliz
};
// Funções para reproduzir as animações
void play_skull_animation(uint32_t valor_led, PIO pio, uint sm) {
    for (int f = 0; f < 10; f++) {
        for (int i = 0; i < 25; i++) {
            if (skull_frames[f][i] == 1.0) {
                valor_led = matrix_rgb(0.0, 1.0, 0.0); // Vermelho
            } else {
                valor_led = matrix_rgb(0.0, 0.0, 0.0); // Preto para LEDs apagados
            }
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(1000); // Pausa de 1 segundo entre os frames
    }
}

void play_animation4(uint32_t valor_led, PIO pio, uint sm) {
    sleep_ms(500);
    for (int f = 0; f < 5; f++) {
        for (int i = 0; i < 25; i++){
            if (animation4_frames[f][24-i] == 1.0) {
                valor_led = matrix_rgb(0.0, 0.0, 1.0); //Verde
            }else if (animation4_frames[f][24-i] == 0.5) {
                valor_led = matrix_rgb(1.0, 0.0, 0.0); //Azul
            }else if (animation4_frames[f][24-i] == 0.9){
                valor_led = matrix_rgb(0.0, 1.0, 0.0); //Vermelho
            }else if (animation4_frames[f][24-i] == 0.0){
                valor_led = matrix_rgb(0.0, 0.0, 0.0);
            }
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(1000);
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
//Acende todos os leds na cor azul, com intesidade de 100%
void ligar_leds_azuis(uint32_t valor_led, PIO pio, uint sm){
    sleep_ms(500);
    for (int i = 0; i < 25; i++){
        valor_led = matrix_rgb(1.0, 0.0, 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}
void desenho_pio_blue(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void desenho_pio_red(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

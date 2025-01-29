#define _POSIX_C_SOURCE 199309L
#include "animations.h"
#include <unistd.h> // Para usar o sleep
#include <time.h>   // Para usar nanosleep
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// Frames da Animação 1 - Geison
const double animation1_frames[40][25] = {
    {   
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0}, //1
    {   
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0}, //vazio
    {   
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0}, //1
    {   
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0}, //Z
    {   
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0}, //Z
    {   
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0},                 
    {   
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0}, // E
    {   
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0}, // E 
    {   
        1.0, 0.0, 0.0, 0.0, 0.8,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.8,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0}, // L
    {   
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0}, // L
    {
        1.0, 1.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 0.0, 0.0}, // D
    {
        1.0, 1.0, 1.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 0.0, 0.0}, // D   
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0}, //A
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0}, //A
    {   
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0},
    {   
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0},
    {   
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0},
    {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0},
    {   
        0.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0},
    {   
        0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0},
    {   
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0},
    {   
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0}, // Triforce symbol
    {   
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0}, // Master Sword
    {
        0.0, 0.0, 1.0, 0.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0}, // Rupee icon
    {
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0}, // Heart container
    {
        1.0, 1.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 0.0, 0.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 0.0, 0.0, 1.0}, // Navi
    {
        0.0, 1.0, 1.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        0.0, 1.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0}, // Shield
    {
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0}, // Bow
    {
        0.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0}, // Fairy
    {
        1.0, 0.0, 1.0, 0.0, 1.0,
        0.0, 1.0, 0.0, 1.0, 0.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        0.0, 1.0, 1.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 0.0, 0.0}, // Zelda’s silhouette
    {
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0} // "Zelda" text pattern
};

const double animation2_frames[5][25] = {
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

const double animation4_frames[3][25] = {
    
    {0.2, 0.1, 0.0, 0.1, 0.2,
    0.3, 0.2, 0.1, 0.2, 0.3,
    0.5, 0.4, 0.3, 0.4, 0.5,
    0.7, 0.5, 0.4, 0.5, 0.7,
    1.0, 0.8, 0.6, 0.8, 1.0
    },
    {0.1, 0.0, 0.0, 0.0, 0.1,
    0.2, 0.1, 0.0, 0.1, 0.2,
    0.4, 0.3, 0.2, 0.3, 0.4,
    0.6, 0.4, 0.3, 0.4, 0.6,
    0.9, 0.7, 0.5, 0.7, 0.9
    },

    {0.0, 0.0, 0.0, 0.0, 0.0,
    0.1, 0.0, 0.0, 0.0, 0.1,
    0.3, 0.2, 0.1, 0.2, 0.3,
    0.5, 0.3, 0.2, 0.3, 0.5,
    0.8, 0.6, 0.4, 0.6, 0.8
    }
};

const float animation5_frames[7][25] = {
    // Definindo os frames do relâmpago em padrões de acendimento e apagamento
    {1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0},
    {0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0},
    {1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0},
    {1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0},
    {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
    {1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0}
};

// Funções para reproduzir as animações
void play_animation1(uint32_t valor_led, PIO pio, uint sm) {
    // Frequências das notas da música Zelda's Lullaby - introdução
    uint32_t frequencias[] = {
        330, 392, 294, 262, 294, 330, 392, 294, 262, 294,
        440, 392, 349, 330, 294, 262, 330, 294, 262, 294,
        392, 349, 330, 294, 262, 294, 330, 349, 392, 440};

    // Duração das notas correspondentes (em ms)
    uint32_t duracoes[] = {
        300, 600, 300, 150, 150, 300, 600, 300, 150, 150,
        300, 300, 300, 300, 150, 150, 300, 150, 150, 300,
        300, 300, 300, 300, 150, 150, 300, 150, 150, 300};

    // Cores do arco-íris (em RGB)
    uint32_t rainbow_colors[7][3] = {
        {1.0, 0.0, 0.0}, // Vermelho
        {1.0, 0.5, 0.0}, // Laranja
        {1.0, 1.0, 0.0}, // Amarelo
        {0.0, 1.0, 0.0}, // Verde
        {0.0, 0.0, 1.0}, // Azul
        {0.5, 0.0, 1.0}, // Índigo
        {0.8, 0.0, 1.0}  // Violeta
    };

    for (int f = 0; f < 40; f++) { // 40 frames totais
        // Atualizar LEDs com cores do arco-íris
        for (int row = 0; row < 5; row++) { // Inverter as linhas
            int inverted_row = 4 - row; // Espelha verticalmente (exemplo: 0 vira 4, 1 vira 3)
            for (int col = 0; col < 5; col++) {
                int index = inverted_row * 5 + col; // Calcula o índice no array 1D
                if (animation1_frames[f][index] == 1.0) {
                    // Selecionar uma cor do arco-íris com base no índice
                    int color_index = (f + row + col) % 7;
                    valor_led = matrix_rgb(
                        rainbow_colors[color_index][0],
                        rainbow_colors[color_index][1],
                        rainbow_colors[color_index][2]
                    );
                } else {
                    valor_led = matrix_rgb(0.0, 0.0, 0.0); // Preto
                }
                pio_sm_put_blocking(pio, sm, valor_led);
            }
        }

        // Tocar a nota correspondente ao frame atual
        if (f < 30 && frequencias[f] != 0) {
            acionar_buzzer_com_frequencia(frequencias[f], duracoes[f]);
        }

        // Pausa entre os frames
        sleep_ms(100);
    }
}

void play_animation2(uint32_t valor_led, PIO pio, uint sm)
{
    sleep_ms(500);
    for (int f = 0; f < 5; f++)
    {
        for (int i = 0; i < 25; i++)
        {
            if (animation2_frames[f][24 - i] == 1.0)
            {
                valor_led = matrix_rgb(0.0, 0.0, 1.0); // Verde
            }
            else if (animation2_frames[f][24 - i] == 0.5)
            {
                valor_led = matrix_rgb(1.0, 0.0, 0.0); // Azul
            }
            else if (animation2_frames[f][24 - i] == 0.9)
            {
                valor_led = matrix_rgb(0.0, 1.0, 0.0); // Vermelho
            }
            else if (animation2_frames[f][24 - i] == 0.0)
            {
                valor_led = matrix_rgb(0.0, 0.0, 0.0);
            }
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(1000);
    }
}

void play_animation3(uint32_t valor_led, PIO pio, uint sm)
{
    // Frequências de uma melodia curta
    uint32_t frequencias[] = {440, 494, 523, 440, 494, 523, 659, 698, 440, 523};
    uint32_t duracoes[] = {300, 300, 300, 300, 300, 300, 300, 300, 300, 300};

    // Número de frames e LEDs na matriz
    int total_frames = 10;
    int num_leds = 25;

    // Padrão de cores (R, G, B)
    float cores[][3] = {
        {1.0, 0.0, 0.0}, // Vermelho
        {0.0, 1.0, 0.0}, // Verde
        {0.0, 0.0, 1.0}, // Azul
        {1.0, 1.0, 0.0}, // Amarelo
        {1.0, 0.0, 1.0}  // Magenta
    };

    for (int f = 0; f < total_frames; f++)
    {
        for (int i = 0; i < num_leds; i++)
        {
            // Determinar a posição do LED na espiral
            int pos = (f + i) % num_leds;

            // Alternar entre cores com base no frame atual
            int cor_index = (f + i) % 5;
            valor_led = matrix_rgb(cores[cor_index][0], cores[cor_index][1], cores[cor_index][2]);

            // Atualizar o LED
            pio_sm_put_blocking(pio, sm, valor_led);
        }

        // Tocar a nota correspondente ao frame atual
        if (f < 10 && frequencias[f] != 0)
        {
            acionar_buzzer_com_frequencia(frequencias[f], duracoes[f]);
        }

        // Pausa entre os frames
        sleep_ms(500);
    }
}

void play_animation4(uint32_t valor_led, PIO pio, uint sm)
{
    //taxa de quadros por segundo
    #define FPS 10

    //tempo entre quadros (em ms)
    #define FRAME_DELAY (1000 / FPS)

    //vetor de quadros da animação
    double *quadros[] = {animation4_frames};
    size_t total_quadros = sizeof(quadros) / sizeof(quadros[0]);

    size_t quadro_atual = 0;

    for (int iteracao = 0; iteracao < 10; iteracao++) { // Executa 10 ciclos completos da animação
        // Exibe o quadro atual
        desenho_pio(quadros[quadro_atual], valor_led, pio, sm, 1.0, 0.4, 0.0); // Tons de laranja/vermelho

        // Atualiza para o próximo quadro
        quadro_atual = (quadro_atual + 1) % total_quadros;

        // Aguarda antes de mostrar o próximo quadro
        sleep_ms(FRAME_DELAY);
    }
}

void play_animation5(uint32_t valor_led, PIO pio, uint sm){
    // Cores do relâmpago e apagado
    float yellow[3] = {0.0, 1.0, 1.0}; // Amarelo
    float off[3] = {0.0, 0.0, 0.0}; // Preto (apagado)

    // Intervalo entre os flashes
    uint32_t FLASH_INTERVAL = 300;

    for (int f = 0; f < 7; f++) {
        for (int i = 0; i < 25; i++) {
            if (animation5_frames[f][i] == 1.0) {
                valor_led = matrix_rgb(0.0, 1.0, 1.0); // Garante o amarelo correto
            } else {
                valor_led = matrix_rgb(off[0], off[1], off[2]); // Apagar o LED (Preto)
            }
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        sleep_ms(FLASH_INTERVAL); // Intervalo entre os flashes
    }
}

//  Função para desenhar na matriz de LEDs
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(
            desenho[i] * b,
            desenho[i] * r,
            desenho[i] * g);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}
// Acende todos os leds na cor azul, com intesidade de 100%
void ligar_leds_azuis(uint32_t valor_led, PIO pio, uint sm)
{
    sleep_ms(500);
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(1.0, 0.0, 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}
// Acende todos os leds na cor Vermelho, com intesidade de 80%
void ligar_leds_vermelhos(uint32_t valor_led, PIO pio, uint sm)
{
    sleep_ms(800);
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(0.0, 0.8, 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}
// Acende todos os leds na cor verde, com intesidade de 50%
void ligar_leds_verdes(uint32_t valor_led, PIO pio, uint sm)
{
    sleep_ms(500);
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(0.0, 0.0, 0.5);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}
// Acende todos os leds na cor Branco, com intesidade de 20%
void ligar_leds_brancos(uint32_t valor_led, PIO pio, uint sm)
{
    sleep_ms(800);
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(0.2, 0.2, 0.2);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

void desenho_pio_blue(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);

void desenho_pio_red(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
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

//--------------------------------------------------------------------------------------------------------------------------------
bool modo_uart = true; // Inicia no modo UART por padrão, para utilizar o modo do teclado matricial no wokwi, altere para false  |
//--------------------------------------------------------------------------------------------------------------------------------

// Definição do número de LEDs
#define NUM_LEDS 25

// Definição do pino de saída dos LEDs
#define OUT_PIN 7

// Definição dos pinos do teclado matricial
#define ROW1 27
#define ROW2 26
#define ROW3 22
#define ROW4 20
#define COL1 19
#define COL2 18
#define COL3 17
#define COL4 16

#define DEBOUNCE_TIME 60 // Tempo de debounce em ms, para evitar leituras incorretas ao pressionar rapidamente as teclas - Geison

#define BUZZER 21 // Pino do buzzer

// Função para inicializar o buzzer
void init_buzzer()
{
    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);
}

// Função para acionar o buzzer com uma frequência específica
void acionar_buzzer_com_frequencia(uint32_t frequencia, uint32_t duracao_ms)
{
    uint32_t periodo = 1000000 / frequencia;
    uint32_t meio_periodo = periodo / 2;
    uint32_t tempo_final = time_us_32() + (duracao_ms * 1000);

    while (time_us_32() < tempo_final)
    {
        gpio_put(BUZZER, 1);
        sleep_us(meio_periodo);
        gpio_put(BUZZER, 0);
        sleep_us(meio_periodo);
    }
}

uint32_t matrix_rgb(double b, double r, double g)
{
    unsigned char R = (unsigned char)(r * 255);
    unsigned char G = (unsigned char)(g * 255);
    unsigned char B = (unsigned char)(b * 255);
    return (G << 24) | (R << 16) | (B << 8);
}

// Função para inicializar o teclado matricial
void init_keypad()
{
    gpio_init(ROW1);
    gpio_set_dir(ROW1, GPIO_OUT);
    gpio_put(ROW1, true);
    gpio_init(ROW2);
    gpio_set_dir(ROW2, GPIO_OUT);
    gpio_put(ROW2, true);
    gpio_init(ROW3);
    gpio_set_dir(ROW3, GPIO_OUT);
    gpio_put(ROW3, true);
    gpio_init(ROW4);
    gpio_set_dir(ROW4, GPIO_OUT);
    gpio_put(ROW4, true);

    gpio_init(COL1);
    gpio_set_dir(COL1, GPIO_IN);
    gpio_pull_up(COL1);
    gpio_init(COL2);
    gpio_set_dir(COL2, GPIO_IN);
    gpio_pull_up(COL2);
    gpio_init(COL3);
    gpio_set_dir(COL3, GPIO_IN);
    gpio_pull_up(COL3);
    gpio_init(COL4);
    gpio_set_dir(COL4, GPIO_IN);
    gpio_pull_up(COL4);
}

// Função para ler a tecla pressionada no teclado matricial
char read_keypad()
{
    const char keys[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};

    for (int row = 0; row < 4; row++)
    {
        gpio_put(ROW1, true);
        gpio_put(ROW2, true);
        gpio_put(ROW3, true);
        gpio_put(ROW4, true);

        if (row == 0)
            gpio_put(ROW1, false);
        if (row == 1)
            gpio_put(ROW2, false);
        if (row == 2)
            gpio_put(ROW3, false);
        if (row == 3)
            gpio_put(ROW4, false);

        if (!gpio_get(COL1))
            return keys[row][0];
        if (!gpio_get(COL2))
            return keys[row][1];
        if (!gpio_get(COL3))
            return keys[row][2];
        if (!gpio_get(COL4))
            return keys[row][3];
    }

    return ' ';
}

// Função para apagar a matriz de LEDs
void apagar_matriz(uint32_t valor_led, PIO pio, uint sm)
{
    for (int16_t i = 0; i < NUM_LEDS; i++)
    {
        valor_led = matrix_rgb(0.0, 0.0, 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Função para processar comandos
void processar_comandos(char comando, uint32_t valor_led, PIO pio, uint sm)
{
    switch (comando)
    {
    case '1': // Animação 1
        play_animation1(valor_led, pio, sm);
        break;
    case '4': // Animação 4
        play_animation4(valor_led, pio, sm);
        break;
    case '5': // Animação 5
        play_animation5(valor_led, pio, sm);
    case '7': // Animação 5
        play_animation7(valor_led, pio, sm);
        break;
    case 'A': // Apagar matriz
        apagar_matriz(valor_led, pio, sm);
        break;
    case 'B': // Ligar LEDs azuis
        ligar_leds_azuis(valor_led, pio, sm);
        break;
    case 'C': // Ligar LEDs Vermelhos
        ligar_leds_vermelhos(valor_led, pio, sm);
        break;
    case 'D': // Ligar LEDs verdes
        ligar_leds_verdes(valor_led, pio, sm);
        break;
    case '#': // Ligar LEDs verdes
        ligar_leds_brancos(valor_led, pio, sm);
        break;
    case '6': // Buzzer por 2 segundos
        acionar_buzzer_com_frequencia(1000, 2000);
        break;
    case '*': // Reset USB
        reset_usb_boot(0, 0);
        printf("Modo de gravação habilitado.\n");
        break;
    default:
        printf("Comando desconhecido: %c\n", comando);
        break;
    }
}

// Função principal
int main()
{
    PIO pio = pio0;
    uint32_t valor_led;
    stdio_init_all();
    init_keypad();
    init_buzzer();

    // Configuração do PIO para LEDs
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    printf("Sistema iniciado. Insira um comando:\n");
    printf("1: Animação 1\n");
    printf("4: Animação 4\n");
    printf("7: Animação 7\n");
    printf("A: Apagar LEDs\n");
    printf("B: Ligar LEDs azuis\n"); 
    printf("C: Ligar LEDs reds\n");
    printf("D: Ligar LEDs verdes\n");
    printf("#: Ligar LEDs brancos\n");
    printf("6: Buzzer por 2 segundos\n");
    printf("*: Reset USB\n");

    while (1)
    {
        char comando;

        if (modo_uart)
        {
            printf("Modo UART: Digite um comando: ");
            comando = getchar(); // Recebe comando via UART
        }
        else
        {
            comando = read_keypad(); // Recebe comando do teclado matricial
        }

        if (comando != ' ')
        {
            printf("Comando recebido: %c\n", comando);
            processar_comandos(comando, valor_led, pio, sm); // Processa o comando
        }

        sleep_ms(DEBOUNCE_TIME); // Para evitar leituras rápidas no teclado matricial
    }
}

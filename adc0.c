#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/uart.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"

// I2C defines
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
// UART defines
#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
// Definições do display
#define LARGURA 128
#define ALTURA 64
#define PORTA_I2C i2c1
#define PINO_SDA 14
#define PINO_SCL 15
#define ENDERECO_OLED 0x3C
ssd1306_t display;
// Pinos
#define PIN_LEDR 13
#define PIN_LEDG 11
#define PIN_LEDB 12
#define PIN_BA 5
#define PIN_BB 22
#define PIN_X 27
#define PIN_Y 26
//variaveis globais
uint slice0; // Variável de propriedades PWM
uint slice1; // Variável de propriedades PWM
uint8_t pos_x = 59; //posical inicial central
uint8_t pos_y = 27;
uint16_t vr_x; // valor analogico lido
uint16_t vr_y;
uint16_t ajust = 12;// ajusta a velocidade
bool eslice = true; // variavel de on/off do pwm
bool eborda = false; //  varieavel de on/off da borda e led verde

uint8_t incr(uint16_t vr, uint8_t pos, int p) {
    int o;
    if(p==1){ o =-1;}
    if(p==2){ o = 1;}
    // Calcula o incremento baseado no valor de 'vr'
    int8_t incr = o*((vr - 2048) * 5 / 4096);  // Pode ser negativo, por isso usamos int8_t
    // Ajusta a posição
    pos = (pos + incr) % (64*p);
    // Corrige valores negativos (comportamento correto para módulo)
    if (pos < 0) {
        pos += 64*p;
    }
    // Garante que valores acima de 127 sejam ajustados para 0 (embora isso já seja coberto pelo % 128)
    if (pos > (64*p -1)) {
        pos -=64*p -1;
    }
    // Retorna a nova posição
    return (int8_t)pos;
}


void config_i2c(){
     // I2C Initialisation. Using it at 400Khz.
     i2c_init(I2C_PORT, 400*1000);
     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
     gpio_pull_up(I2C_SDA);
     gpio_pull_up(I2C_SCL);   
}
void config_display() {
    ssd1306_init(&display, LARGURA, ALTURA, false, ENDERECO_OLED, PORTA_I2C);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
}

void config_uart(){
    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_puts(UART_ID, " Hello, UART!\n");
}
void config_adc(){
    adc_init();
    adc_gpio_init(PIN_X);
    adc_gpio_init(PIN_Y);
}

void config_pwm(){
    // Configuração do PWM
    gpio_set_function(PIN_LEDB, GPIO_FUNC_PWM);
    gpio_set_function(PIN_LEDR, GPIO_FUNC_PWM);
    slice0 = pwm_gpio_to_slice_num(PIN_LEDB);
    slice1 = pwm_gpio_to_slice_num(PIN_LEDR);
    pwm_set_clkdiv(slice0, 62.8f);
    pwm_set_clkdiv(slice1, 62.8f); // (125MHz / (50Hz * 2000) = 125)
    pwm_set_wrap(slice0, 2000);
    pwm_set_wrap(slice1, 2000);    // 1 wrap = 1us 
    pwm_set_enabled(slice0, eslice);  // Habilita o PWM
    pwm_set_enabled(slice1, eslice);  // Habilita o PWM
}
void gpio_irq_handler(uint gpio, uint32_t eventos) {
    static absolute_time_t ultimo_tempo_a = 0;
    static absolute_time_t ultimo_tempo_b = 0;

    if (gpio == PIN_BA) {
        static absolute_time_t ultimo_tempo_interrupcao = 0;
        absolute_time_t agora = get_absolute_time();

        // Debounce
        if (absolute_time_diff_us(ultimo_tempo_interrupcao, agora) > 200000) {
            eslice= !eslice;
            pwm_set_enabled(slice0, eslice);  // Habilita o PWM
            pwm_set_enabled(slice1, eslice);  // Habilita o PWM
        }
        ultimo_tempo_interrupcao = agora;
    } else if (gpio == PIN_BB) {
        static absolute_time_t ultimo_tempo_interrupcao = 0;
        absolute_time_t agora = get_absolute_time();

        // Debounce
        if (absolute_time_diff_us(ultimo_tempo_interrupcao, agora) > 200000) {
            eborda=!eborda;
            gpio_put(PIN_LEDG,eborda);

        }
        ultimo_tempo_interrupcao = agora;
    }
}
// Inicialização dos botões com interrupções
void config_botoes() {
    gpio_init(PIN_LEDG);
    gpio_set_dir(PIN_LEDG, GPIO_OUT);
    gpio_init(PIN_BA);
    gpio_set_dir(PIN_BA, GPIO_IN);
    gpio_pull_up(PIN_BA);
    gpio_init(PIN_BB);
    gpio_set_dir(PIN_BB, GPIO_IN);
    gpio_pull_up(PIN_BB);
    gpio_set_irq_enabled_with_callback(PIN_BA, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(PIN_BB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}
int main(){
    stdio_init_all();
    config_i2c();
    config_display();
    config_uart();
    config_adc();
    config_pwm();
    config_botoes();

    while (true) {
        adc_select_input(PIN_X -26);
        vr_x = adc_read();
        pwm_set_chan_level(slice0, PWM_CHAN_A, abs(vr_x-2048));
        printf("Posição x: %d\n", pos_x);

        adc_select_input(PIN_Y-26);
        vr_y = adc_read();
        pwm_set_chan_level(slice1, PWM_CHAN_B, abs(vr_y-2048));
        printf("Posição y: %d\n", pos_y);

        pos_x = incr(vr_x,pos_x,2);
        pos_y = incr(vr_y,pos_y,1);
        ssd1306_fill(&display,false);
        ssd1306_rect(&display, 0, 0, 128, 64, eborda, !eborda); // Desenha um retângulo
        ssd1306_rect(&display,pos_y,pos_x, 8, 8, true, false); 
        ssd1306_send_data(&display);

        sleep_ms(100);
    }
}

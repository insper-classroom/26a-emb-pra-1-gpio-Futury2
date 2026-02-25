#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

const int BTN_PIN = 6;
const int LED_PIN = 5;

const int BOBINA_PIN_1 = 21;
const int BOBINA_PIN_2 = 20;
const int BOBINA_PIN_3 = 19;
const int BOBINA_PIN_4 = 18;

const int PASSOS_VOLTA = 4096;

const int matriz[8][4] = {
    {1, 0, 0, 0},  
    {1, 1, 0, 0}, 
    {0, 1, 0, 0},  
    {0, 1, 1, 0},  
    {0, 0, 1, 0},  
    {0, 0, 1, 1},  
    {0, 0, 0, 1},  
    {1, 0, 0, 1}   
};

void motor_passo(int passo) {
    gpio_put(BOBINA_PIN_1, matriz[passo][0]);
    gpio_put(BOBINA_PIN_2, matriz[passo][1]);
    gpio_put(BOBINA_PIN_3, matriz[passo][2]);
    gpio_put(BOBINA_PIN_4, matriz[passo][3]);
}

void girar_sentido_horario() {
    int passo_atual = 0;
    
    for (int i = 0; i < PASSOS_VOLTA; i++) {
        motor_passo(passo_atual);
        sleep_ms(2);  
        
        passo_atual++;
        if (passo_atual >= 8) {
            passo_atual = 0;
        }
    }
    gpio_put(BOBINA_PIN_1, 0);
    gpio_put(BOBINA_PIN_2, 0);
    gpio_put(BOBINA_PIN_3, 0);
    gpio_put(BOBINA_PIN_4, 0);
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);  

    gpio_init(BOBINA_PIN_1);
    gpio_set_dir(BOBINA_PIN_1, GPIO_OUT);
    gpio_init(BOBINA_PIN_2);
    gpio_set_dir(BOBINA_PIN_2, GPIO_OUT);
    gpio_init(BOBINA_PIN_3);
    gpio_set_dir(BOBINA_PIN_3, GPIO_OUT);
    gpio_init(BOBINA_PIN_4);
    gpio_set_dir(BOBINA_PIN_4, GPIO_OUT);

    bool estado = gpio_get(BTN_PIN);
    
    while (true) {
        bool novo_estado = gpio_get(BTN_PIN);
        
        if (novo_estado != estado) {
            sleep_ms(50);  
            
            bool estado_verificado = gpio_get(BTN_PIN);
            if (estado_verificado != estado) {
                if (!estado) {
                    gpio_put(LED_PIN, 1);
                    girar_sentido_horario(); 
                    gpio_put(LED_PIN, 0);
                }
                
                estado = estado_verificado;
            }
        }
        
        sleep_ms(10);
    }
    return 0;
}
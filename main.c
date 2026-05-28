#include "stm32f4xx.h"
#include <stdint.h>

#define GPIOAEN     (1U<<0)
#define GPIOBEN     (1U<<1)
#define GPIOCEN     (1U<<2)

#define RED         (1U<<6)
#define YELLOW      (1U<<7)
#define GREEN       (1U<<8)

#define RED_OFF     (1U<<22)
#define YELLOW_OFF  (1U<<23)
#define GREEN_OFF   (1U<<24)

#define PEDESTRIAN_BUTTON (1U<<13)

#define SEG_MASK    0x7F

#define DELAY_1S        1000000UL
#define DELAY_GREEN     (DELAY_1S * 7)
#define DELAY_YELLOW    (DELAY_1S * 3)
#define DELAY_RED       (DELAY_1S * 9)
#define DELAY_ALL_RED   (DELAY_1S * 1)
#define DELAY_DEBOUNCE  50000UL

uint8_t pedestrian_request = 0;

uint8_t digit_map[10] =
{
    0x40, // 0
    0x79, // 1
    0x24, // 2
    0x30, // 3
    0x19, // 4
    0x12, // 5
    0x02, // 6
    0x78, // 7
    0x00, // 8
    0x10  // 9
};

void delay(uint32_t time)
{
    for(uint32_t i = 0; i < time; i++)
    {
        __asm__("nop");
    }
}

void display_digit(uint8_t num)
{
    GPIOB->ODR |= SEG_MASK;
    GPIOB->ODR &= ~(~digit_map[num] & SEG_MASK);
}

void display_off(void)
{
    GPIOB->ODR |= SEG_MASK;
}

void red_on(void)
{
    GPIOA->BSRR = RED | YELLOW_OFF | GREEN_OFF;
}

void yellow_on(void)
{
    GPIOA->BSRR = YELLOW | RED_OFF | GREEN_OFF;
}

void green_on(void)
{
    GPIOA->BSRR = GREEN | RED_OFF | YELLOW_OFF;
}

void all_red(void)
{
    GPIOA->BSRR = RED | YELLOW_OFF | GREEN_OFF;
    delay(DELAY_ALL_RED);
}

uint8_t button_pressed_once(void)
{
    static uint8_t button_was_pressed = 0;

    if((GPIOC->IDR & PEDESTRIAN_BUTTON) == 0)
    {
        delay(DELAY_DEBOUNCE);

        if((GPIOC->IDR & PEDESTRIAN_BUTTON) == 0 && button_was_pressed == 0)
        {
            button_was_pressed = 1;
            return 1;
        }
    }
    else
    {
        button_was_pressed = 0;
    }

    return 0;
}

void wait_and_scan_button(uint32_t total_delay)
{
    uint32_t step = 100000UL;
    uint32_t loops = total_delay / step;

    for(uint32_t i = 0; i < loops; i++)
    {
        if(button_pressed_once())
        {
            pedestrian_request = 1;
        }

        delay(step);
    }
}

void pedestrian_crossing(void)
{
    all_red();

    red_on();

    for(int i = 9; i >= 0; i--)
    {
        display_digit(i);
        delay(DELAY_1S);
    }

    display_off();

    all_red();

    pedestrian_request = 0;
}

int main(void)
{
    RCC->AHB1ENR |= GPIOAEN | GPIOBEN | GPIOCEN;

    GPIOA->MODER &= ~((3U<<12) | (3U<<14) | (3U<<16));
    GPIOA->MODER |=  ((1U<<12) | (1U<<14) | (1U<<16));

    GPIOB->MODER &= ~(0x3FFF);
    GPIOB->MODER |=  0x1555;

    GPIOC->MODER &= ~(3U<<26);
    GPIOC->PUPDR &= ~(3U<<26);

    display_off();

    while(1)
    {
        green_on();
        wait_and_scan_button(DELAY_GREEN);

        yellow_on();
        wait_and_scan_button(DELAY_YELLOW);

        if(pedestrian_request)
        {
            pedestrian_crossing();
        }
        else
        {
            red_on();
            wait_and_scan_button(DELAY_RED);
        }
    }
}

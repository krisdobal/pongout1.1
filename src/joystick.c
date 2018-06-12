/*
**
**                           joystick.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 06/06/2018 $
   Last changed by:    $Author: Kristoffer $
   Last changed date:  $Date: 11:20 06/06/2018 $
   ID:                 $Id: 1 $

**********************************************************************/
#include "stm32f30x_conf.h"
#include "30010_io.h"



void initializeJoystick(){
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port C

// Set pin PA4 /joystickup   to input
    GPIOA->MODER &= ~(0x00000003 << (4 * 2)); // Clear mode register
    GPIOA->MODER |= (0x00000000 << (4 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    GPIOA->PUPDR &= ~(0x00000003 << (4 * 2)); // Clear push/pull register
    GPIOA->PUPDR |= (0x00000002 << (4 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)

// Set pin PB0 /joystickdown to input
    GPIOB->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    GPIOB->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
    GPIOB->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)

// Set pin PB5 /joystickcenter to input
    GPIOB->MODER &= ~(0x00000003 << (5 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000000 << (5 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    GPIOB->PUPDR &= ~(0x00000003 << (5 * 2)); // Clear push/pull register
    GPIOB->PUPDR |= (0x00000002 << (5 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)

// Set pin PC0 /joystickright to input
    GPIOC->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
    GPIOC->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    GPIOC->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
    GPIOC->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)

// Set pin PC1 /joystickleft to input
    GPIOB->MODER &= ~(0x00000003 << (1 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000000 << (1 * 2)); // Set mode register (0x00 - Input, 0x01 - Output, 0x02 - Alternate Function, 0x03 - Analog in/out)
    GPIOB->PUPDR &= ~(0x00000003 << (1 * 2)); // Clear push/pull register
    GPIOB->PUPDR |= (0x00000002 << (1 * 2)); // Set push/pull register (0x00 -No pull, 0x01 - Pull-up, 0x02 - Pull-down)


}

uint8_t readJoystick(){
    uint8_t output = 0x00;
    //viderefører registrene til program variabler
    uint16_t Aval = GPIOA->IDR & (0x0010);
    //GPIOA->ODR |= (0x0001 << 1); //Set pin PA1 to high
    uint16_t Bval = GPIOB->IDR & (0x0021);
    uint16_t Cval = GPIOC->IDR & (0x0003);
    // Sets the outputregitser for each pin at a time.
    if(Aval & 0x0001 << (4)){
        output |= (0x01 << 0);
    } else output &= ~(0x01 << 0);
    if(Bval & 0x0001 << (0)){
        output |= (0x01 << 1);
    } else output &= ~(0x01 << 1);

    if(Cval & 0x0001 << (1)){
        output |= (0x01 << 2);
    } else output &= ~(0x01 << 2);

    if(Cval & 0x0001 << (0)){
        output |= (0x01 << 3);
    } else output &= ~(0x01 << 3);

    if(Bval & 0x0001 << (5)){
        output |= (0x01 << 4);
    } else output &= ~(0x01 << 4);
    return output;
}

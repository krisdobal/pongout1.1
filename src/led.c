/*
**
**                           led.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 06/06/2018 $
   Last changed by:    $Author: Kristoffer $
   Last changed date:  $Date: 15:05 06/06/2018 $
   ID:                 $Id: 1 $

**********************************************************************/
#include "stm32f30x_conf.h"
#include "30010_io.h"


void initializeLed(){
// SET PIN PA9/BLUE TO OUTPUT
GPIOA->MODER &= ~(0X00000003 << (9 * 2)); // CLEAR MODE REGISTER
GPIOA->MODER |= (0X00000001 << (9 * 2)); // SET MODE REGISTER (0X00 - INPUT, 0X01 - OUTPUT, 0X02 - ALTERNATE FUNCTION, 0X03 - ANALOG IN/OUT)
GPIOA->OSPEEDR &= ~(0X00000003 << (9 * 2)); // CLEAR SPEED REGISTER
GPIOA->OSPEEDR |= (0X00000002 << (9 * 2)); // SET SPEED REGISTER (0X01 - 10MHZ, 0X02 - 2 MHZ, 0X03 - 50 MHZ)
GPIOA->OTYPER &= ~(0X0001 << (9)); // CLEAR OUTPUT TYPE REGISTER
GPIOA->OTYPER |= (0X0001 << (9)); // SET OUTPUT TYPE REGISTER (0X00 -PUSH PULL, 0X01 - OPEN DRAIN)

// SET PIN PB4/RED TO OUTPUT
GPIOB->MODER &= ~(0X00000003 << (4 * 2)); // CLEAR MODE REGISTER
GPIOB->MODER |= (0X00000001 << (4 * 2)); // SET MODE REGISTER (0X00 - INPUT, 0X01 - OUTPUT, 0X02 - ALTERNATE FUNCTION, 0X03 - ANALOG IN/OUT)
GPIOB->OSPEEDR &= ~(0X00000003 << (4 * 2)); // CLEAR SPEED REGISTER
GPIOB->OSPEEDR |= (0X00000002 << (4 * 2)); // SET SPEED REGISTER (0X01 - 10MHZ, 0X02 - 2 MHZ, 0X03 - 50 MHZ)
GPIOB->OTYPER &= ~(0X0001 << (4)); // CLEAR OUTPUT TYPE REGISTER
GPIOB->OTYPER |= (0X0001 << (4)); // SET OUTPUT TYPE REGISTER (0X00 -PUSH PULL, 0X01 - OPEN DRAIN)

// SET PIN PC7/Green TO OUTPUT
GPIOC->MODER &= ~(0X00000003 << (7 * 2)); // CLEAR MODE REGISTER
GPIOC->MODER |= (0X00000001 << (7 * 2)); // SET MODE REGISTER (0X00 - INPUT, 0X01 - OUTPUT, 0X02 - ALTERNATE FUNCTION, 0X03 - ANALOG IN/OUT)
GPIOC->OSPEEDR &= ~(0X00000003 << (7 * 2)); // CLEAR SPEED REGISTER
GPIOC->OSPEEDR |= (0X00000002 << (7 * 2)); // SET SPEED REGISTER (0X01 - 10MHZ, 0X02 - 2 MHZ, 0X03 - 50 MHZ)
GPIOC->OTYPER &= ~(0X0001 << (7)); // CLEAR OUTPUT TYPE REGISTER
GPIOC->OTYPER |= (0X0001 << (7)); // SET OUTPUT TYPE REGISTER (0X00 -PUSH PULL, 0X01 - OPEN DRAIN)
}

void setLed(uint8_t rgb){ // for the rgb 0x04 - red lamp, 0x02 - green color, 0x01 - blue color

    //Bits:
    //  usage:  -----RGB
    //  bit:    00000000

    //Setting Red LED
    if(rgb & (0x01 << 2)){
        GPIOB->ODR &= ~(0x0001 << 4); //Resets Light
    } else{
        GPIOB->ODR |= (0x0001 << 4); //Sets Light
    }

    //Setting Green LED
    if(rgb & (0x01 << 1)){
        GPIOC->ODR &= ~(0x0001 << 7); //Resets Light
    } else{
        GPIOC->ODR |= (0x0001 << 7); //Sets Light
    }

    //Setting Blue LED
    if(rgb & (0x01 << 0)){
        GPIOA->ODR &= ~(0x0001 << 9); //Resets Light
    } else{
        GPIOA->ODR |= (0x0001 << 9); //Sets Light
    }
}





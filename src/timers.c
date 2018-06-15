/*
**
**                           timers.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 08/06/2018 $
   Last changed by:    $Author: Kristoffer $
   Last changed date:  $Date: 15:25 08/06/2018 $
   ID:                 $Id: 1 $

**********************************************************************/
#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" // Input/output library for this course
#include "timers.h"


void startTimer1(int f) { // Initiation and starting the timer,  f - Frequency
    int reloadValue = 64*1000000/f;
    RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
    TIM2->CR1 = 0x01; // Configure timer 2
    TIM2->ARR = reloadValue; // Set reload value
    TIM2->PSC = 0x00; // Set prescale value
    TIM2->DIER |= 0x0001; // Enable timer 2 interrupts
    NVIC_SetPriority(TIM2_IRQn, 2); // Set interrupt priority
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt
}

// TODO: void changeFrequency(int f){

void stopTimer1(void) { // Pauses the timer
    TIM2->CR1 = 0x00;
}

void resetTimer1(void){
    t1.centiseconds = 0;
    t1.seconds = 0;
    t1.minutes = 0;
    t1.hours = 0;
}

void TIM2_IRQHandler(void) { // interrupt code
    t1.flag = 1;
/*
    t1.centiseconds++;
    if (t1.centiseconds/100 > 0) {
        t1.seconds++;
        t1.centiseconds %= 100;
    }
    if (t1.seconds/60 > 0) {
        t1.minutes++;
        t1.seconds %= 60;
    }
    if (t1.minutes/60 > 0) {
        t1.hours++;
        t1.minutes %= 60;
   }
*/
    TIM2->SR &= ~0x0001; // Clear interrupt bit
}

//uint8_t interruptflag(){
//    if(interrupt){
//        interrupt = 0;
//        return 1;
//    }else{ return 0;}
//}

void setSplitTimeFromTimer1(funTime_t *split){
    split->centiseconds = t1.centiseconds;
    split->seconds = t1.seconds;
    split->minutes = t1.minutes;
    split->hours = t1.hours;

}


/*
void startTimer2(int f) { // Initiation and starting the timer,  f - Frequency
    int reloadValue = 64*1000000/f;
    RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
    TIM3->CR1 = 0x01; // Configure timer 2
    TIM3->ARR = reloadValue; // Set reload value
    TIM3->PSC = 0x00; // Set prescale value
    TIM3->DIER |= 0x0001; // Enable timer 2 interrupts
    NVIC_SetPriority(TIM3_IRQn, 1); // Set interrupt priority
    NVIC_EnableIRQ(TIM3_IRQn); // Enable interrupt
}

// TODO: void changeFrequency(int f){

void stopTimer2(void) { // Pauses the timer
    TIM3->CR1 = 0x00;
}

void resetTimer2(void){
    t2.centiseconds = 0;
    t2.seconds = 0;
    t2.minutes = 0;
    t2.hours = 0;
}

void TIM3_IRQHandler(void) { // interrupt code
    t2.centiseconds++;
    if (t2.centiseconds/100 > 0) {
        t2.seconds++;
        t2.centiseconds %= 100;
    }
    if (t2.seconds/60 > 0) {
        t2.minutes++;
        t2.seconds %= 60;
    }
    if (t2.minutes/60 > 0) {
        t2.hours++;
        t2.minutes %= 60;
    }
    TIM3->SR &= ~0x0001; // Clear interrupt bit
}

void setSplitTimeFromTimer2(funTime_t *split){
    split->centiseconds = t2.centiseconds;
    split->seconds = t2.seconds;
    split->minutes = t2.minutes;
    split->hours = t2.hours;

}
*/

/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f30x_conf.h"
#include "30010_io.h"

#include "lookup.h"
#include "PhysicsEngine.h"
#include "ansi.h"

#include "joystick.h"
#include "led.h"

#include "potmeter.h"

#include "timers.h"

#include "levels.h"


void loadLevel(int * levelSelect, uint32_t * currentLevel){

    switch(* levelSelect){
        case 1 :
            currentLevel = level1;
            break;
        default :
            currentLevel = defaultLevel;
    }
}

int main(void)
{
    //Initializing hardware setup
    // is initialized in Main
    init_usb_uart(115200);
    startTimer1(100);
    initializeJoystick();
    //initializeLed();
    initPot();
    init_spi_lcd();


    //Initialize game variables
    // 18.14 values
    uint32_t striker0 = 0;
    uint32_t striker1 = 0;
    uint8_t activeBalls = 0x00;
    ball_t balls[8];
    uint32_t bricks[8];
    uint8_t speed = 1;
    uint16_t score = 0x0000;
    uint8_t lives = 0x33;
    int renderCount = 0;
    int physicsCount = 0;

    // striker initial position
    loadLevel(0, bricks);//0: default
    //Initiate ball 1

    // Rendering initial positions


    while(1)
    {

        // Check if the timer have had an interrupt since last call
        if(t1.flag){
            physicsCount++;
            renderCount++;
            t1.flag = 0;
        }

        if(physicsCount > 60){//10000-speed*10){
            updatePhysics(balls, &activeBalls, &striker0, &striker1, &lives, &score, bricks);
            physicsCount = 0;
        }

        if(renderCount > 30){//10000){
            renderGame(balls, bricks, striker0, striker1);
            //updateRender();
            renderCount = 0;
        }
        // INSERT lives equal 0 gives return


    }
}


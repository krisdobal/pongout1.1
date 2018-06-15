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

//#include "charset.h"
#include <string.h>
#include "lcd.h"

void loadLevel(int * levelSelect, uint32_t * bricks_p){
    int i;
    switch(* levelSelect){
        case 1 :
            for(i=0;i<8;i++){
                bricks_p[i] = level1[i];
            }
            break;
        default :
            for(i=0;i<8;i++){
                bricks_p[i] = defaultLevel[i];
            }

    }
}

int main(void)
{
    //Initializing hardware setup
    // is initialized in Main
    init_usb_uart(115200);
    startTimer1(100);
    initPots();
    initializeJoystick();
    //initializeLed();

    init_spi_lcd();


    //Initialize game variables
    // 18.14 values
    uint32_t striker0 = 0;
    uint32_t striker1 = 0;
    uint8_t activeBalls = 0x00;
    ball_t balls[8];
    uint32_t bricks[8];
    //TODO implement speed
    uint8_t speed = 3; // value from upper main from 0, to 3 where 3 is fastest
    uint16_t score = 0x0000;
    uint8_t lives = 0x33;
    int renderCount = 0;
    int physicsCount = 0;
    uint8_t buffer[512];

   	//lcd_push_buffer(buffer);

    // striker initial position
    loadLevel(0, bricks);//0: default
    updateStrikers(&striker0, &striker1);

    newBall(&balls[0],&activeBalls,&striker0);
    //Initiate ball 1


    // Rendering initial posittions
    hideCursor();
    //renderGame(balls, bricks, striker0, striker1);
    lcdCleanScreen(buffer);
    //memset(buffer,0xAA,512);
    //lcd_push_buffer(buffer);

    while(1)
    {

        // Check if the timer have had an interrupt since last call
        if(t1.flag){
            physicsCount++;
            renderCount++;
            t1.flag = 0;
        }

        if(physicsCount > 12/(speed+1)){
            updatePhysics(balls, &activeBalls, &striker0, &striker1, &lives, &score, bricks);
            physicsCount = 0;
        }

        if(renderCount > 20){//10000){
            //renderGame(balls, bricks, striker0, striker1);
            lcdCleanScreen(buffer);
            lcdRenderGame(balls, &activeBalls, &striker0, &striker1, bricks, buffer, &lives, &score);
            //lcd_push_buffer(buffer);
            //updateRender();
            renderCount = 0;
        }
        // INSERT lives equal 0 gives return


    }
}


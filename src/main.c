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

void loadLevel(uint8_t * levelSelect_p, uint32_t * bricks_p, uint32_t * specialBricks_p){
    int i;
    switch(* levelSelect_p){
        case 1 :
            for(i=0;i<8;i++){
                bricks_p[i] = level1[i];
                specialBricks_p[i] = special1[i];
            }
            break;
        case 2 :
            for(i=0;i<8;i++){
                bricks_p[i] = level2[i];
                specialBricks_p[i] = special2[i];
            }
            break;
        default :
            for(i=0;i<8;i++){
                bricks_p[i] = defaultLevel[i];
                specialBricks_p[i] = defaultSpecial[i];
            }

    }
}

int main(void)
{
    //Initializing hardware setup
                                        // To be put in main
    init_usb_uart(115200);
    startTimer1(1500);
    initPots();
    initializeJoystick();
    init_spi_lcd();
    //initializeLed();

    //Variables to be referenced from main
    uint8_t chosenLevel = 0;
    uint8_t chosenSpeed = 3; // value from upper main from 0, to 3 where 3 is fastest

    //Initialize game variables
    // 18.14 values
    uint32_t striker0 = 0;
    uint32_t striker1 = 0;
    //arrays
    uint8_t activeBalls = 0x00;
    ball_t balls[8];
    uint32_t bricks[8];
    uint32_t specialBricks[8];

    //game statistics
    uint16_t score = 0x0000;
    uint8_t lives = 0x33;
    int gameStats = 0;

    //timing variables
    int renderCount = 0;
    int physicsCount = 0;
    uint8_t buffer[512];
    uint8_t i;


    // Initialization

    // striker initial position
    updateStrikers(&striker0, &striker1);
    loadLevel(&chosenLevel, bricks, specialBricks);//0: default

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

        if(physicsCount > 10-(chosenSpeed*3)){
            updatePhysics(balls, &activeBalls, &striker0, &striker1, &lives, &score, bricks, specialBricks);
            physicsCount = 0;
        }

        if(renderCount > 50){//10000){
            //renderGame(balls, bricks, striker0, striker1);// rendering for PuTTY
            lcdCleanScreen(buffer);
            lcdRenderGame(balls, &activeBalls, &striker0, &striker1, bricks, buffer, &lives, &score); //, specialBricks
            //lcd_push_buffer(buffer);
            //updateRender();
            renderCount = 0;
        }
        // INSERT lives equal 0 gives return
        /* Uncomment when main is inserted!
        // finds if any brick is still active
        for(i=0; i<8; i++){
            if(bricks[i] != 0){
                break;
            }
        }
        if(i == 8 && chosenLevel){ // if all bricks are gone and we are not in level0
            gameStats += score << 16;
            gameStats += (0x0001 << 0); // game outcome - all bricks destroyed, winner determined on score
            if(score)
            return gameStats;
        }
        if((lives & 0x0F) == 0x00){
            gameStats += score << 16;
            gameStats += (0x0000 << 0); // game outcome based on lives
            gameStats += (0x0001 << 1); // Player 1 has won
            return gameStats;
        } else if((lives & 0xF0) == 0x00){
            gameStats += score << 16;
            gameStats += (0x0000 << 0); // game outcome based on lives
            gameStats += (0x0000 << 1); // Player 0 has won
            return gameStats;
        }
        */



    }
}


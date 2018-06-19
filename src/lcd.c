
#include "charset.h"
#include "stm32f30x_conf.h"
#include "PhysicsEngine.h"
#include "30010_io.h"
#include <string.h>

void lcdRenderChar(int slice, uint8_t line, uint8_t c, uint8_t * buffer_p){//char c, uint8_t * buffer_p){
    for(int i=0; i<5; i++){
        buffer_p[slice+(128*line)+i] = character_data[c][i];
    }
}

void lcdRenderArrow(uint8_t line, uint8_t * buffer_p){//char c, uint8_t * buffer_p){
    for(int i=0; i<4; i++){
        if(i == line){
            lcdRenderChar(128-5,i,0x1E, buffer_p);
        } else{
            lcdRenderChar(128-5,i,0, buffer_p);
        }
    }
}

void lcdRenderString(int slice, uint8_t line, uint8_t * string_p, uint8_t * buffer_p){
    int i = 0;
    int l = strlen(string_p);
    while(slice<(128-5)){
        if(i >= l) break;
        lcdRenderChar(slice, line, (string_p[i]-0x20), buffer_p);
        slice += 5;
        i++;
    }
}

void lcdCleanScreen(uint8_t * buffer_p){
    memset(buffer_p, 0x00, 512);
    lcd_push_buffer(buffer_p);
}

void lcdRenderBricks(uint32_t * bricks_p, uint8_t * buffer_p){
    //This funciton renders all the bricks.
    // they're rendered two bricks at a time, on top of each other.

    //Brickdata explanation:
    //Top Brick:    0x0F
    //Bottom Brick: 0xF0
    //Both bricks:  0xFF
    //No Bricks:    0x00
    int i, j;

    //Render each buffer line
    for(i=0; i<4; i++){
        //Each brick
        for(j=0; j<32; j++){
            //Finds the value to go into the postition in the array
            uint8_t brickData = 0x00;
            if(bricks_p[(i<<1)]   & (0x00000001<<j)) brickData += 0x0F;
            if(bricks_p[(i<<1)+1] & (0x00000001<<j)) brickData += 0xF0;

            buffer_p[(i<<7)+31+(j<<1)]   = brickData;
            buffer_p[(i<<7)+31+(j<<1)+1] = brickData;
        }

    }
    //MATH EXPLANATION:
    // (i<<8)   - i*128 - Shift the location by i rows.
    // +33              - 33 is the offset the bricks' coords have
    // +(j<<1)  - j*2   - j is the brick index, *2 takes brick width into account.
    // +1               - The other side of the rendered bricks.
}

//TODO : Actual life and score data
void renderDecorations(uint8_t * buffer_p,  uint8_t * lives_p, uint16_t * score_p){
    //Draw the border
    for(int i=0; i<128; i++){
        buffer_p[i]     |= 0x01; //Top
        buffer_p[384+i] |= 0x80; //Bottom
    }
    for(int i=0; i<4; i++){
        buffer_p[128*i]         |= 0xFF;  //Left edge
        buffer_p[(128*i) + 6]   |= 0xFF;  //Behind left paddle
        buffer_p[(128*i) + 121] |= 0xFF;  //Behind right paddle
        buffer_p[(128*i) + 127] |= 0xFF;  //Right edge
    }

    //Setting the char for the lives
    uint8_t lives0Char, lives1Char;
    switch (*lives_p & 0x0F){
        case 0x00 :
            lives0Char = 0x6A;
        break;
        case 0x01 :
            lives0Char = 0x6B;
        break;
        case 0x02 :
            lives0Char = 0x6C;
        break;
        case 0x03 :
            lives0Char = 0x6D;
        break;
        default :
            lives0Char = 0x8D;
    }
    switch (*lives_p  >> 4){
        case 0x00 :
            lives1Char = 0x6A;
        break;
        case 0x01 :
            lives1Char = 0x6B;
        break;
        case 0x02 :
            lives1Char = 0x6C;
        break;
        case 0x03 :
            lives1Char = 0x6D;
        break;
        default :
            lives1Char = 0x8D;

    }
    //Lives
    lcdRenderChar(1,3,lives0Char, buffer_p); // Can be implemented without the switch as 0x6A + (*lives_p & 0x0F)
    lcdRenderChar(122,3,lives1Char, buffer_p);

/*
    //Setting the char for score
    uint8_t score0Onechar, score0TenChar, score1Onechar, score1TenChar;
    switch (*score_p & 0x000F){
        case 0x0000 :
            score0Onechar = ;
        break;
    }
*/

    //Scores
    lcdRenderChar(1,0,0x60 + ((* score_p & 0x00FF)/0x000A) ,buffer_p);
    lcdRenderChar(1,1,0x60 + ((* score_p & 0x00FF)%0x000A) ,buffer_p);
    lcdRenderChar(1,2,0x5F,buffer_p);
    lcdRenderChar(122,0,0x60 + (((* score_p & 0xFF00)>>8)/0x000A),buffer_p);
    lcdRenderChar(122,1,0x60 + (((* score_p & 0xFF00)>>8)%0x000A),buffer_p);
    lcdRenderChar(122,2,0x5F,buffer_p);


}

void lcdRenderStrikers(uint32_t *striker0, uint32_t *striker1, uint8_t * buffer){

    //Striker 0:

    //Placement represents entire vertical line.
    //3f is 00111111 in binary. AKA striker width
    uint32_t strikerPlacement = 0x0000003f;

    // strikerPlacement is bitshifted by the rounded position of the striker,
    //  thus placing the bits correctly
    strikerPlacement <<= (* striker0 >> 14);

    // Copy the placement to the graphics buffer
    for(int i=0; i<4; i++){
        buffer[(i<<7)+8] |= strikerPlacement>>(i<<3);
    }

    //Striker 1:
    strikerPlacement = 0x0000003f;
    strikerPlacement <<= (* striker1 >> 14);
    for(int i=0; i<4; i++){
        buffer[(i<<7)+119] |= strikerPlacement>>(i<<3);
    }

}

void lcdRenderBalls(ball_t * balls_p, uint8_t * activeBalls_p, uint8_t * buffer_p){
    /*
    //Macros for readability
    #define X (balls[i].xpos>>14)
    #define Y (balls[i].ypos>>14)
    //Go through the balls
    for(int i=0; i<8; i++){
        //Is this ball active?
        if((0x01<<i) & * activeBalls){
            //Set that bit in the graphics buffer
            buffer[X*(Y/8)] |= (Y % 8);
        }
    }
    #undef X
    #undef Y
    */
    int i, x, y;
    for(i=0; i<8;i++){
        if((0x01<<i) & * activeBalls_p){
            x = balls_p[i].xpos>>14;
            y = balls_p[i].ypos>>14;
            buffer_p[x + ((y>>3)<<7)] |= (0x01<<(y % 8));
        }
    }
}

void lcdRenderGame(ball_t * balls_p, uint8_t * activeBalls_p, uint32_t * striker0_p, uint32_t * striker1_p, uint32_t * bricks_p, uint8_t * buffer_p, uint8_t * lives_p, uint16_t * score_p){
    // Bricks rendered before decorations
    // Because bricks can overlap with the border
    lcdRenderBricks(bricks_p, buffer_p);
    renderDecorations(buffer_p, lives_p, score_p); //TODO : scores and lives
    lcdRenderStrikers(striker0_p, striker1_p, buffer_p);
    lcdRenderBalls(balls_p, activeBalls_p, buffer_p);
    lcd_push_buffer(buffer_p);
}

void lcdRenderHelpScreen(int * buffer_p) {
    lcdRenderString(1, 0, "Last survivor wins. If  ", buffer_p);
    lcdRenderString(1, 1, "all bricks are gone, the", buffer_p);
    lcdRenderString(1, 2, "player with the most    ", buffer_p);
    lcdRenderString(1, 3, "points wins.       EXIT ", buffer_p);
    lcdRenderArrow(3, buffer_p);
    lcd_push_buffer(buffer_p);
}


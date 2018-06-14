
#include "charset.h"

lcd_render_char(int slice, uint8_t line, char c, uint8_t * buffer){
    for(int i=0; i<5; i++){
        buffer[slice+(128*line)+1] = character_data[c][i];
    }
}

renderBricks(uint32_t * bricks, uint8_t * buffer){
    //This funciton renders all the bricks.
    // they're rendered two bricks at a time, on top of each other.

    //Brickdata explanation:
    //Top Brick:    0x0F
    //Bottom Brick: 0xF0
    //Both bricks:  0xFF
    //No Bricks:    0x00

    //Render each buffer line
    for(int i=0; i<4; i++){
        //Each brick
        for(int j=0; j<32; j++){
            uint8_t brickData = 0x00;
            if(bricks[(i<<1)]   & (0x00000001<<j)) brickData += 0x0F;
            if(bricks[(i<<1)+1] & (0x00000001<<j)) brickData += 0xF0;

            buffer[(i<<8)+33+(j<<1)] = brickData;
            buffer[(i<<8)+33+(j<<1)+1] = brickData;
        }
    }
    //MATH EXPLANATION:
    // (i<<8)   - i*128 - Shift the location by i rows.
    // +33              - 33 is the offset the bricks' coords have
    // +(j<<1)  - j*2   - j is the brick index, *2 takes brick width into account.
    // +1               - The other side of the rendered bricks.
}

//TODO : Actual life and score data
void renderDecorations(uint8_t * buffer){
    //Draw the border
    for(int i=0; i<128; i++){
        buffer[i]     |= 0x80; //Top
        buffer[384+i] |= 0x01; //Bottom
    }
    for(int i=0; i<4; i++){
        buffer[128*i]         |= 0xFF;  //Left edge
        buffer[(128*i) + 6]   |= 0xFF;  //Behind left paddle
        buffer[(128*i) + 121] |= 0xFF;  //Behind right paddle
        buffer[(128*i) + 127] |= 0xFF;  //Right edge
    }

    //Lives (example values)
    lcd_draw_char(1,3,0x8D, buffer);
    lcd_draw_char(122,3,0x8C, buffer);

    //Scores (blank values)
    lcd_draw_char(1,0,0x7F,buffer);
    lcd_draw_char(1,1,0x7F,buffer);
    lcd_draw_char(1,2,0x7F,buffer);
    lcd_draw_char(122,0,0x7F,buffer);
    lcd_draw_char(122,1,0x7F,buffer);
    lcd_draw_char(122,2,0x7F,buffer);

}

void renderStrikers(uint32_t striker0, uint32_t striker1, uint8_t * buffer){

    //Striker 0:

    //Placement represents entire vertical line.
    //3f is 00111111 in binary. AKA striker width
    uint32_t strikerPlacement = 0x0000003f

    // strikerPlacement is bitshifted by the rounded position of the striker,
    //  thus placing the bits correctly
    strikerPlacement <<= (striker0 >> 14);

    // Copy the placement to the graphics buffer
    for(int i=0; i<4; i++){
        buffer[(i<<7)+8] |= strikerPlacement>>(i<<3);
    }

    //Striker 1:
    strikerPlacement = 0x0000003f;
    strikerPlacement <<= (striker1 >> 14);
    for(int i=0; i<4; i++){
        buffer[(i<<7)+119] |= strikerPlacement>>(i<<3);
    }

}

renderBalls(ball_t * balls, uint8_t * activeBalls. uint8_t * buffer){
    //Macros for readability
    #define X (balls[i].xpos>>14)
    #define Y (balls[i].ypos>>14)
    //Go through the balls
    for(int i=0; i<8; i++){
        //Is this ball active?
        if((0x01<<i) & activeBalls){
            //Set that bit in the graphics buffer
            buffer[X*(Y/8)] |= (Y % 8);
        }
    }
    #undef X
    #undef X
}

void lcd_renderGame(ball_t * balls, uint8_t * activeBalls, uint32_t * striker0, uint32_t * striker1, uint32_t * bricks, uint8_t * buffer){
    // Bricks rendered before decorations
    // Because bricks can overlap with the border
    renderBricks(bricks, buffer);
    renderDecorations(buffer); //TODO : scores and lives
    renderStrikers(striker0, striker1, buffer);
    renderBalls(balls, activeballs, buffer);

    lcd_push_buffer(buffer);
}

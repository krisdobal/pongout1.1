#define ESC 0x1B

#include "stm32f30x_conf.h" // STM32 config
#include "PhysicsEngine.h"
#include <stdio.h>
#include <stdlib.h>



void fgcolor(int foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
    int type = 22;             // normal text
    if (foreground > 7) {
        type = 1;                // bold text
        foreground -= 8;
    }
    printf("%c[%d;%dm", ESC, type, foreground + 30);
}

void bgcolor(int background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
    printf("%c[%dm", ESC, background + 40);
}

void color(int foreground, int background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
    int type = 22;             // normal text
    if (foreground > 7) {
        type = 1;                // bold text
        foreground -= 8;
    }
    printf("%c[%d;%d;%dm", ESC, type, foreground + 30, background + 40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
    printf("%c[m", ESC);
}

void clrscr() {
    printf("%c[2J", ESC);
}

void clrrol() {
    printf("%c[K", ESC);
}

void gotoxy(int x, int y) {
    //printf("%c[H",ESC);
    printf("%c[%d;%df", ESC, y, x);
}

void underline(uint8_t on) {
    on ? printf("%c[4m", ESC) : printf("%c[24m", ESC);
}

void blink(uint8_t on) {
    on ? printf("%c[5m", ESC) : printf("%c[25m", ESC);
}

void inverse(uint8_t on) {
    on ? printf("%c[7m", ESC) : printf("%c[27m", ESC);
}

//Reset formatting.
void reset() {
    printf("%c[m", ESC);
}

//hide cursor
void hideCursor() {
    printf("%c[?25l",ESC);
}


// Helper function for window(). Takes coordinates, end and middle characters and line length
void printFrameLine(uint8_t x, uint8_t y, char left, char middle, char right, uint8_t length) {
    char * line = calloc(length + 1,sizeof (char));
    line[0] = left;
    int i;
    for (i = 1; i < length - 1; i++) {
        line[i] = middle;
    }
    line[length - 1] = right;
    line[length] = '\0';
    gotoxy(x, y);
    printf("%s", line);
    free(line);
}

//renders a window
void window(frame_t * frame_p, int style, char * title_p) {

    uint8_t db = 14; // number of decimal bits in the frame type fields

    //convert frame fields to uint8_t
    uint8_t TLx = (frame_p->TLx >> db);
    uint8_t TLy = (frame_p->TLy >> db);
    uint8_t BRx = (frame_p->BRx >> db);
    uint8_t BRy = (frame_p->BRy >> db);

    //Title_p == 0 means no title.

    //TODO : Should colors be supported?

    // Checks for bad input.
    if (TLx >= BRx) return;
    if (TLy >= BRy) return;

    //TODO : Test for <= 0 and >= terminal size.
    // Should out of range box placement be handled here as an input error, or rendered?

    uint8_t BG; // Background
    uint8_t VL; // Vertical Line
    uint8_t HL; // Horsizontal Line
    uint8_t TL; // Top Left corner
    uint8_t TR; // Top Right corner
    uint8_t BL; // Bottom Left corner
    uint8_t BR; // Bottom Right corner

    switch (style) {

        // STYLE 1
        // - Double line
        // - Title on border w. decoration
        case 1 :
            BG = 32;
            VL = 186;
            HL = 205;
            TL = 201;
            TR = 187;
            BL = 200;
            BR = 188;
            break;

        // STYLE 2
        // Solid sides, blank BG
        // No title
        case 2 :
            BG = 0x20;
            VL = 0xDB;
            HL = 0xDB;
            TL = 0xDB;
            TR = 0xDB;
            BL = 0xDB;
            BR = 0xDB;
            break;

            // DEFAULT STYLE
            // - single line
            // - title within border
            // Implicit style 0
        default :
            BG = 32;
            VL = 179;
            HL = 196;
            TL = 218;
            TR = 191;
            BL = 192;
            BR = 217;
    }
    /*
      RENDERING:
      Rendering happens top to bottom, line for line.
      A whole line is rendered  left-to-right, then
      gotoxy() is called to move the cursor to the
      beginning of the next line.
      The title is then rendered on top, if necessary.
    */

    uint8_t length = BRx - TLx + 1;

    //Top line
    printFrameLine(TLx, TLy, TL, HL, TR, length);

    //Middle lines (outcommented for testing)
    int y;
    for (y = TLy + 1; y < BRy; y++) {
         printFrameLine(TLx, y, VL, BG, VL, length);
    }

    //Bottom line
    printFrameLine(TLx, BRy, BL, HL, BR, length);

    //Title rendering
    if (title_p != 0) {
        switch (style) {
            case 1 :
                gotoxy(TLx + 1, TLy);
                printf("%c", 185);
                printf("%s", title_p);
                printf("%c", 204);
                break;

            case 2 :
                break;

            default :
                gotoxy(TLx + 1, TLy + 1);
                printf("%s", title_p);
        }
    }
    /*
    //free memory
    free(&TLx);
    free(&TLy);
    free(&BRx);
    free(&BRy);
    free(&BG);
    free(&VL);
    free(&HL);
    free(&TL);
    free(&TR);
    free(&BL);
    free(&BR);
    */
}

// takes pointer to ball_struct and renders it according to the postion.
void renderBall(ball_t * ball_p) {
    //Rounds an 18.14 fixed point number down.
    int rendX = (ball_p->xpos) >> 14;
    int rendY = (ball_p->ypos) >> 14;

    gotoxy(rendX, rendY);
    printf("o");
}

void renderBricks(uint32_t * bricks){
    //Doesn't render special bricks!
    int i, j;
    // Whole rows
    for(i=0; i<8;i++){
        //Single lines
        for(j=0; j<4; j++){
            gotoxy(31,(i*4)+j);//*8)+j);
            //Single block slices
            for(int k=0; k<32; k++){
                if(bricks[i] & 0x00000001<<k){
                    printf("%c", 0xDB);
                    printf("%c", 0xDB);
                }else{
                    printf("%c", 0x20);
                    printf("%c", 0x20);
                }
            }
        }
    }
}

void renderStrikers(int striker0, int striker1){
    //Striker 0
    for(int i=0;i<6;i++){
        gotoxy(8,(striker0>>14)+i);
        printf("%c", 0xDB);
    }
    //Striker 1
    for(int i=0;i<6;i++){
        gotoxy(119,(striker1>>14)+i);
        printf("%c", 0xDB);
    }
}

// Renders PongOut game
void renderGame(ball_t * ball_p, uint32_t * bricks, int striker0, int striker1){
    clrscr();

    frame_t frame;
    frame.TLx = 0;
    frame.TLy = 0;
    frame.BRx = 127;
    frame.BRy = 31;
    window(&frame,0,0);

    frame_t left;
    left.TLx = 0;
    left.TLy = 0;
    left.BRx = 6;
    left.BRy = 31;
    window(&left,2,0);

    frame_t right;
    right.TLx = 121;
    right.TLy = 0;
    right.BRx = 127;
    right.BRy = 31;
    window(&right,2,0);

    renderBricks(bricks);
    renderStrikers(striker0, striker1);

    renderBall(&ball_p[0]);
}

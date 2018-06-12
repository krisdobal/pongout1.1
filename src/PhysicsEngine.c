//.c file for all internal 'physics' functions and structs in the program
#include "PhysicsEngine.h"
#include "stm32f30x_conf.h"

// Uses 18.14 fixed-point integers

// general one-dimensional reflect function taking moving object position and
// velocity plus statical object position.
void reflect(uint32_t * pos_p, int wall, uint32_t * v_p){
    //This function assumes good input.
    //All checks must be made before calling it.
    *pos_p = (wall << 1) - *v_p - *pos_p;
    *v_p *= -1;
}

void moveBall(ball_t * ball_p){
    ball_p->xpos += ball_p->xv;
    ball_p->ypos += ball_p->yv;
}

//Checks for collisions on walls and updates ball accordingly
uint8_t wallCollision(ball_t * ball_p) {
    if (ball_p->xpos + ball_p->xv < (1 << 14)) {
        reflect(&ball_p->xpos, (1 << 14), &ball_p->xv);
        return 1;
    } else if (ball_p->xpos + ball_p->xv > (31 << 14)) {
        reflect(&ball_p->xpos, (31 << 14), &ball_p->xv);
        return 1;
    } else{
        return 0;
    }
}

// TODO: Delete ball from ball-array
uint8_t endCollision(ball_t * ball_p, uint8_t * lives_p) {
    if (ball_p->ypos + ball_p->yv < (7 << 14)) {
        *lives_p-=0x01;
        return 1;
    } else if (ball_p->ypos + ball_p->yv > (121 << 14)) {
        *lives_p-=0x10;
        return 1;
    } else{
        return 0;
    }
}

uint8_t strikerCollision(ball_t * ball_p, uint32_t * striker0, uint32_t * striker1) {
    int nextX = ball_p->xpos + ball_p->xv;
    int nextY = ball_p->ypos + ball_p->yv;

    // Striker coordinate is 18.14 and upper coordinate on striker

    //STRIKER 0
    // check if ball passes the line where the striker0 can move.
    if (ball_p->xpos > (9 << 14) && nextX <= (9 << 14)) {
        //check if the ball hits the striker.
        if (nextY >= (* striker0) && nextY < (* striker0) + (6 << 14)) {

            // Deduce normal reflect angle from last striker hit
            // if/else structure needs to be compressed
/*            if(!ball_p->lastStriker){
                if (ball_p->yv > 0){
                    if (ball_p->angle < 128){
                        ball_p->angle = 511 - ball_p->angle;
                    }
                }
                else {
                    if (ball_p->angle > 127){
                        ball_p->angle = 511 - ball_p->angle;
                }
            }
            else {
                if (ball_p->yv > 0){
                    if (ball_p->angle < 128){
                        ball_p->angle = -ball_p->angle;
                    }
                    else {
                        ball_p->angle = 640 - ball_p->angle;
                    }
                }
                else {
                    if (ball_p->angle < 128){
                        ball_p->angle = 128 - ball_p->angle;
                    }
                    else {
                        ball_p->angle = - ball_p->angle; //adding a full revolution to avoid negative angles
                    }
                }
            }
*/
            if (XOR3(ball_p->lastStriker, ball_p -> angle > 127 && ball_p -> angle < 512, ball_p->yv > 0)){
                ball_p->angle = 512 - ball_p->angle;
            }
            if (ball_p->lastStriker) {
                ball_p->angle += 128;
            }

            reflect(&ball_p->xpos, 9 << 14, &ball_p->xv);
            //check where it hits and adjust angle accordingly.
            if (nextY < (* striker0) + (1 << 14)){
                ball_p->angle += (127 - ball_p->angle) >> 1;
            }
            else if (nextY < (* striker0) + (2 << 14)){
                ball_p->angle += (127 - ball_p->angle) >> 2;
            }
            else if (nextY < (* striker0) + (4 << 14)){}
            else if (nextY < (* striker0) + (5 << 14)){
                ball_p->angle -= (ball_p->angle - 383) >> 2;
            }
            else if (nextY >= (* striker0) + (5 << 14) && nextY < * striker0 + (6 << 14)) {
                ball_p->angle -= (ball_p->angle - 383) >> 1;
            }
            //adjust velocity vector according to new angle.
            uint32_t fixcos = fix14cos(ball_p->angle);
            ball_p->xv = FIX14MULT(ball_p->v, fixcos);
            ball_p->yv = FIX14MULT(ball_p->v, fix14sin(ball_p->angle));
            /*
            free(&nextX);
            free(&nextY);
            */
            return 1;
        }
    }

    //STRIKER 1
    // check if the ball passes the line where the striker1 can move.
    if (ball_p->xpos < (31 << 14) && nextX >= (31 << 14)) {
        //check if the ball hits the striker.
        if (nextY >= * striker0 && nextY < * striker0 + (6 << 14)) {

            if (XOR3(!ball_p->lastStriker, ball_p -> angle > 127 && ball_p -> angle < 512, ball_p->yv > 0)){
                ball_p->angle = 512 - ball_p->angle;
            }
            if (ball_p->lastStriker) {
                ball_p->angle += 128;
            }


            reflect(&ball_p->xpos, 9 << 14, &ball_p->xv);
            //check where it hits and adjust angle accordingly.
            if (nextY < * striker0 + (1 << 14)) {
                ball_p->angle -= (ball_p->angle - 127) >> 1;
            }
            else if (nextY < * striker0 + (2 << 14)){
                ball_p->angle -= (ball_p->angle - 127) >> 2;
            }
            else if (nextY < * striker0 + (4 << 14)){}
            else if (nextY < * striker0 + (5 << 14)){
                ball_p->angle += (383 - ball_p->angle) >> 2;
            }
            else if (nextY >= * striker0 + (5 << 14) && nextY < * striker0 + (6 << 14)){
                ball_p->angle += (383 - ball_p->angle) >> 1;
            }
            //adjust velocity vector according to new angle.
            ball_p->xv = FIX14MULT(ball_p->v, fix14cos(ball_p->angle));
            ball_p->yv = FIX14MULT(ball_p->v, fix14sin(ball_p->angle));
            /*
            free(&nextX);
            free(&nextY);
            */
            return 1;
        }
    }
    /*
    free(&nextX);
    free(&nextY);
    */
    return 0;
}


uint8_t brickCollision(ball_t * ball_p, uint16_t * score, uint32_t * bricks){

    // TODO:
    // Special bricks not implemented
    // Score updating not implemented
    // Placeholder for level-info (bricks).

   /* About bit-shiting in this function:
    *   >>14 - Conversion from 18:14 to int.
    *   >>15 - Conversion and division by two.
    *       Used for calculating x-coordinates for bricks.
    *       Because bricks are two wide
    *   >>16 - Conversion and division by four.
    *       Used for calculating y-coordinates for bricks.
            Because bricks are four tall.
    */

    // Are we even near the bricks?
    if(ball_p->xpos>>14 > 96) return 0;
    if(ball_p->xpos>>14 < 32) return 0;

    // Remembering old positions for double-bouncing.
    // Without these, a ball might end up inside a brick.
    int oldx = ball_p->xpos;
    int oldy = ball_p->ypos;

    //Helper value
    int nextx = oldx+ball_p->xv;

    // Return value.
    // Indicates if a brick has been hit.
    uint8_t retval = 0;

    /* Generally about the collision detection:
     *
     * The block the ball collides with is assumed
     *  to be the one closest to the original position.
     * This not only simplifies detection, but also
     *  speeds it up. Change it if it feels bad.
     * This works best visually, because the physics
     *  is faster and more accurate than the rendering.
     *
     * X and Y are checked separately.
     * This should allow corner bounces.
     */

    // X

    // Are we crossing a whole-numbered coordinate?
    if(oldx>>15 != nextx>>15){

        //Calculate brick index values
        uint32_t decoded_x = 0x00000001<<((nextx-(33<<14))>>15);
        uint8_t iy = oldy>>16;

        // Is the brick we're "hitting" there?
        //currentLevel is a placeholder for the level (brick) data!!!!
        if(bricks[iy] & decoded_x){
            if(ball_p->xv > 0) {
                // Hitting left edge of brick:
                reflect(&ball_p->xpos, nextx>>14, &ball_p->xv);
            }else{
                // Hitting right edge of brick:
                reflect(&ball_p->xpos, oldx>>14, &ball_p->xv);
            }
            //Flip the bit
            bricks[iy] ^= decoded_x;
            // update the score of the hitting player
            if(ball_p->lastStriker){
                * score += 0x10;
            }else{
                * score += 0x01;
            }
            retval = 1;
        }
    } // End x


    // Helper value
    int nexty = oldy+ball_p->yv;

    // Y
    if(oldy>>16 != nexty>>16){

        //Calculate brick index values
        uint32_t decoded_x = 0x00000001<<((oldx-33)>>15);
        uint8_t iy = nexty>>16;

        // Is the brick we're "hitting" there?
        //currentLevel is a placeholder for the level (brick) data!!!!
        if(bricks[iy] & decoded_x){
            if(ball_p->yv > 0) {
                // Hitting top of brick:
                reflect(&ball_p->ypos, oldy>>14, &ball_p->yv);
            }else{
                // Hitting bottom of brick:
                reflect(&ball_p->ypos, nexty>>14, &ball_p->yv);
            }
            //Flip the bit
            bricks[iy] ^= decoded_x;
            // update the score of the hitting player
            if(ball_p->lastStriker){
                * score += 0x10;
            }else{
                * score += 0x01;
            }
            retval = 1;
        }
    } // End y
    return retval;
} // End brickCollision

//Adds a new ball. Only called when there are no balls.
void newBall(ball_t * ball_p, uint8_t * activeBalls, uint32_t * striker0){
    // TODO
    // Always spawns at player 0, BUT IT's Just a dummy pointer to position 0
    // Always spawns ball 0

    //Coords in 18:14
    ball_p->xpos = 9<<14;
    ball_p->ypos = *striker0 + (3<<14);
    ball_p->xv = 5;
    ball_p->yv = 5;
    ball_p->lastStriker = 0;
    ball_p->angle = 128;

    //Activate ball 0
    * activeBalls |= 0x01;
}

/*
updateStrikers(uint32_t * striker0_p, uint32_t * striker1_p) {
	//transform potmetervalue to coordinate range of strikers
	*striker0_p = (1 << 14) + (readPot0()*(24 << 14))/4096;
	*striker0_p = (1 << 14) + (readPot1()*(24 << 14))/4096;
}
*/

//Checks all collisions, updates accordingly.
// For one ball only.
void updatePhysics(ball_t * ball_p, uint8_t * activeBalls_p, uint32_t * striker0_p, uint32_t * striker1_p, uint8_t * lives_p, uint16_t * score_p, uint32_t * bricks_p){

    //Assumes no balls
    uint8_t noBalls = 1;
    uint8_t i;

    // TODO Add update striker position

    for(i = 0; i<8; i++){
        if((*activeBalls_p) & (0x01<<i)){continue;}
        if(wallCollision(&ball_p[i])){}
        else if(endCollision(&ball_p[i], lives_p)){}
        else if(strikerCollision(&ball_p[i], striker0_p, striker1_p)){}
        else if(brickCollision(&ball_p[i], score_p, bricks_p)){}
        else moveBall(&ball_p[i]);
        noBalls = 0; //There are balls! Hooray!
    }
    if(noBalls){
        newBall(ball_p, activeBalls_p, striker0_p);
    }
}


#ifndef _PHYSICSENGINE_H_
#define _PHYSICSENGINE_H_

#include "stm32f30x_conf.h"

#define XOR3(a, b, c) ( ((a) && !(b) && !(c)) || (!(a) && (b) && !(c)) || (!(a) && !(b) && (c)) || ((a) && (b) && (c)) )
#define XOR2(a, b) ( ((a) && !(b)) || (!(a) && (b)) )
/* PhysicsEngine.h
 * contains ball and frame struct, and prototypes for the physics functions.
 */

 #define FIX14_SHIFT 14
 #define FIX14MULT(a, b) ( ((a)*(b)) >> FIX14_SHIFT )
 #define FIX14DIV(a, b) ( ((a) << FIX14_SHIFT) / b )

 //Ball struct containing position and velocity
typedef struct {
    // 18.14
    uint32_t xpos;
    uint32_t ypos;
    uint32_t xv;
    uint32_t yv;
    uint32_t v; // magnitude of velocity

    uint8_t lastStriker; // the first bit indicates the striker that last hit the ball
    int angle;
} ball_t;

//Frame struct containing coordinates of upper left and bottom right corner.
typedef struct {
    int TLx;
    int TLy;
    int BRx;
    int BRy;
} frame_t;

//function declarations
void updatePhysics(ball_t * ball_p, uint8_t * activeBalls_p, uint32_t * striker0_p, uint32_t * striker1_p, uint8_t * lives_p, uint16_t * score_p, uint32_t * bricks_p);
void updateStrikers(uint32_t * striker0_p, uint32_t * striker1_p);
void newBall(ball_t * ball_p, uint8_t * activeBalls, uint32_t * striker0_p);

#endif /* _PHYSICSENGINE_H_ */

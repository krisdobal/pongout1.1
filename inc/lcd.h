
#ifndef _LCD_H_
#define _LCD_H_


void lcdRenderString(int slice, uint8_t line, uint8_t * string_p, uint8_t * buffer_p);
void lcdRenderGame(ball_t * balls_p, uint8_t * activeBalls_p, uint32_t * striker0_p, uint32_t * striker1_p, uint32_t * bricks_p, uint8_t * buffer_p, uint8_t * lives_p, uint16_t * score_p);

void lcdCleanScreen(uint8_t * buffer_p);

void lcdRenderHelpScreen(uint8_t * buffer_p);

#endif /* _LCD_H_ */

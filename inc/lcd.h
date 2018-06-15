
#ifndef _LCD_H_
#define _LCD_H_

void lcdRenderString(int slice, uint8_t line, uint8_t * string_p, uint8_t * buffer_p);
void lcdRenderGame(ball_t * balls, uint8_t * activeBalls, uint32_t * striker0, uint32_t * striker1, uint32_t * bricks, uint8_t * buffer);
void lcdCleanScreen(uint8_t * buffer_p);

#endif /* _LCD_H_ */

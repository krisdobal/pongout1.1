

#ifndef _ANSI_H_
#define _ANSI_H_

void fgcolor(int foreground);
void bgcolor(int background);
void color(int foreground, int background);
void resetbgcolor();
void clrscr();
void clrrol();
void gotoxy(int x, int y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void reset();
void window(frame_t * frame_p, int style, char * title_p);
void bufferToAnsi(uint8_t * buffer_p);
void renderGame(ball_t * ball_p, uint32_t * bricks, int striker0, int striker1);
void hideCursor();

#endif /* _ANSI_H_ */

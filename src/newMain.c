#include "stm32f30x_conf.h"
#include "30010_io.h"

#include "inGameMain.h"
#include "ansi.h"
#include "joystick.h"

//#include "charset.h"
#include <string.h>
#include "lcd.h"

void helpScreen(uint8_t * buffer_p);
void endScreen(uint32_t gameStats, uint8_t * buffer_p);

int newMain(void) {
  
  //Initializing hardware
  init_usb_uart(115200);
  startTimer1(100);
  initPots();
  startScreen();
  init_spi_lcd();
  
  //Show fancy start screen
  startScreen();
  
  //Initializing game variables
  uint8_t level = 0;
  uint8_t speed = 0;
  
  while(1) {
      //Show main menu
      char * mainOption0 = "change level";
      char * mainOption1 = "change speed";
      char * mainOption2 = "help";
      char * mainOption3 = "PLAY";
      uint8_t next_action = "
      menu(mainOption0, mainOption1, mainOption2, mainOption3, buffer_p);
      
      switch (nextAction) {
          case 0 :
              char * levelOption0 = "Pong";
              char * levelOption1 = "Vanilla";
              char * levelOption2 = "rainbow Road";
              char * levelOption3 = "HELL";
              menu(levelOption0, levelOption1, levelOption2, levelOption3, &level);
              break;
          case 1 :
              char * speedOption0 = "mindful";
              char * speedOption1 = "medium raw";
              char * speedOption2 = "speedy gonzales";
              char * speedOption3 = "warp speed";
              menu(levelOption0, levelOption1, levelOption2, levelOption3, &speed);
              break;
          case 2 : 
              helpScreen(buffer_p);
              break;
          case 3 : 
              endScreen(startGame(level, speed));
              break;          
      }  
  }
}

void menu(char * opt0, char * opt1, char * opt2, char * opt3, uint8_t * option_p, uint8_t * buffer_p) {
  
    uint8_t pressed = 1;
    uint8_t stillDeciding = 1;
    
    renderString(0, 0, opt0, buffer_p);
    renderString(0, 1, opt1, buffer_p);
    renderString(0, 2, opt2, buffer_p);
    renderString(0, 3, opt3, buffer_p); 
      
    while(stillDeciding) {
        if (!pressed) {
            if (readJoystick() & 0x01 << 0) {
                *option_p = (*option_p + 1) % 4;
                pressed = 1;
            }
            else if (readJoystick() & 0x01 << 1) {
                *option_p = (*option_p + 3) % 4;
                pressed = 1;
            }
            else if (readJoystick() & 0x01 << 3) {
                stillDeciding = 0;
                pressed = 1;
            }
            renderArrow(option, buffer_p);
        }
        else if (!(readJoystick() & 0x0F) {
            pressed = 0;
        }
    }
}
                 
void helpScreen(uint8_t * buffer_p) {
    lcdRenderHelpScreen(buffer_p);
    while(!(readJoystick() & 0x01 << 3)) {}
}
                 
void endScreen(uint32_t gameStats, uint8_t * buffer_p) {
    lcdRenderString(20, 1, "...and the winner is:", buffer_p);
    if (gameStats & 0x02) {
        lcdRenderString(20, 2, "Player 0", buffer_p);
    }
    else {
        lcdRenderString(20, 2, "Player 1", buffer_p);
    }
}
  

#include "stm32f30x_conf.h"
#include "30010_io.h"

#include "inGameMain.h"
#include "ansi.h"
#include "joystick.h"

//#include "charset.h"
#include <string.h>
#include "lcd.h"

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
      
      uint8_t nextAction = menu(mainOption0, mainOption1, mainOption2, mainOption3);
      
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
              helpScreen();
              break;
          case 3 : 
              endScreen(startGame(level, speed));
              break;          
      }  
  }
}

void menu(char * opt0, char * opt1, char * opt2, char * opt3, uint8_t * option_p) {
  
    uint8_t pressed = 1;
    uint8_t stillDeciding = 1;
    
    renderString(opt0, 0);
    renderString(opt1, 1);
    renderString(opt2, 2);
    renderString(opt3, 3); 
      
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
            renderArrow(option);
        }
        else if (!(readJoystick() & 0x0F) {
            pressed = 0;
        }
    }
}

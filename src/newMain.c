#include "stm32f30x_conf.h"

int newMain(void)
{
  // Initilalize all shit  
  // Show start screen  
  while(1) {
      //Show main menu
      char * mainOption0 = "change level"
      char * mainOption1 = "change speed"
      char * mainOption2 = "help"
      char * mainOption3 = "PLAY"
      
      uint8_t nextAction = menu(mainOption0, mainOption1, mainOption2, mainOption3);
      switch (nextAction) {
          case 1 : 
      
  }
}

uint8_t menu(char * opt0, char * opt1, char * opt2, char * opt3) {
    uint8_t option = 0;
    uint8_t pressed = 1;
    uint8_t stillDeciding = 1;
    
    renderString(opt0, 0);
    renderString(opt1, 1);
    renderString(opt2, 2);
    renderString(opt3, 3);
      
    while(stillDeciding) {
        if (!pressed) {
            if (readJoystick() & 0x01 << 0) {
                option = (option + 1) % 4;
            }
            else if (readJoystick() & 0x01 << 1) {
                option = (option + 3) % 4;
            }
            else if (readJoystick() & 0x01 << 3) {
                stillDeciding = 0;
            }
            renderArrow(option);
        }
        else if (!(readJoystick() & 0x0F) {
            pressed = 0;
        }
    }
    return option;
}

#ifndef BLINK_H
#define BLINK_H

#include <unistd.h>

/**
  * @brief      Initialize the LED Pin
  * @retval     int
  */
int initialize_led();

/**
  * @brief      Turn on the built-in LED on the MilkV Duo
  * @retval     void 
  */
void turn_on_led();

/**
  * @brief      Turn on the built-in LED on the MilkV Duo
  * @retval     void 
  */
void turn_off_led();

#endif // BLINK_H


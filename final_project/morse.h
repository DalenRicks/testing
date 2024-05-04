#ifndef MORSE_H
#define MORSE_H

/**
  * @brief          Translates input string to morse code
  * @param[in]      *input: Input string
  * @param[out]     *output: Translated string
  * @retval         void
  */
void translate_to_morse(const char *input, char *output);

/**
  * @brief          Blinks built in MilkV Duo LED
  * @param[in]      *input: Input morse code string
  * @retval         void
  */
void morse_blink_led(const char *input);

#endif
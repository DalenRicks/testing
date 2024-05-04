#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "morse.h"
#include "blink.h"

#define MORSE_BLINK_DELAY       100000   //Time in microseconds. Cannot be less than 1

const char *char_to_morse(char c)
{
    // Define Morse code for each alphabet and digit
    static const char *morseCode[36] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
                                        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
                                        "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----", "..---",
                                        "...--", "....-", ".....", "-....", "--...", "---..", "----."};

    if (c >= 'A' && c <= 'Z')
    {
        // Convert uppercase to lowercase
        c = c - 'A' + 'a';
    }
  
    if (c >= 'a' && c <= 'z')
    {
        return morseCode[c - 'a'];
    }
    else if (c >= '0' && c <= '9')
    {
        return morseCode[c - '0' + 26];
    }
    else
    {
        return ""; // Return empty string for other characters
    }
}

// Function to convert a word to Morse code
// Use the delimiter / for any spaces between words
void translate_to_morse(const char *text, char *morseOutput)
{
    // Loop through the text to convert each
    // charactor to morse code.
    // char_to_morse function is provided above but may not
    // handle all cases.

    // Length of text array
    int len = strlen(text);

    // Initialize output array
    morseOutput[0] = '\0'; 

    for (int i = 0; i < len; i++)
    {
        // make sure its not binary and ASCII
        if (text[i] >= 32 && text[i] <= 126) 
        {
            //for space between words
            if (text[i] == ' ') 
            {
                strcat(morseOutput, "/ ");
            }

            //regular conversion between letters
            else if (i!=len-1)
            {
                const char *morseChar = char_to_morse(text[i]);
                strcat(morseOutput, morseChar);
                strcat(morseOutput, " ");
            }
            else
            {
                const char *morseChar = char_to_morse(text[i]);
                strcat(morseOutput, morseChar);
                
            }
        }

        //return empty string if its not ASCII
        else
        {
            strcat(morseOutput, "");
        }
    }
}

void morse_blink_led(const char *morseCode)
{
    // Blink LED based on the timing needed
    // 1 Unit = 1 Second

    // int duration = 1 or w/e value you need
    // Use DELAY(duration) to sleep the appropriate units
    // Rememeber 1 unit = 1 second for this assignment

    // Getting the length of the morse code
    int len = strlen(morseCode);    

    //Duration in Units (seconds)
    int duration = MORSE_BLINK_DELAY;

    //Loop Through each part of the morse code and turn
    //The LED on or off with respective delays
    for (int i = 0; i < len; i++){
        switch(morseCode[i]){
            //Dots will be 1 unit
            case '.':
                turn_on_led();
                usleep(duration);
                printf("Dot (1 Unit)\n");
                turn_off_led();

                //if it is the same letter, add the space
                if(morseCode[i+1] != ' ' && morseCode[i+1]!='\0'){
                    usleep(duration);
                    printf("Space (1 Unit)\n"); // space in the same letter
                }
                break;

            //Dashes will be 3 units
            case '-':
                turn_on_led();
                usleep(3 * duration);
                printf("Dash (3 Units)\n");
                turn_off_led();

                //if it is the right letter, add the space
                if(morseCode[i+1] != ' ' && morseCode[i+1]!='\0'){
                    usleep(duration);
                    printf("Space (1 Unit)\n"); // space in the same letter
                }
                break;

            //Space between words will be 7 units
            case '/':
                usleep(7 * duration);
                printf("Word Space (7 Units)\n"); //Word space
                break;

            //Space between letters will be 3 units
            case ' ':
                usleep(3 * duration);
                printf("Letter Space (3 Units)\n"); // Letter Space
                break;

            default:
                turn_off_led();
                break;
        }
    }
}

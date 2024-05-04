#include <stdio.h>

#include "blink.h"

#include <wiringx.h>

#define DUO_LED 25

// Fill in the three functions to
// 1.) enable the output mode for onboard LED
// 2.) set the led to high
// 3.) set the led to low

int initialize_led()
{
    // Verify the pin is available
    if(wiringXValidGPIO(DUO_LED) != 0) {
        printf("Invalid GPIO %d\n", DUO_LED);
        wiringXGC();
        return -1;
    }

    // Initialize the GPIO pin as PINMODE_OUTPUT
    pinMode(DUO_LED, PINMODE_OUTPUT);
    printf("Duo LED Intialized %d\n", DUO_LED);

    return 0;
}

void turn_on_led()
{
    // Code to turn on the LED
    digitalWrite(DUO_LED, HIGH);
}

void turn_off_led()
{
    // Code to turn off the LED
    digitalWrite(DUO_LED, LOW);
}

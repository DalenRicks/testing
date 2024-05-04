#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <wiringx.h>
#include "ssd1306.h"
#include "linux_i2c.h"
#include "bmp280_i2c.h"
#include "morse.h"
#include "blink.h"

#define FONT_SIZE                   0
#define MAX_STRING_MSG_BUF_SIZE     250     //Cannot be less than 200
#define MAX_INT_MSG_BUF_SIZE        20
#define MAX_MORSE_BUF_SIZE          MAX_STRING_MSG_BUF_SIZE * 5 
/**I multiplied it by 5 because the longest morse represenation for a 
 * single character is 5 characters long
 */

#define MORSE_ENABLE                true   //Deactivates blinking the LED if you dont have all day
                                            //You can change the blink delay in morse.c

#define START_UP_MESSAGE_ENABLE     false    //Activate if you want to test the OLED display

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (message->payloadlen)
    {
        printf("%s %s\n", message->topic, (char *)message->payload);
        cJSON *root = cJSON_Parse(message->payload);
        if (root)
        {
            const cJSON *name = cJSON_GetObjectItemCaseSensitive(root, "name");
            if (cJSON_IsString(name) && (name->valuestring != NULL))
            {
                printf("Name: %s\n", name->valuestring);

                char morseOutput[MAX_STRING_MSG_BUF_SIZE];
                
                //Translate name to morse code
                translate_to_morse(name->valuestring, morseOutput);

                //Printing translated name to the terminal
                printf("Morse encoding:\n%s\n", morseOutput);

                if(MORSE_ENABLE){
                    ///Passing the translated message to LED blinking function
                    morse_blink_led(morseOutput);
                }
            }

            const cJSON *number = cJSON_GetObjectItemCaseSensitive(root, "number");
            if (cJSON_IsNumber(number))
            {
                printf("Number: %d\n", number->valueint);

                char inputNumber[MAX_INT_MSG_BUF_SIZE];
                char morseOutput[MAX_INT_MSG_BUF_SIZE];
                
                /**Converting the int to a string because
                 * the translate_to_morse function only accepts
                 * strings as an input
                */sprintf(inputNumber, "%d", number->valueint);

                //Translate number to morse code
                translate_to_morse(inputNumber, morseOutput);

                //Printing translated number to the terminal
                printf("Morse encoding:\n%s\n", morseOutput);

                if(MORSE_ENABLE){
                    //Passing the translated number to LED blinking function
                    morse_blink_led(morseOutput);
                }
            }

            const cJSON *task = cJSON_GetObjectItemCaseSensitive(root, "task");
            if (cJSON_IsString(task) && (task->valuestring != NULL))
            {
                if(strcmp(task->valuestring, "get_temperature") == 0){
                    printf("Getting temperature\n");
                    struct bmp280_i2c result = read_temp_pressure();
                    char buffer[30];

                    //Printing current temperature to the terminal
                    printf("Current Temperature: %.2f C\n", result.temperature);

                    //Format the data into a string and save it to a buffer
                    sprintf(buffer, "Temperature: %.2f C", result.temperature);

                    //Clear the screen before printing
                    ssd1306_oled_clear_screen();

                    //Set starting coordinates to (0,0)
                    ssd1306_oled_set_XY(0, 0);

                    //Print the string to the display
                    ssd1306_oled_write_string(FONT_SIZE, buffer);

                    /*---------Translating to morse code---------*/

                    char bufT[20];
                    char morseOutput[20 * 5];   //Read MAX_MORSE_BUF_SIZE comment for 30 * 5 explanation

                    //Formatting shortened temperature message
                    sprintf(bufT, "T: %.2f C", result.temperature);

                    //Translate pressure to morse code
                    translate_to_morse(bufT, morseOutput);

                    //Translate temperature to morse code
                    translate_to_morse(bufT, morseOutput);

                    //Printing translated temperature to the terminal
                    printf("Temperature Morse encoding:\n%s\n", morseOutput);

                    if(MORSE_ENABLE){
                        //Passing the translated temperature to LED blinking function
                        morse_blink_led(morseOutput);
                    }


                }else if(strcmp(task->valuestring, "get_pressure") == 0){
                    printf("Getting pressure\n");
                    struct bmp280_i2c result = read_temp_pressure();
                    char buffer[30];
                    
                    //Printing current pressure to the terminal
                    printf("Current Pressure: %.3f kPa\n", result.pressure);

                    //Format the data into a string and save it to a buffer
                    sprintf(buffer, "Pressure: %.3f kPa", result.pressure);

                    //Clear the screen before printing
                    ssd1306_oled_clear_screen();

                    //Set starting coordinates to (0,0)
                    ssd1306_oled_set_XY(0, 0);

                    //Print the string to the display
                    ssd1306_oled_write_string(FONT_SIZE, buffer);

                    /*---------Translating to morse code---------*/

                    char bufP[20];
                    char morseOutput[20 * 5];   //Read MAX_MORSE_BUF_SIZE comment for 30 * 5 explanation

                    //Formatting shortened pressure message
                    sprintf(bufP, "P: %.3f kPa", result.pressure);

                    //Translate pressure to morse code
                    translate_to_morse(bufP, morseOutput);

                    //Printing translated pressure to the terminal
                    printf("Pressure morse encoding:\n%s\n", morseOutput);

                    if(MORSE_ENABLE){
                        //Passing the translated pressure to LED blinking function
                        morse_blink_led(morseOutput);
                    }


                }else if(strcmp(task->valuestring, "get_temperature_pressure") == 0){
                    printf("Getting temperature and pressure\n");
                    struct bmp280_i2c result = read_temp_pressure();
                    char buffer[50];

                    //Printing current temperature and pressure to the terminal
                    printf("Current Temperature: %.2f C\n", result.temperature);
                    printf("Current Pressure: %.3f kPa\n", result.pressure);

                    //Format the data into a string and save it to a buffer
                    sprintf(buffer, "Temperature: %.2f C \\nPressure:: %.3f kPa", result.temperature, result.pressure);
                    
                    //Clear the screen before printing
                    ssd1306_oled_clear_screen();

                    //Set starting coordinates to (0,0)
                    ssd1306_oled_set_XY(0, 0);

                    //Print the string to the display
                    ssd1306_oled_write_string(FONT_SIZE, buffer);

                    /*---------Translating to morse code---------*/
                    
                    char bufT[20];
                    char bufP[20];
                    char morseOutput[20 * 5];   //Read MAX_MORSE_BUF_SIZE comment for 30 * 5 explanation

                    //Formatting shortened temperature and pressure messages
                    sprintf(bufT, "T: %.2f C", result.temperature);
                    sprintf(bufP, "P: %.3f kPa", result.pressure);

                    //Translate pressure to morse code
                    translate_to_morse(bufT, morseOutput);

                    //Printing translated temperature to the terminal
                    printf("Temperature morse encoding:\n%s\n", morseOutput);

                    //Translate pressure to morse code
                    translate_to_morse(bufP, morseOutput);

                    //Printing translated pressure to the terminal
                    printf("Pressure morse encoding:\n%s\n", morseOutput);

                    /**Since I only use one buffer the temperature and pressure
                     * will need to be re-translated prior to blinking the LED
                    */
                    if(MORSE_ENABLE){
                        //Translate temperature to morse code
                        translate_to_morse(bufT, morseOutput);

                        //Passing the translated temperature to LED blinking function
                        morse_blink_led(morseOutput);

                        //Translate pressure to morse code
                        translate_to_morse(bufP, morseOutput);

                        //Passing the translated pressure to LED blinking function
                        morse_blink_led(morseOutput);
                    }

                }else{
                    printf("Unknown Command\n");

                    char oopsies[30];

                    //Format message into a buffer
                    sprintf(oopsies, "Unknown command");

                    //Clear the screen before printing
                    ssd1306_oled_clear_screen();

                    //Set starting coordinates to (0,0)
                    ssd1306_oled_set_XY(0, 0);

                    //Print the string to the display
                    ssd1306_oled_write_string(FONT_SIZE, oopsies);

                    //You messed up im not translating it to morse
                }
            }

            const cJSON *string_msg = cJSON_GetObjectItemCaseSensitive(root, "string_msg");
            if (cJSON_IsString(string_msg) && (string_msg->valuestring != NULL))
            {
                //Printing the message to the terminal
                printf("Message: %s\n", string_msg->valuestring);

                char buffer[MAX_STRING_MSG_BUF_SIZE];
                char morseOutput[MAX_MORSE_BUF_SIZE];
                
                //Format the input string into a buffer
                sprintf(buffer, "%s", string_msg->valuestring);

                //Clear the screen before printing
                ssd1306_oled_clear_screen();

                //Set starting coordinates to (0,0)
                ssd1306_oled_set_XY(0, 0);

                //Print the string to the display
                ssd1306_oled_write_string(FONT_SIZE, buffer);

                //Translate string to morse code
                translate_to_morse(buffer, morseOutput);

                //Printing translated string to the terminal
                printf("String morse encoding:\n%s\n", morseOutput);

                if(MORSE_ENABLE){
                    //Passing the translated string to LED blinking function
                    morse_blink_led(morseOutput);
                }
            }

            const cJSON *int_msg = cJSON_GetObjectItemCaseSensitive(root, "int_msg");
            if (cJSON_IsNumber(int_msg))
            {
                //Printing the message to the terminal
                printf("Number : %d\n", int_msg->valueint);

                char buffer[MAX_INT_MSG_BUF_SIZE];
                char morseOutput[MAX_MORSE_BUF_SIZE];

                //Format data into a string and save it to a buffer
                sprintf(buffer, "%d", int_msg->valueint);

                //Clear the screen before printing
                ssd1306_oled_clear_screen();

                //Set starting coordinates to (0,0)
                ssd1306_oled_set_XY(0, 0);

                //Print the string to the display
                ssd1306_oled_write_string(FONT_SIZE, buffer);

                //Translate number to morse code
                translate_to_morse(buffer, morseOutput);

                //Printing translated number to the terminal
                printf("Number morse encoding:\n%s\n", morseOutput);

                if(MORSE_ENABLE){
                    //Passing the translated number to LED blinking function
                    morse_blink_led(morseOutput);
                }
            }

            cJSON_Delete(root);
        }
        else
        {
            printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        }
    }
    else
    {
        printf("%s (null)\n", message->topic);
    }
}

void system_init(){
    /*Initializing WiringX*/
    if(wiringXSetup("duo", NULL) == -1) {
    wiringXGC();
    }

    /*Initializing the LED*/
    if(initialize_led() == -1){
        printf("LED initialization failed.\n");
    }

    /*Initializing the sensor*/
    bmp280_i2c_init();
    
    /*Settting the i2c channel for the OLED display*/
    uint8_t i2c_node_address = 0;

    /*Initializing the OLED Display*/
    ssd1306_system_init(i2c_node_address);

    /*Testing the system*/
    if(START_UP_MESSAGE_ENABLE){
        printf("Testing BMP280 Sensor and OLED Display printing.\n");
        struct bmp280_i2c result = read_temp_pressure();
        char buffer[60];

        //Printing current temperature and pressure to the terminal
        printf("Current Temperature: %.2f C\n", result.temperature);
        printf("Current Pressure: %.3f kPa\n", result.pressure);

        //Format the data into a string and save it to a buffer
        sprintf(buffer, "Temperature: %.2f C \\nPressure: %.3f kPa", result.temperature, result.pressure);
        
        //Clear the screen before printing
        ssd1306_oled_clear_screen();

        /*Turning display on*/
        ssd1306_oled_onoff(1);

        //Set starting coordinates to (0,0)
        ssd1306_oled_set_XY(0, 0);

        //Print the string to the display
        ssd1306_oled_write_string(FONT_SIZE, buffer);

        //Sleep for 3 seconds so you can see the screen
        sleep(3);
    }
}

int main(int argc, char *argv[])
{
    struct mosquitto *mosq;

    // Initialize the Mosquitto library
    mosquitto_lib_init();

    //Initialize sensor and OLED
    system_init();

    // Create a new Mosquitto runtime instance with a random client ID
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        fprintf(stderr, "Could not create Mosquitto instance\n");
        exit(-1);
    }

    // Assign the message callback
    mosquitto_message_callback_set(mosq, message_callback);

    // Connect to an MQTT broker
    if (mosquitto_connect(mosq, "104.236.198.67", 1883, 60) != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Could not connect to broker\n");
        exit(-1);
    }

    // Subscribe to the topic
    mosquitto_subscribe(mosq, NULL, "cpe4953/spring2024/group1", 0);

    // Start the loop
    mosquitto_loop_start(mosq);

    printf("Press Enter to quit...\n");
    getchar();

    // Cleanup
    mosquitto_loop_stop(mosq, true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}

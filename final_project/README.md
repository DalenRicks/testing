# Final Project

## Introduction

This project will utilize the mqtt protocol to receive messages from the class server and perform operations on the MilkV Duo using the connected sensors and OLED display. The MilkV Duo also has a morse code translator program to translate the incoming messages and blink the on-board LED for extra credit.

## Instructions

Use the following commands to use configure the CMake file, cross compile the code and upload it to the MilkV Duo. The confuguration and cross compile image have been provided by the professor of the course, Professo Ortega.

### Configure CMake

```bash
docker run --rm -v $PWD:/app -v $PWD/install_cv1800b_openssl:/app/install_cv1800b_openssl -v $PWD/install_cv1800b_cjson:/app/install_cv1800b_cjson -v $PWD/install_cv1800b_mqtt:/app/install_cv1800b_mqtt ejortega/duo-sdk bash -c "mkdir build && cd build && cmake -DCMAKE_TOOLCHAIN_FILE=/app/milkv_duo.cmake .."
```

### Compile

```bash
docker run --rm -v $PWD:/app -v $PWD/install_cv1800b_openssl:/app/install_cv1800b_openssl -v $PWD/install_cv1800b_cjson:/app/install_cv1800b_cjson -v $PWD/install_cv1800b_mqtt:/app/install_cv1800b_mqtt ejortega/duo-sdk bash -c "cd build && make"
```

### Upload the subscriber program to the Duo

```bash
scp build/subscriber root@192.168.42.1:/root
```

### Upload the publisher program to the Duo

```bash
scp build/publisher root@192.168.42.1:/root
```

## Customizing the code

There are defined functions in `subscriber.c` that you can modify prior to deploying the code.

### `FONT_SIZE`

Font size will change the size of the letters printed on the OLED display. The default value is ***0***.

### `MAX_STRING_MSG_BUF_SIZE`

This will set the maximum size of buffers that contain strings. The maximum size of all possible incoming strings is ***200***, so this value should not be set lower than that.

### `MAX_INT_MSG_BUF_SIZE`

This will set the maximum size of buffers that contain integers. The integers must be converted to strings prior to printing on the OLED display. Since the the maximum integer value is unknown, the maximum string length is hard to determine. Feel free to play around and find smaller values that work. The default value is ***20***.

### `MAX_MORSE_BUF_SIZE`

This will set the maximum size of buffers that contain translated morse code messages. Since the maximum morse representation of a single character is 5 morse characters, the default value is ***5 times MAX_STRING_MSG_BUF_SIZE***.

### `MORSE_ENABLE`

This will enable the function that blinks the on-board led on the MilkV Duo. Since the fastest the LED can blink is a 2 second period, so long messages can take a while to blink. You have the option to diable the blinking to save time. Regardless of the state of `MORSE_ENABLE` the translated morse messages will be printed to the terminal.

### `START_UP_MESSAGE_ENABLE`

This is a boolean macro that will test the OLED and BMP280 i2c communication by collecting information from the sensor and printing it to the display on start up. This feature is promarially for debugging, so once set up has comcluded it can be disabled. The default falue is ***false***.

## Challenges

The first challenge was getting the display to properly show the incoming messages. The first version of the code used an example i2c library from MilkV but did not support wrapping text. Addirionally, the drivers only supported two sizes of text, both of which were too large for the text to display properly. This problem was solved by swapping out the driver for a new driver that accessed the MilkV i2c bus differently, but supported wrapping and multi-size font.

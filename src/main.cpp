/*
The main loop here is structured to use feedback control. In each pass of the loop:
    1. Values from the IR sensors are recorded and stored in a variable.
    2. The sensor data is used to determine the direction the robot should proceed in.
    3. The robot turns in the appropriate direction.
*/

#include <Arduino.h>
#include <DDBot.h>

uint8_t directionPins[] = {
    4,  // Left motor forward
    5,  // Left motor backward
    2,  // Right motor forward
    3   // Right motor backward
};

uint8_t sensorPins[] = {
    7,  // Left sensor
    8,  // Middle sensor
    9   // Right sensor
};

DDBot bot(directionPins);
byte sensorValues;

void setup() {
    bot.setPinModes();
}

void loop() {
    
    // Write values from the sensors to a variable. The goal is to have MSBs represent left
       // sensors and LSBs represent right sensors. This action could have been a loop but that
       // results in significantly longer assembly code.
    sensorValues = 0;
    sensorValues = digitalRead(sensorPins[0]); // Write the value for the first sensor.
    sensorValues = sensorValues << 1; // Make room for the next sensor at the LSB.
    sensorValues = sensorValues + digitalRead(sensorPins[1]); // Write the value for the next sensor
                                                              // to the LSB.
    sensorValues = sensorValues << 1; // ...and repeat.
    sensorValues = sensorValues + digitalRead(sensorPins[2]);

    // Set the direction for the robot based on the sensor values. These value-direction pairs have
       // been determined experimentally. Since this is the crux of building a line follower, the
       // explanation is left as an exercise for the reader.
    switch (sensorValues) {
        case 0: // 000
            bot.left();
            break;

        case 1: // 001
            bot.right();
            break;

        case 2: // 010
            bot.forward();
            break;

        case 3: // 011
            bot.left();
            break;

        case 4: // 100
            bot.right();
            break;

        case 5: // 101
            bot.left();
            break;

        case 6: // 110
            bot.right();
            break;

        case 7: // 111
            bot.left();
            break;
        
        default:
            bot.forward();
            break;
    }

    // Wait between loop iterations to allow the sensors to refresh and the motors to smoothly
       // change or maintain their direction.
    delay(10);
}
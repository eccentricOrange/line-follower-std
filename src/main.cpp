/*
The main loop here is structured to use feedback control. In each pass of the loop:
    1. Values from the IR sensors are recorded and stored in a variable.
    2. The sensor data is used to determine the direction the robot should proceed in.
    3. The speed of each motor is adjusted to turn the robot closer to the desired direction.

Conventionally, the direction of a **differential wheeled robot** is set by turning motors on
or off in a specific direction. In this setup, they are set to move forward perpetually, and
direction control is achieved by varying the speed of each motor instead. This allows for
smoother turns and will (hopefully) help the robot to stay on its line more reliably.
*/

#include <Arduino.h>

static const uint8_t motorPins[] = {
    4,  // Left motor forward
    5,  // Left motor backward
    2,  // Right motor forward
    3,  // Right motor backward
    10, // Left motor speed
    11  // Right motor speed
};
static const uint8_t sensorPins[] = {
    7,  // Left sensor
    8,  // Middle sensor
    9   // Right sensor
};

static const uint8_t maxPwm = 255; // Maximum "speed" for each motor
static const float adjustment = 0.25; // Scaling factor used when slowing down a given motor.
static const float adjustedPwm = maxPwm * adjustment; // Scaled down PWM "speed" for motors.

// Values used for the closed-loop feedback controlling the motors.
uint8_t leftActualPwm = 255,
        leftTargetPwm = 255,
        rightActualPwm = 255,
        rightTargetPwm = 255;

byte sensorValues;

// Differential drive definitions for each direction.
inline void left() {
    leftTargetPwm = adjustedPwm;
    rightTargetPwm = maxPwm;
}

inline void centre() {
    leftTargetPwm = maxPwm;
    rightTargetPwm = maxPwm;
}

inline void right() {
    leftTargetPwm = maxPwm;
    rightTargetPwm = adjustedPwm;
}

void setup() {

    // Initialise all motor pins as outputs
    for (int pinIndex = 0; pinIndex < 6; pinIndex++) {
        pinMode(motorPins[pinIndex], OUTPUT);
    }

    // Initialise all sensor pins as inputs
    for (int pinIndex = 0; pinIndex < 3; pinIndex++)
    {
        pinMode(sensorPins[pinIndex], INPUT);
    }
    
    // Write 1010 to the motors to continuously go forward
    digitalWrite(motorPins[0], HIGH);
    digitalWrite(motorPins[1], LOW);
    digitalWrite(motorPins[2], HIGH);
    digitalWrite(motorPins[3], LOW);

    // Write to the PWM values to make sure the motors actually move
    analogWrite(motorPins[4], leftActualPwm);
    analogWrite(motorPins[5], rightActualPwm);
}

void loop() {
    
    // Write values from the sensors to a variable. The goal is to have MSBs represent left
       // sensors and LSBs represent right sensors. This action could have been a loop but that
       // results in significantly longer assembly code.
    sensorValues = 0;
    sensorValues = digitalRead(sensorPins[0]); // Write the value for the first sensor.
    sensorValues = sensorValues << 1; // Make room for the next sensor at the LSB.
    sensorValues = sensorValues + digitalRead(sensorPins[1]); // Write the value for the next sensor to the LSB.
    sensorValues = sensorValues << 1; // ...and repeat.
    sensorValues = sensorValues + digitalRead(sensorPins[2]);

    // Set the direction for the robot based on the sensor values. These value-direction pairs have been
       // determined experimentally. Since this is the crux of building a line follower, the explanation
       // is left as an exercise for the reader.
    switch (sensorValues) {
        case 0: // 000
            centre();
            break;

        case 1: // 001
            right();
            break;

        case 2: // 010
            centre();
            break;

        case 3: // 011
            left();
            break;

        case 4: // 100
            right();
            break;

        case 5: // 101
            centre();
            break;

        case 6: // 110
            right();
            break;

        case 7: // 111
            centre();
            break;
        
        default:
            centre();
            break;
    }

    // Update "actual" PWM values so that they get closer to the "target" values. This will
       // eventually get the values to equal, as exponential decay (but with integer division).
    leftActualPwm = (leftTargetPwm + leftActualPwm) / 2;
    rightActualPwm = (rightTargetPwm + rightActualPwm) / 2;

    // Write the actual PWM values to the motor driver
    analogWrite(motorPins[4], leftActualPwm);
    analogWrite(motorPins[5], rightActualPwm);

    // Wait between loop iterations to allow the sensors to refresh and the motors to smoothly
       // change or maintain their direction.
    delay(10);
}
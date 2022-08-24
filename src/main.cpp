#include <Arduino.h>
#include <Locomotion.h>

static const uint8_t pins[] = {6, 7, 4, 5, 10, 11};
static const uint8_t maxPwm = 255;
static const float adjustment = 0.25;

uint8_t leftActualPwm, leftTargetPwm, rightActualPwm, rightTargetPwm;

inline void driveMotors(bool left_forward, bool left_backward, bool right_forward, bool right_backward) {
    digitalWrite(pins[0], left_forward);
    digitalWrite(pins[1], left_backward);
    digitalWrite(pins[2], right_forward);
    digitalWrite(pins[3], right_backward);
}

inline void left() {
    leftTargetPwm = maxPwm * adjustment;
    rightTargetPwm = maxPwm;
}

inline void right() {
    leftTargetPwm = maxPwm;
    rightTargetPwm = maxPwm * adjustment;
}

inline void adjustPwmToTarget() {
    leftActualPwm = (leftTargetPwm + leftActualPwm) / 2;
    rightActualPwm = (rightTargetPwm + rightActualPwm) / 2;
    analogWrite(pins[4], leftActualPwm);
    analogWrite(pins[5], rightActualPwm);
}

void setup() {
    Serial.begin(9600);

    for (int pinIndex = 0; pinIndex < 6; pinIndex++) {
        pinMode(pins[pinIndex], OUTPUT);
    }

    leftActualPwm = 255;
    rightActualPwm = 255;
    leftTargetPwm = 255;
    rightTargetPwm = 255;

    driveMotors(true, false, true, false);
    adjustPwmToTarget();
    left();
    delay(2000);
}

void loop() {
    Serial.println(leftActualPwm);
    delay(100);
    adjustPwmToTarget();
}
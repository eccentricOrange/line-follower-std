#include "Locomotion.h"
#include "Arduino.h"

Locomotion::Locomotion(const uint8_t *pins, const float* adjustment) {
    _pins = pins;
    _adjustment = adjustment;
}

void Locomotion::init() {
    for (int i = 0; i < 4; i++) {
        pinMode(_pins[i], OUTPUT);
    }

    leftActualPwm = 255;
    rightActualPwm = 255;
    leftTargetPwm = 255;
    rightTargetPwm = 255;
}

inline void Locomotion::driveMotors(bool left_forward, bool left_backward, bool right_forward, bool right_backward) {
    digitalWrite(_pins[0], left_forward);
    digitalWrite(_pins[1], left_backward);
    digitalWrite(_pins[2], right_forward);
    digitalWrite(_pins[3], right_backward);
}

void Locomotion::forward() {
    driveMotors(true, false, true, false);
}

void Locomotion::stop() {
    driveMotors(false, false, false, false);
}

void Locomotion::setPwm(uint8_t left_pwm, uint8_t right_pwm) {
    leftTargetPwm = left_pwm;
    rightTargetPwm = right_pwm;
}

void Locomotion::adjustPwmToTarget() {
    leftActualPwm = (leftTargetPwm + leftActualPwm) / 2;
    rightActualPwm = (rightTargetPwm + rightActualPwm) / 2;
    analogWrite(_pins[4], leftActualPwm);
    analogWrite(_pins[5], rightActualPwm);
}

void Locomotion::left() {
    setPwm(255 * *_adjustment, 255);
}

void Locomotion::right() {
    setPwm(255, 255 * *_adjustment);
}
#include <Arduino.h>
#include <Locomotion.h>

static const uint8_t pins[] = {6, 7, 4, 5, 10, 11};
static const float adjustment = 0.25;

Locomotion motors(pins, &adjustment);

void setup() {
    motors.init();
    Serial.begin(9600);
    motors.forward();
    motors.adjustPwmToTarget();
    motors.left();
    delay(2000);
}

void loop() {
    Serial.println(motors.leftActualPwm);
    delay(100);
    motors.adjustPwmToTarget();
}
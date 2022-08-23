#include "Arduino.h"

class Locomotion {
    public:
        Locomotion(const uint8_t* pins, const float* adjustment);

        void init();

        void forward();
        void stop();

        void setPwm(uint8_t left_pwm, uint8_t right_pwm);
        void adjustPwmToTarget();

        void left();
        void right();

        uint8_t leftTargetPwm;
        uint8_t rightTargetPwm;
        uint8_t leftActualPwm;
        uint8_t rightActualPwm;

    private:
        const uint8_t* _pins;
        const float* _adjustment;

        inline void driveMotors(bool left_forward, bool left_backward, bool right_forward, bool right_backward);
};

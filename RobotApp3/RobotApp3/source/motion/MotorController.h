#pragma once
#include <stdint.h>

enum RotateDirection
{
    DIRECTION_STOP = 0,
    DIRECTION_FORWARD,
    DIRECTION_REVERSE
};

class MotorController
{
public:
    MotorController(const uint8_t in1, const uint8_t in2, const uint8_t pwm);
    ~MotorController() {}
    void run(const RotateDirection direction, int power) const;
    void setPower(const int power) const;                 // 0 ... 1024
    void stop() const;
private:
    const uint8_t pin1;
    const uint8_t pin2;
    const uint8_t pin_pwm;
};


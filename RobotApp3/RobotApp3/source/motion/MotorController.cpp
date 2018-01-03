#include <stdio.h>
#include "MotorController.h"
#include <wiringPi.h>


MotorController::MotorController(const uint8_t in1, const uint8_t in2,
             const uint8_t pwm) : pin1(in1), pin2(in2), pin_pwm(pwm)
{
    //printf("Motor init\n");
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin_pwm, PWM_OUTPUT);
    stop();
}


void MotorController::run(RotateDirection const direction, int power) const
{
    printf("Motor run %d %d\n", static_cast<int>(direction), power);
    if (direction == DIRECTION_FORWARD)
    {
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
    }
    else if (direction == DIRECTION_REVERSE)
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
    }
    else if (direction == DIRECTION_STOP)
    {
        stop();
        return;
    }

    if (power > 1024)
    {
        power = 1024;
    }

    pwmWrite(pin_pwm, power);
}


void MotorController::setPower(const int power) const
{
    pwmWrite(pin_pwm, power);
}


void MotorController::stop() const
{
    pwmWrite(pin_pwm, 0);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
}

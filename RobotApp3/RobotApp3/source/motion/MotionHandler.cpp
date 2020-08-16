#include "MotionHandler.h"
#include "MotionHandler.h"
#include "configuration.h"
#include <thread>


using namespace std;

void motionTask();

MotionHandler::MotionHandler() : left_motor(LEFT_MOTOR_1_PIN, LEFT_MOTOR_2_PIN, LEFT_MOTOR_PWM_PIN),
right_motor(RIGHT_MOTOR_1_PIN, RIGHT_MOTOR_2_PIN, RIGHT_MOTOR_PWM_PIN),
current_movement_duration(-1),
current_power(400),
motion_thread(motionTask) {
    sem_init(&queue_semaphore, 0, 0);
}


void MotionHandler::addCurve(Curve c) {
    queue_mutex.lock();
    route.push(c);
    sem_post(&queue_semaphore);
    queue_mutex.unlock();
}

Curve MotionHandler::getNextCurve() {
    sem_wait(&queue_semaphore);

    queue_mutex.lock();
    Curve first = route.front();
    route.pop();
    queue_mutex.unlock();
    return first;
}


MotorController* MotionHandler::getMotor(int idx) {
    if (idx == LEFT_MOTOR_IDX) {
        return &left_motor;
    }
    if (idx == RIGHT_MOTOR_IDX) {
        return &right_motor;
    }
    return nullptr;
}


void MotionHandler::process() {
    if (route.empty()) {
        right_motor.stop();
        left_motor.stop();
    }

    Curve curr_curve = getNextCurve();
    setMovement(curr_curve.radius, current_power);
    this_thread::sleep_for(chrono::milliseconds(curr_curve.duration));
}


// if radius > 0 -- turning right,
// if radius < 0 -- turning left
void MotionHandler::setMovement(int radius, int power) {
    RotateDirection left_direction = DIRECTION_FORWARD, right_direction = DIRECTION_FORWARD;
    int left_power = power, right_power = power;

    if (radius != INFINITE_RADIUS) { // straight
        if (radius < 0) { // turning left
            left_power = right_power * (radius + HALF_ROBOT_WIDTH) / (radius - HALF_ROBOT_WIDTH);
        }
        else { // turning right
            right_power = left_power * (radius - HALF_ROBOT_WIDTH) / (radius + HALF_ROBOT_WIDTH);
        }
    }

    if (left_power < 0) {
        left_power = -left_power;
        left_direction = DIRECTION_REVERSE;
    }
    if (right_power < 0) {
        right_power = -right_power;
        right_direction = DIRECTION_REVERSE;
    }

    // calibrate correction
    right_power = static_cast<int>((1 - STRAIGHT_CALIBRATE_COEFFICIENT *
        static_cast<double>(right_power) * static_cast<double>(right_power) / 
        1024.0 / 1024.0) * static_cast<double>(right_power) + 0.5f);

    right_motor.run(right_direction, right_power);
    left_motor.run(left_direction, left_power);
}


void motionTask() {
    while (true) {
        motionHandler->process();
    }
}

#pragma once
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <semaphore.h>

#include "MotorController.h"
#include "timer.h"


// 1 unit - 1 mm
#define INFINITE_RADIUS    0x7FFFFFFF
#define MAX_SPEED          730    // mm/s
#define STRAIGHT_CALIBRATE_COEFFICIENT         0.08f
#define HALF_ROBOT_WIDTH                       100  // mm

#define LEFT_MOTOR_IDX          1
#define RIGHT_MOTOR_IDX         2

struct Curve {
    int duration;
    int radius;
};


class MotionHandler {
public:
    MotionHandler();

    void setWorkingPower(int value) { current_power = value; }
    void addCurve(Curve c);
    void move(int radius, int power, uint32_t time_out_ms);
    MotorController* getMotor(int idx);

    void process();
private:
    Curve getNextCurve();
    void setMovement(int radius, int power);

    MotorController left_motor;
    MotorController right_motor;
    std::queue<Curve> route;
    Curve *last_curve = nullptr;

    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> start_movement_time;
    int current_movement_duration;
    int current_power;

    sem_t queue_semaphore;
    std::mutex queue_mutex;
    std::thread motion_thread;
    Timer motion_timer;
};


extern MotionHandler* motionHandler;

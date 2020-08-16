#pragma once

#include <ctime>
#include <functional>

// time definitions
#define HOURS_IN_DAY                    24
#define MINUTES_IN_HOUR                 60
#define SECONDS_IN_MINUTE               60
#define SECONDS_IN_HOUR                 (SECONDS_IN_MINUTE * MINUTES_IN_HOUR)
#define MINUTES_IN_DAY                  (HOURS_IN_DAY * MINUTES_IN_HOUR)
#define SECONDS_IN_DAY                  (MINUTES_IN_DAY * SECONDS_IN_MINUTE)
#define WEEK_DAYS_NUM                   7

#define NANOSECONDS_IN_SECOND_NUM       1000000000     // billion
#define NANOSECONDS_IN_MILLISECOND_NUM  1000000        // million
#define MILLISECONDS_IN_SECOND_NUM      1000           // thousand


/**
 * @class Timer system_utils.h
 * @brief It is a wrapper around Linux system timer functions.
 */
class Timer
{
public:
   Timer();
   /**
    * @param callback Function to be called after timer expiration.
    */
   Timer(std::function<void()> callback);
   Timer(const Timer& other) = delete;
   Timer(Timer&& other) = delete;
   ~Timer();

   void SetCallback(std::function<void()> callback) {
      callback_ = std::move(callback);
   }
   /**
    * @brief Arms the timer.
    * @param initial_expiration_ms Amount of time in milliseconds until the first timer expiration.
    * @param period_ms If not 0, timer will be automatically restarted with given period.
    * @return true on success, else false.
    * @note If the timer was already armed, then the previous settings are overwritten.
    */
   bool Start(uint32_t initial_expiration_ms, uint32_t period_ms = 0);
   /**
    * @brief Stops timer.
    * @return true on success, else false.
    */
   bool Stop();

private:
   std::function<void()> callback_;
   timer_t timer_id_;
};

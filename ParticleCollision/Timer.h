#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

class Timer
{
  using Clock = std::chrono::high_resolution_clock;
  using miliseconds = std::chrono::duration<float, std::milli>;
  using seconds = std::chrono::duration<float>;

 public:
  Timer();
  ~Timer();

  void Reset();

  float Seconds() const;
  float Miliseconds() const;

 private:
  Clock::time_point m_start;
};

#endif //_TIMER_H_
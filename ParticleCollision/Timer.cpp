#include "Timer.h"

Timer::Timer()
{
  Reset();
}

Timer::~Timer()
{ }

void Timer::Reset()
{
  m_start = Clock::now();
}

float Timer::Seconds() const
{
  return std::chrono::duration_cast<seconds>(Clock::now() - m_start).count();
}

float Timer::Miliseconds() const
{
  return std::chrono::duration_cast<miliseconds>(Clock::now() - m_start).count();
}
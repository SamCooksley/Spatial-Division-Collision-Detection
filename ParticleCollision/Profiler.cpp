#include "Profiler.h"

#include <sstream>
#include <limits>
#include <iomanip>

Profiler::Profiler(std::shared_ptr<Font> _font)
{
  Reset();
  m_fpsText.SetFont(_font);
  m_minText.SetFont(_font);
  m_maxText.SetFont(_font);

  m_fpsText.SetText("FPS: 0.0 (0.0000ms)");
}

Profiler::~Profiler()
{ }

void Profiler::Update(float _deltaTime)
{
  float fps = _deltaTime == .0f ? .0f : 1.f / _deltaTime;

  m_min = std::fmin(fps, m_min);
  m_max = std::fmax(fps, m_max);

  std::stringstream ss;

  if (m_updateTimer.Seconds() > .1f)
  {
    m_updateTimer.Reset();

    ss << "FPS: " << std::fixed << std::setprecision(1) << fps << " (" << std::setprecision(4) << _deltaTime << "ms)";
    m_fpsText.SetText(ss.str());

    ss.str(std::string());
  }

  ss << "Min: " << std::fixed << std::setprecision(1) << m_min;
  m_minText.SetText(ss.str());

  ss.str(std::string());

  ss << "Max: " << std::fixed << std::setprecision(1) << m_max;
  m_maxText.SetText(ss.str());
}

void Profiler::Render(Renderer& _renderer)
{
  m_fpsText.Draw(_renderer, 10, 10);
  m_minText.Draw(_renderer, 10, 40);
  m_maxText.Draw(_renderer, 10, 70);
}

void Profiler::Reset()
{
  m_min = std::numeric_limits<float>().max();
  m_max = std::numeric_limits<float>().min();
}
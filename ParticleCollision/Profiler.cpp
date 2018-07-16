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
  m_avgText.SetFont(_font);

  m_fpsText.SetText("FPS: 0.0 (0.0000ms)");
  m_minText.SetText("Min: 0.0");
  m_maxText.SetText("Max: 0.0");
  m_avgText.SetText("Avg: 0.0");
}

Profiler::~Profiler()
{ }

void Profiler::Update(float _deltaTime)
{
  float fps = _deltaTime == .0f ? .0f : 1.f / _deltaTime;

  // allow 100 frames for the frame rate to settle.
  if (m_frames > 100u)
  {
    m_min = std::fmin(fps, m_min);
    m_max = std::fmax(fps, m_max);
  }
  else if (m_frames == 100u)
  {
    m_min = fps;
    m_max = fps;
  }
  
  m_average = (m_frames == 0u) ? fps : (m_average + fps) * .5f;

  std::stringstream ss;

  if (m_updateTimer.Seconds() > .1f)
  {
    m_updateTimer.Reset();

    ss << "FPS: " << std::fixed << std::setprecision(1) << fps << " (" << std::setprecision(4) << _deltaTime << "ms)";
    m_fpsText.SetText(ss.str());

    ss.str(std::string());

    ss << "Avg: " << std::fixed << std::setprecision(1) << m_average;
    m_avgText.SetText(ss.str());

    ss.str(std::string());
  }

  ss << "Min: " << std::fixed << std::setprecision(1) << m_min;
  m_minText.SetText(ss.str());

  ss.str(std::string());

  ss << "Max: " << std::fixed << std::setprecision(1) << m_max;
  m_maxText.SetText(ss.str());

  ++m_frames;
}

void Profiler::Render(Renderer& _renderer)
{
  m_fpsText.Draw(_renderer, 10, 10);
  m_minText.Draw(_renderer, 10, 40);
  m_maxText.Draw(_renderer, 10, 70);
  m_avgText.Draw(_renderer, 300, 10);
}

void Profiler::Reset()
{
  m_min = 0.f;//std::numeric_limits<float>().max();
  m_max = 0.f;//std::numeric_limits<float>().min();
  m_average = 0.f;
  m_frames = 0u;
}
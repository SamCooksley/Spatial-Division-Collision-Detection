#ifndef _PROFILER_H_
#define _PROFILER_H_

#include "Text.h"
#include "Timer.h"

/**
 * \brief Store time statisics.
 * Displays current fps, minimum fps and maximum fps.
 */
class Profiler
{
 public:
  Profiler(std::shared_ptr<Font> _font); //!< Constructor.
  ~Profiler(); //!< Destructor.

  void Update(float _deltaTime); //!< Update content of the text.

  void Render(Renderer& _renderer); //!< Render the text. 

  void Reset(); //!< Reset the minimum and maximum.

 private:
  float m_min; //!< Minimum fps.
  float m_max; //!< Maximum fps.

  float m_average; //!< Average fps.

  Text m_fpsText; //!< Current fps text to display.
  Text m_minText; //!< Minimum fps text to display.
  Text m_maxText; //!< Maximum fps text to display.
  Text m_avgText; //!< Average fps text to display.

  Timer m_updateTimer; //!< Timer to update.
  unsigned int m_frames; //!< number of frames since last reset.
};

#endif //_PROFILER_H_
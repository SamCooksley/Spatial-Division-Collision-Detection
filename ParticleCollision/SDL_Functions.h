#ifndef _SDLFUNCTIONS_H_
#define _SDLFUNCTIONS_H_

#include "Renderer.h"

namespace SDL
{
  /**
   * \brief Draw a circle outline.
   * \param [in, out] _renderer Renderer to draw to.
   * \param [in]      _x        Centre x position.
   * \param [in]      _y        Centre y position.
   * \param [in]      _radius   Size.
   */
  void DrawCircle(Renderer& _renderer, int _x, int _y, int _radius);

  /**
   * \brief Set a single pixel of a renderer.
   * \param [in, out] _renderer Renderer to draw to.
   * \param [in]      _x 
   * \param [in]      _y
   */
  void DrawPoint(Renderer& _renderer, int _x, int _y);
}

#endif //_SDLFUNCTIONS_H_
#pragma once

#include "Renderer.h"

namespace SDL
{
  void DrawCircle(Renderer &_renderer, int _x, int _y, int _radius);

  void DrawPoint(Renderer &_renderer, int _x, int _y);
}
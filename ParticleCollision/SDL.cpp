#include "SDL.h"

namespace SDL
{
  void DrawCircle(Renderer &_renderer, int _x, int _y, int _radius)
  {
    int x = _radius;
    int y = 0;
    int err = 0;

    while (x >= y)
    {
        DrawPoint(_renderer, _x + x, _y + y);
        DrawPoint(_renderer, _x + y, _y + x);
        DrawPoint(_renderer, _x - y, _y + x);
        DrawPoint(_renderer, _x - x, _y + y);
        DrawPoint(_renderer, _x - x, _y - y);
        DrawPoint(_renderer, _x - y, _y - x);
        DrawPoint(_renderer, _x + y, _y - x);
        DrawPoint(_renderer, _x + x, _y - y);

        y += 1;
        if (err <= 0)
        {
            err += 2*y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2*x + 1;
        }
    }
  }

  void DrawPoint(Renderer &_renderer, int _x, int _y)
  {
    SDL_RenderDrawPoint(_renderer.Get(), _x, _y);
  }
}
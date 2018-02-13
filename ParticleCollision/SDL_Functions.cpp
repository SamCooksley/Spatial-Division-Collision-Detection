#include "SDL_Functions.h"

#include <vector>

namespace SDL
{
  void DrawCircle(Renderer& _renderer, int _x, int _y, int _radius)
  {
    int x = _radius - 1, y = 0;
    int dx = 1, dy = 1;
    int err = dx - (_radius << 1);

    std::vector<SDL_Point> points;

    while (x >= y)
    {
      points.push_back({ _x + x, _y + y });
      points.push_back({ _x + y, _y + x });
      points.push_back({ _x - y, _y + x });
      points.push_back({ _x - x, _y + y });
      points.push_back({ _x - x, _y - y });
      points.push_back({ _x - y, _y - x });
      points.push_back({ _x + y, _y - x });
      points.push_back({ _x + x, _y - y });

      if (err <= 0)
      {
        ++y;
        err += dy;
        dy += 2;
      }
      if (err > 0)
      {
        --x;
        dx += 2;
        err += dx - (_radius << 1);
      }
    }

    SDL_RenderDrawPoints(_renderer.Get(), &points[0], points.size());

  }

  void DrawPoint(Renderer& _renderer, int _x, int _y)
  {
    SDL_RenderDrawPoint(_renderer.Get(), _x, _y);
  }
}
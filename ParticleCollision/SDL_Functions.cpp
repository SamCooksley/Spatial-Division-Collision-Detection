#include "SDL_Functions.h"

namespace SDL
{
  void DrawCircle(Renderer& _renderer, int _x, int _y, int _radius)
  {
    int x = _radius - 1, y = 0;
	int dx = 1, dy = 1;
    int err = dx - (_radius << 1);
	SDL_Renderer* renderer = _renderer.Get();

	SDL_Point points[8];

    while (x >= y)
    {
		points[0] = { _x + x, _y + y };
		points[1] = { _x + y, _y + x };
		points[2] = { _x - y, _y + x };
		points[3] = { _x - x, _y + y };
		points[4] = { _x - x, _y - y };
		points[5] = { _x - y, _y - x };
		points[6] = { _x + y, _y - x };
		points[7] = { _x + x, _y - y };

		SDL_RenderDrawPoints(renderer, points, 8);

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
  }

  void DrawPoint(Renderer& _renderer, int _x, int _y)
  {
    SDL_RenderDrawPoint(_renderer.Get(), _x, _y);
  }
}
#include "Rect.h"

Rect::Rect()
{ }

Rect::Rect(const Vector2& _min, const Vector2& _max) :
  min(_min), max(_max)
{ }

Rect::Rect(float _minx, float _miny, float _maxx, float _maxy) :
	min(_minx, _miny), max(_maxx, _maxy)
{ }

float Rect::Width() const
{
	return max.x - min.x;
}

float Rect::Height() const
{
	return max.y - min.y;
}

Vector2 Rect::Size() const
{
	return max - min;
}

float Rect::Parimeter() const
{
	return (Width() + Height()) * 2.0f;
}

float Rect::Area() const
{
	return Width() * Height();
}

Vector2 Rect::TopLeft() const
{
  return min;
}

Vector2 Rect::TopRight() const
{
  return Vector2(max.x, min.y);
}

Vector2 Rect::BottomLeft() const
{
  return Vector2(min.x, max.y);
}

Vector2 Rect::BottomRight() const
{
  return max;
}

bool Rect::Contains(const Vector2& _point) const
{
  return _point.x >= min.x &&
         _point.x <= max.x &&
         _point.y >= min.y &&
         _point.y <= max.y;
}

bool Rect::Contains(const Rect& _rect) const
{
	return Contains(_rect.TopLeft())    && 
				 Contains(_rect.BottomRight());
}

bool Rect::Intersects(const Rect& _rect) const
{
  return this->min.x <= _rect.max.x &&
         this->max.x >= _rect.min.x &&
         this->min.y <= _rect.max.y &&
         this->max.y >= _rect.min.y;
}

void Rect::Draw(Renderer& _renderer)
{
	SDL_Rect rect = {
		Floor(min.x),  Floor(min.y),
    Ceil(Width()), Ceil(Height())
	};
	SDL_RenderDrawRect(_renderer.Get(), &rect);
}

bool Rect::Intersects(const Rect& _a, const Rect& _b)
{
	return _a.Intersects(_b);
}

Rect Rect::Union(const Rect& _a, const Rect& _b)
{
	Rect rect(
    Min(_a.min.x, _b.min.x),
    Min(_a.min.y, _b.min.y),

    Max(_a.max.x, _b.max.x),
    Max(_a.max.y, _b.max.y)
  );
	return rect;
}
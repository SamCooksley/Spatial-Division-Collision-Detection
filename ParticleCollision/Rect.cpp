#include "Rect.h"

Rect::Rect(void)
{ }

Rect::Rect(const Vector2 &_minimum, const Vector2 &_maximum) :
  m_minimum(_minimum), m_maximum(_maximum)
{ }

Rect::Rect(float _minX, float _minY, float _maxX, float _maxY) :
	m_minimum(_minX, _minY), m_maximum(_maxX, _maxY)
{ }

Rect::~Rect(void)
{
}

void Rect::Set(const Vector2 &_minimum, const Vector2 &_maximum)
{
	m_minimum = _minimum;
	m_maximum = _maximum;
}

void Rect::Set(float _minX, float _minY, float _maxX, float _maxY)
{
	m_minimum.x = _minX;
	m_minimum.y = _minY;

	m_maximum.x = _maxX;
	m_maximum.y = _maxY;
}

void Rect::MinX(float _min)
{
	m_minimum.x = _min;
}

void Rect::MaxX(float _max)
{
	m_maximum.x = _max;
}

void Rect::MinY(float _min)
{
	m_minimum.y = _min;
}

void Rect::MaxY(float _max)
{
	m_maximum.y = _max;
}

float Rect::MinX(void) const
{
	return m_minimum.x;
}

float Rect::MaxX(void) const
{
	return m_maximum.x;
}

float Rect::MinY(void) const
{
	return m_minimum.y;
}

float Rect::MaxY(void) const
{
	return m_maximum.y;
}

const Vector2 &Rect::Minimum(void) const
{
	return m_minimum;
}

const Vector2 &Rect::Maximum(void) const
{
	return m_maximum;
}

float Rect::Width(void) const
{
	return m_maximum.x - m_minimum.x;
}

float Rect::Height(void) const
{
	return m_maximum.y - m_minimum.y;
}

Vector2 Rect::Size(void) const
{
	return m_maximum - m_minimum;
}

float Rect::Parimeter(void) const
{
	return (Width() + Height()) * 2.0f;
}

float Rect::Area(void) const
{
	return Width() * Height();
}

Vector2 Rect::TopLeft(void) const
{
  return m_minimum;
}

Vector2 Rect::TopRight(void) const
{
  return Vector2(m_maximum.x, m_minimum.y);
}

Vector2 Rect::BottomLeft(void) const
{
  return Vector2(m_minimum.x, m_maximum.y);
}

Vector2 Rect::BottomRight(void) const
{
  return m_maximum;
}

bool Rect::Contains(const Vector2 &_point) const
{
  return _point.x >= m_minimum.x &&
         _point.x <= m_maximum.x &&
         _point.y >= m_minimum.y &&
         _point.y <= m_maximum.y;
}

bool Rect::Contains(const Rect &_rect) const
{
	return Contains(_rect.TopLeft())    && 
				 Contains(_rect.BottomRight());
}

bool Rect::Intersects(const Rect &_rect) const
{
  return this->m_minimum.x <= _rect.m_maximum.x &&
         this->m_maximum.x >= _rect.m_minimum.x &&
         this->m_minimum.y <= _rect.m_maximum.y &&
         this->m_maximum.y >= _rect.m_minimum.y;
}

void Rect::Union(const Rect &_a, const Rect &_b)
{
	m_minimum.x = fmin(_a.m_minimum.x, _b.m_minimum.x);
	m_minimum.y = fmin(_a.m_minimum.y, _b.m_minimum.y);

	m_maximum.x = fmax(_a.m_maximum.x, _b.m_maximum.x);
	m_maximum.y = fmax(_a.m_maximum.y, _b.m_maximum.y);
}

void Rect::Draw(Renderer &_renderer)
{
	SDL_Rect rect = {
		floor(m_minimum.x),
		floor(m_minimum.y),
		ceil(Width()),
		ceil(Height())
	};
	SDL_RenderDrawRect(_renderer.Get(), &rect);
}

bool Rect::Intersects(const Rect &_a, const Rect &_b)
{
	return _a.Intersects(_b);
}

Rect Rect::Combine(const Rect &_a, const Rect &_b)
{
	Rect rect;
	rect.Union(_a, _b);
	return rect;
}
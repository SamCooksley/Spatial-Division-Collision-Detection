#include "Polygon.h"

#include "CollisionManager.h"

Polygon::Polygon(const Vector2 &_position, const Vector2 &_velocity) : Collider(ColliderType::POLYGON, _position, _velocity)
{
  points = nullptr;
  size = 0;
  GenerateRandom(10, 40, 5);

	Range x = MinMaxOnAxis(Vector2(1, 0));
	Range y = MinMaxOnAxis(Vector2(0, 1));
	m_aabb.Set(x.minimum, y.minimum, x.maximum, y.maximum);
}

Polygon::~Polygon(void)
{
  Destroy();
}

void Polygon::Destroy(void)
{
  if (points != nullptr)
  {
    delete [] points;
    points = nullptr;
    size = 0;
  }
}

void Polygon::GenerateRandom(float _minSize, float _maxSize, int _vertexCount)
{
  Destroy();

  points = new Vector2[_vertexCount];
  size = _vertexCount;
  float angle = 0.0f;
  float step = 360.0f / _vertexCount;
  float total = 0.0f;

  for (int i = 0; i < size; i++)
  {
    angle += step;

    Vector2 dir = Vector2::AngleToVector(angle);
    float size = _minSize + rand() % (int)(_maxSize - _minSize);
    total += size;
    points[i] = dir * size;
  }

  total /= size;
  float area = PI * (total * total);
  invMass = 1.0f / total;
}

void Polygon::Draw(Renderer &_renderer)
{
  _renderer.SetRenderColour(0, 0, 0);
  SDL_Point *SDLpoints = new SDL_Point[size + 1];
  for (int i = 0; i < size; i++)
  {
    SDLpoints[i].x = points[i].x + position.x;
    SDLpoints[i].y = points[i].y + position.y;
  }

  SDLpoints[size].x = points[0].x + position.x;
  SDLpoints[size].y = points[0].y + position.y;

  SDL_RenderDrawLines(_renderer.Get(), SDLpoints, size + 1);

	delete [] SDLpoints;
}

bool Polygon::CheckCollision(const Collider &_other, CollisionData &_data) const
{
  return _other.CheckCollision(*this, _data);
}

bool Polygon::CheckCollision(const Circle &_other, CollisionData &_data) const
{
  return CollisionManager::CheckCollision(_other, *this, _data);
}

bool Polygon::CheckCollision(const Polygon &_other, CollisionData &_data) const
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

bool Polygon::CheckCollision(const Plane &_other, CollisionData &_data) const
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

Vector2 Polygon::Min(void) const 
{
  return Vector2();
}

Vector2 Polygon::Max(void) const
{
  return  Vector2();
}

Range Polygon::MinMaxOnAxis(const Vector2 &_axis) const 
{
  float temp = Vector2::Dot(points[0], _axis);
  Range range;
  range.minimum = temp;
  range.maximum = temp;

  for (int i = 1; i < size; i++)
  {
    float test = Vector2::Dot(points[i], _axis);

    if (test < range.minimum)
    {
      range.minimum = test;
    }
    if (test > range.maximum)
    {
      range.maximum = test;
    }
  }
  float pos = Vector2::Dot(position, _axis);
  range.minimum += pos;
  range.maximum += pos;

  return range;
}
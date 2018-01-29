#include "Polygon.h"

#include "CollisionManager.h"

Polygon::Polygon(const Vector2& _position, const Vector2& _velocity) : Collider(ColliderType::POLYGON, _position, _velocity)
{
  GenerateRandom(10, 40, 5);
}

void Polygon::GenerateRandom(float _minSize, float _maxSize, int _vertexCount)
{
  //allocate enough storage for the points.
  m_points.resize(_vertexCount);

  float curAngle = 0.f; 
  float step = 360.f / _vertexCount; //angle differnce between vertices.
  float averageDistance = 0.f;

  //range of sizes.
  int sizeDiff = static_cast<int>(_maxSize - _minSize);

  //set the positions of each vertex.
  for (size_t i = 0; i < m_points.size(); ++i)
  {
    curAngle += step;

    //get the direction of the point.
    Vector2 dir = Vector2::AngleToVector(curAngle * DEG2RAD);
    //get the distance of the point.
    float size = _minSize + rand() % sizeDiff;
    averageDistance += size;
    
    m_points[i] = dir * size;
  }

  //set the mass.
  averageDistance /= m_points.size();
  float area = PI * (averageDistance * averageDistance);
  m_invMass = 1.f / averageDistance;

  //update the aabb.
  Range x = MinMaxOnAxis(Vector2(1, 0));
  Range y = MinMaxOnAxis(Vector2(0, 1));
  m_aabb = Rect(x.min, y.min, x.max, y.max);
}

void Polygon::Draw(Renderer& _renderer)
{
  //exit if there is not even a line.
  if (m_points.size() < 2) { return; }

  //convert the local points to world space.
  std::vector<SDL_Point> points(m_points.size() + 1);

  for (size_t i = 0; i < m_points.size(); ++i)
  {
    points[i].x = static_cast<int>(m_points[i].x + m_position.x);
    points[i].y = static_cast<int>(m_points[i].y + m_position.y);
  }

  //connect back to the first point.
  points[m_points.size()].x = points[0].x;
  points[m_points.size()].y = points[0].y;

  SDL_RenderDrawLines(_renderer.Get(), &points[0], points.size());
}

bool Polygon::CheckCollision(Collider& _other, CollisionData& _data)
{
  //go through the other collider with this object now specified.
  return _other.CheckCollision(*this, _data);
}

bool Polygon::CheckCollision(Circle& _other, CollisionData& _data)
{
  return CollisionManager::CheckCollision(_other, *this, _data);
}

bool Polygon::CheckCollision(Polygon& _other, CollisionData& _data)
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

bool Polygon::CheckCollision(Plane& _other, CollisionData& _data)
{
  return CollisionManager::CheckCollision(*this, _other, _data);
}

Range Polygon::MinMaxOnAxis(const Vector2& _axis) const 
{
  //if there is no points, the object does not exist.
  if (m_points.empty()) 
  {
    throw std::out_of_range("No vertices set.");
  }

  //project each point onto the axis. Set the min and
  //max accordingly.

  float tmp = Vector2::Dot(m_points[0], _axis);
  Range range(tmp, tmp);

  for (size_t i = 1; i < m_points.size(); i++)
  {
    tmp = Vector2::Dot(m_points[i], _axis);
   
    range.min = ::Min(tmp, range.min);
    range.max = ::Max(tmp, range.max);
  }

  //add the position to move it to world space.
  float pos = Vector2::Dot(m_position, _axis);
  range.min += pos;
  range.max += pos;

  return range;
}
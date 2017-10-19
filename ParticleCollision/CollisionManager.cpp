#include "CollisionManager.h"

#include "QuadTree.h"
#include <iostream>

CollisionManager::CollisionManager(void) :
	m_quadTree(Vector2(800, 600), 5)
{
}

CollisionManager::~CollisionManager(void)
{ 
}

void CollisionManager::Collide(void)
{
	if (m_divisionType == DivisionType::NONE)
	{
		int size = colliders.size();
		for (int i = 0; i < size; i++)
		{
			for (int j = i + 1; j < size; j++)
			{
				Collide(*colliders.at(i), *colliders.at(j));
			}
		}
	}
	else if (m_divisionType == DivisionType::QUAD)
	{
		auto collide = [this](Collider &_a, Collider &_b)
		{
			Collide(_a, _b);
		};

		m_quadTree.CheckCollision(collide);
	}
	else if (m_divisionType == DivisionType::AABB)
	{
		auto &pairs = m_aabbTree.GetColliderPairs();

		for (auto &pair : pairs)
		{
			Collide(pair.A(), pair.B());
		}
	}
}

void CollisionManager::Draw(Renderer &_renderer)
{
	if (m_divisionType == DivisionType::QUAD)
	{
		m_quadTree.Draw(_renderer);
	}
	else if (m_divisionType == DivisionType::AABB)
	{
		m_aabbTree.Draw(_renderer);
	}
}

void CollisionManager::Collide(Collider &_a, Collider &_b)
{
  CollisionData data;
  if (_b.CheckCollision(_a, data))
  {
    ResolveCollision(*data.a, *data.b, data);
  }
}

bool CollisionManager::CollisionOnAxis(const Collider &_a, const Collider &_b, const Vector2 &_axis)
{
  Range ar = _a.MinMaxOnAxis(_axis);
  Range br = _b.MinMaxOnAxis(_axis);

  if (ar.Overlap(br))
  {
    return true;
  }

  return false;
}

bool CollisionManager::CheckCollision(const Circle &_a, const Circle &_b, CollisionData &_data)
{
  //get the difference in positions
  Vector2 diff = _a.position - _b.position;
  //get the squared distance (squared magnitude) 
  float distSq = diff.MagnitudeSq();
  //get the total radii
  float rad = _a.radius + _b.radius;

  //if the circles are overlapping
  if (distSq <= rad * rad)
  {
    _data.a = (Collider*)&_a;
    _data.b = (Collider*)&_b;
    //get the amount of overlap (inverse of distance)
    _data.overlap = Max(-(sqrt(distSq) - rad), 0);
    //prevent division by 0
    if (distSq == 0) { diff.y = 1; }
    //get the collision normal.
    _data.normal = diff.Normalized();
    //say that there has been a collision
    return true;
  }

  //no collision
  return false;
}

bool CollisionManager::CheckCollision(const Circle &_a, const Polygon &_b, CollisionData &_data)
{
  for (int i = 1; i < _b.size + 1; i++)
  {
    const Vector2 &v1 = _b.position + _b.points[i - 1];
    const Vector2 &v2 = _b.position + _b.points[i % _b.size];

    Vector2 edge = (v1 - v2).Normalized();

    float circlePos = Vector2::Dot(_a.position, edge);
    Range edgePos;
    edgePos.minimum = Vector2::Dot(v1, edge);
    edgePos.maximum = Vector2::Dot(v2, edge);

    edgePos.Sort();

    if (edgePos.Distance(circlePos) <= 0.0f)
    {
      Vector2 normal = edge.Left();
      Vector2 diff = v1 - _a.position;
      float dist = Vector2::Dot(diff, normal);
      //dist -= _a.radius;

      if (dist <= _a.radius && dist >= 0.0f)
      {
        _data.a = (Collider*)&_b;
        _data.b = (Collider*)&_a;
        //get the overlap
        _data.overlap = -(dist - _a.radius);
        //get the collision normal
        _data.normal = normal;
        //say that there has been a collision
        return true;
      }
    }

    Vector2 diff = _a.position - v1;
    float distSq = diff.MagnitudeSq();
    float radSq = _a.radius * _a.radius;
    if (distSq <= radSq)
    {
      _data.a = (Collider*)&_a;
      _data.b = (Collider*)&_b;
      //get the overlap
      _data.overlap = -(sqrt(distSq) - _a.radius);
      //get the collision normal
      _data.normal = diff.Normalized();
      //say that there has been a collision
      return true;
    }
  }

  return false;
}

bool CollisionManager::CheckCollision(const Circle &_a, const Plane &_b, CollisionData &_data)
{
  //get the distance between the particle and the plane
  Vector2 diff = _a.position - _b.position;
  float dist = Vector2::Dot(diff, _b.normal); //get the distance on the plane's normal
  dist -= _a.radius; //get the distance from the closest point on the circle. - actual distance

  //if they overlap
  if (dist <= 0.0f)
  {
    _data.a = (Collider*)&_a;
    _data.b = (Collider*)&_b;
    //get the overlap
    _data.overlap = -dist;
    //get the collision normal
    _data.normal = _b.normal;
    //say that there has been a collision
    return true;
  }

  //no collision
  return false;
}

bool CollisionManager::CheckCollision(const Polygon &_lhs, const Polygon &_rhs, CollisionData &_data)
{
  bool collided = false;

  Vector2 direction((_lhs.position - _rhs.position).Normalized());

  if (CollisionOnAxis(_lhs, _rhs, direction) &&
      CollisionOnAxis(_lhs, _rhs, direction.Left()))
  {
    collided = true;
    _data.overlap = 9999.9f;

    collided = CheckEdgeCollisions(_lhs, _rhs, _data);

    if (collided)
    {
      collided = CheckEdgeCollisions(_rhs, _lhs, _data);
    }
  }
  return collided;
}

bool CollisionManager::CheckCollision(const Polygon &_a, const Plane &_b, CollisionData &_data)
{
  Range r = _a.MinMaxOnAxis(_b.normal);
  float pos = Vector2::Dot(_b.position, _b.normal);
  if (r.minimum < pos)
  {
    _data.a = (Collider*)&_a;
    _data.b = (Collider*)&_b;
    _data.normal = _b.normal;
    _data.overlap = pos - r.minimum;
    return true;
  }
  return false;
}

bool CollisionManager::CheckEdgeCollisions(const Polygon &_a, const Polygon &_b, CollisionData &_data)
{
  bool collided = true;

  for (int i = 1; i < _a.size + 1; i++)
  {
    Vector2 normal = (_a.points[i - 1] - _a.points[i % _a.size]).Normalized().Left();

    Range lr = _a.MinMaxOnAxis(normal);
    Range rr = _b.MinMaxOnAxis(normal);

    if (!lr.Overlaps(rr))
    {
      collided = false;
      break;
    }
    else
    {
      float overlap = lr.Overlap(rr);
      if (overlap < _data.overlap)
      {
        _data.normal = normal;
        _data.overlap = overlap;
        _data.a = (Collider*)&_a;
        _data.b = (Collider*)&_b;
      }
    }
  }

  return collided;
}

void CollisionManager::ResolveCollision(Collider &_a, Collider &_b, const CollisionData &_data)
{
  //get the total mass
  //as inverse masses are used, smaller mass would have a higher 
  //percentage of the total, making them be more effected by collisions. 
  float mT = _a.invMass + _b.invMass;

  //total bounciness
  float bounce = _a.bounciness * _b.bounciness; 

  //if there is an overlap, separate the particles
  if (_data.overlap >= 0.0f)
  {
    //move the objects apart by their overlap amount in the direction of the collision normal.
    //use the mass to move lighter objects further.
    _a.position += _data.normal * ((_data.overlap + 0.001f) * (_a.invMass / mT));
    _b.position -= _data.normal * ((_data.overlap + 0.001f) * (_b.invMass / mT));
  }
  
  //Get the reflection velocity total magnitude
  //Get the relative velocity on the normal and apply bounce. Divide by total mass so it can be distributed by mass. 
  float p = ((1 + bounce) * (Vector2::Dot(_a.velocity, _data.normal) - Vector2::Dot(_b.velocity, _data.normal))) / mT;

  //apply the velocity change in opposite directions and apply in proportion to the mass
  _a.velocity -= _data.normal * (p * _a.invMass);
  _b.velocity += _data.normal * (p * _b.invMass); 
}
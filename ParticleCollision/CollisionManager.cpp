#include "CollisionManager.h"

#include "QuadTree.h"
#include <iostream>

CollisionManager::CollisionManager() :
	m_quadTree(Vector2(800, 600), 5)
{ }

void CollisionManager::Collide()
{
  switch (m_divisionType)
  {
    case CollisionManager::DivisionType::NONE:
    {
      //go through all combonations and test for a collision.
      for (size_t i = 0; i < m_colliders.size() - 1; ++i)
      {
        for (size_t j = i + 1; j < m_colliders.size(); ++j)
        {
          Collide(*m_colliders.at(i), *m_colliders.at(j));
        }
      }
      break;
    }
    case CollisionManager::DivisionType::QUAD:
    {
      //define what to do with objects in a bucket.
      auto collide = [](Collider& _a, Collider& _b)
      {
        Collide(_a, _b);
      };

      m_quadTree.CheckCollision(collide);
      break;
    }
    case CollisionManager::DivisionType::AABB:
    {
      auto& pairs = m_aabbTree.GetColliderPairs();

      for (auto& pair : pairs)
      {
        Collide(pair.A(), pair.B());
      }
      break;
    }
  }
}

void CollisionManager::Draw(Renderer &_renderer)
{
  switch (m_divisionType)
  {
    case CollisionManager::DivisionType::QUAD:
    {
      m_quadTree.Draw(_renderer);
      break;
    }
    case CollisionManager::DivisionType::AABB:
    {
      m_aabbTree.Draw(_renderer);
      break;
    }
  }
}

void CollisionManager::Add(const std::shared_ptr<Collider>& _collider)
{
  m_colliders.emplace(_collider);
}

QuadTree::QuadTree& CollisionManager::GetQuadTree()
{
  return m_quadTree;
}

AABBTree::AABBTree& CollisionManager::GetAABBTree()
{
  return m_aabbTree;
}

void CollisionManager::SetBroadPhase(BroadPhaseType _type)
{
  m_broadPhase = _type;
}

void CollisionManager::Collide(Collider &_a, Collider &_b)
{
  CollisionData data;
  //flip a and b as the double dispatch will do _a.CheckCollision(_b, data).
  if (_b.CheckCollision(_a, data))
  {
    ResolveCollision(*data.a, *data.b, data);
  }
}

bool CollisionManager::CollisionOnAxis(const Collider &_a, const Collider &_b, const Vector2 &_axis)
{
  Range ar = _a.MinMaxOnAxis(_axis);
  Range br = _b.MinMaxOnAxis(_axis);
 
  return ar.Overlap(br);
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
    _data.a = static_cast<Collider*>(&_a);
    _data.b = static_cast<Collider*>(&_b);
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
  bool col = false;

  //go through all edges of the polygon.
  for (size_t i = 1; i < _b.m_points.size() + 1; ++i)
  {
    const Vector2 v1 = _b.position + _b.m_points[i - 1];
    const Vector2 v2 = _b.position + _b.m_points[i % _b.m_points.size()];

    Vector2 edge = (v1 - v2).Normalized();

    //check of the circle can overlap the edge.

    float circlePos = Vector2::Dot(_a.position, edge);
    Range edgePos;
    edgePos.min = Vector2::Dot(v1, edge);
    edgePos.max = Vector2::Dot(v2, edge);
    edgePos.Sort();

    //if the circle position overlaps the edge.
    if (edgePos.Overlaps(circlePos))
    {
      //plane circle collision.

      Vector2 normal = edge.Left();
      Vector2 diff = v1 - _a.position;
      float dist = Vector2::Dot(diff, normal);
      //dist -= _a.radius;

      //make sure its above the plane.
      if (dist <= _a.radius && dist >= 0.0f)
      {
        _data.a = static_cast<Collider*>(&_b);
        _data.b = static_cast<Collider*>(&_a);
        //get the overlap
        _data.overlap = -(dist - _a.radius);
        //get the collision normal
        _data.normal = normal;
        //say that there has been a collision
        col = true;
        continue;
        //return true;
      }
    }
    //if the circle overlaps the edge with the radius.
    //else if (edgePos.Distance(circlePos) <= _a.radius)
    {
      //get the distance from the first point of the edge.
      float distSq = (_a.position - v1).MagnitudeSq();
      float radSq = _a.radius * _a.radius;
      if (distSq <= radSq)
      {
        _data.a = static_cast<Collider*>(&_a);
        _data.b = static_cast<Collider*>(&_b);
        //get the overlap
        _data.overlap = -(sqrt(distSq) - _a.radius);
        //get the collision normal
        _data.normal = diff.Normalized();
        //say that there has been a collision
        col = true;
        continue;
        //return true;
      }
    }
  }

  return col;
}

bool CollisionManager::CheckCollision(const Circle& _a, const Plane& _b, CollisionData& _data)
{
  //get the distance between the particle and the plane
  Vector2 diff = _a.position - _b.position;
  float dist = Vector2::Dot(diff, _b.m_normal); //get the distance on the plane's normal
  dist -= _a.radius; //get the distance from the closest point on the circle. - actual distance

  //if they overlap
  if (dist <= 0.0f)
  {
    _data.a = static_cast<Collider*>(&_a);
    _data.b = static_cast<Collider*>(&_b);
    //get the overlap
    _data.overlap = -dist;
    //get the collision normal
    _data.normal = _b.m_normal;
    //say that there has been a collision
    return true;
  }

  //no collision
  return false;
}

bool CollisionManager::CheckCollision(const Polygon& _lhs, const Polygon& _rhs, CollisionData& _data)
{
  bool collided = false;

  Vector2 direction = (_lhs.position - _rhs.position).Normalized();

  //check if the objects are intersecting in a box aligned to the direction.
  if (CollisionOnAxis(_lhs, _rhs, direction) &&
      CollisionOnAxis(_lhs, _rhs, direction.Left()))
  {
    collided = true;
    _data.overlap = std::numeric_limits<float>().max();

    //Separate Axis Theorem (SAT)
    
    collided = CheckEdgeCollisions(_lhs, _rhs, _data);

    if (collided)
    {
      collided = CheckEdgeCollisions(_rhs, _lhs, _data);
    }
  }
  return collided;
}

bool CollisionManager::CheckCollision(const Polygon& _a, const Plane& _b, CollisionData& _data)
{
  //project the polygon on the plane.
  Range r = _a.MinMaxOnAxis(_b.m_normal);
  float pos = Vector2::Dot(_b.position, _b.m_normal);
  
  if (r.min < pos)
  {
    //check if the polygon is within the width of the plane.
    Vector2 edge = _b.m_normal.Right();
    Range r2 = _a.MinMaxOnAxis(edge);
    Range pr = _b.MinMaxOnAxis(edge);

    if (Range::Overlaps(r2, pr))
    {
      _data.a = static_cast<Collider*>(&_a);
      _data.b = static_cast<Collider*>(&_b);
      _data.normal = _b.m_normal;
      _data.overlap = pos - r.min;
      return true;
    }
  }
  return false;
}

bool CollisionManager::CheckEdgeCollisions(const Polygon& _a, const Polygon& _b, CollisionData& _data)
{
  bool collided = true;

  //go through all the edges of a.
  for (size_t i = 1; i < _a.m_points.size() + 1; i++)
  {
    //get the normal of the edge.
    Vector2 normal = (_a.m_points[i - 1] - _a.m_points[i % _a.m_points.size()]).Normalized().Left();

    //get the size of each polygon on the edge.
    Range lr = _a.MinMaxOnAxis(normal);
    Range rr = _b.MinMaxOnAxis(normal);

    if (Range::Overlaps(lr, rr))
    {
      //if the overlap is smaller.
      float overlap = Range::Overlap(lr, rr);
      if (overlap < _data.overlap)
      {
        _data.normal = normal;
        _data.overlap = overlap;
        _data.a = static_cast<Collider*>(&_a);
        _data.b = static_cast<Collider*>(&_b);
      }
    }
    //if they don't overlap, there is no collision.
    else
    {
      collided = false;
      break;
    }
  }

  return collided;
}

void CollisionManager::ResolveCollision(Collider& _a, Collider& _b, const CollisionData& _data)
{
  //get the total mass
  //as inverse masses are used, smaller mass would have a higher 
  //percentage of the total, making them be more effected by collisions. 
  float mT = _a.invMass + _b.invMass;

  //total bounciness
  float bounce = _a.bounciness * _b.bounciness; 

  //if there is an overlap, separate the particles
  if (_data.overlap >= .0f)
  {
    //move the objects apart by their overlap amount in the direction of the collision normal.
    //use the mass to move lighter objects further.
    _a.position += _data.normal * ((_data.overlap + .001f) * (_a.invMass / mT));
    _b.position -= _data.normal * ((_data.overlap + .001f) * (_b.invMass / mT));
  }
  
  //Get the reflection velocity total magnitude
  //Get the relative velocity on the normal and apply bounce. Divide by total mass so it can be distributed by mass. 
  float p = ((1.f + bounce) * (Vector2::Dot(_a.velocity, _data.normal) - Vector2::Dot(_b.velocity, _data.normal))) / mT;

  //apply the velocity change in opposite directions and apply in proportion to the mass
  _a.velocity -= _data.normal * (p * _a.invMass);
  _b.velocity += _data.normal * (p * _b.invMass); 
}
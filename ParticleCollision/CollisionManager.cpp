#include "CollisionManager.h"

#include "QuadTree.h"
#include <iostream>

CollisionManager::CollisionManager() :
	m_quadTree(Vector2(800, 600))
{ }

void CollisionManager::Collide()
{
  switch (m_broadPhase)
  {
    case BroadPhaseType::NONE:
    {
      if (m_colliders.empty()) { break; }

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
    case BroadPhaseType::QUAD:
    {
      //define what to do with objects in a bucket.
      //this should not be needs as Collide is a static method.
      auto collide = [this](Collider* _a, Collider* _b)
      {
        CollisionManager::Collide(*_a, *_b);
      };

      m_quadTree.CheckCollision(collide);

      break;
    }
    case BroadPhaseType::AABB:
    {
      m_aabbTree.Update();

      auto& pairs = m_aabbTree.GetColliderPairs();

      for (auto& pair : pairs)
      {
        Collide(*pair.a, *pair.b);
      }
      break;
    }
  }
}

void CollisionManager::Draw(Renderer &_renderer)
{
  switch (m_broadPhase)
  {
    case BroadPhaseType::QUAD:
    {
      m_quadTree.Draw(_renderer);
      break;
    }
    case BroadPhaseType::AABB:
    {
      m_aabbTree.Draw(_renderer);
      break;
    }
  }
}

void CollisionManager::Add(const std::shared_ptr<Collider>& _collider)
{
  m_colliders.emplace_back(_collider);
}

QuadTree::QuadTree<Collider>& CollisionManager::GetQuadTree()
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

BroadPhaseType CollisionManager::GetBroadPhase() const
{
  return m_broadPhase;
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

bool CollisionManager::CollisionOnAxis(Collider& _a, Collider& _b, const Vector2 &_axis)
{
  Range ar = _a.MinMaxOnAxis(_axis);
  Range br = _b.MinMaxOnAxis(_axis);
 
  return ar.Overlap(br);
}

bool CollisionManager::CheckCollision(Circle& _a, Circle& _b, CollisionData &_data)
{
  //get the difference in positions
  Vector2 diff = _a.m_position - _b.m_position;
  //get the squared distance (squared magnitude) 
  float distSq = diff.MagnitudeSq();
  //get the total radii
  float rad = _a.m_radius + _b.m_radius;

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

bool CollisionManager::CheckCollision(Circle& _a, Polygon& _b, CollisionData &_data)
{
  bool col = false;

  //go through all edges of the polygon.
  for (size_t i = 1; i < _b.m_points.size() + 1; ++i)
  {
    const Vector2 v1 = _b.m_position + _b.m_points[i - 1];
    const Vector2 v2 = _b.m_position + _b.m_points[i % _b.m_points.size()];

    Vector2 edge = (v1 - v2).Normalized();

    //check of the circle can overlap the edge.

    float circlePos = Vector2::Dot(_a.m_position, edge);
    Range edgePos;
    edgePos.min = Vector2::Dot(v1, edge);
    edgePos.max = Vector2::Dot(v2, edge);
    edgePos.Sort();

    //if the circle position overlaps the edge.
    if (edgePos.Contains(circlePos))
    {
      //plane circle collision.

      Vector2 normal = edge.Left();
      Vector2 diff = v1 - _a.m_position;
      float dist = Vector2::Dot(diff, normal);
      //dist -= _a.radius;

      //make sure its above the plane.
      if (dist <= _a.m_radius && dist >= 0.0f)
      {
        _data.a = static_cast<Collider*>(&_b);
        _data.b = static_cast<Collider*>(&_a);
        //get the overlap
        _data.overlap = -(dist - _a.m_radius);
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
      Vector2 diff = _a.m_position - v1;
      float distSq = diff.MagnitudeSq();
      float radSq = _a.m_radius * _a.m_radius;
      if (distSq <= radSq)
      {
        _data.a = static_cast<Collider*>(&_a);
        _data.b = static_cast<Collider*>(&_b);
        //get the overlap
        _data.overlap = -(sqrt(distSq) - _a.m_radius);
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

bool CollisionManager::CheckCollision(Circle& _a, Plane& _b, CollisionData& _data)
{
  //get the distance between the particle and the plane
  Vector2 diff = _a.m_position - _b.m_position;
  //get the distance on the plane's normal
  float dist = Vector2::Dot(diff, _b.m_normal); 
  //get the distance from the closest point on the circle. - actual distance
  dist -= _a.m_radius; 

  //if they overlap on the normal axis.
  if (dist <= 0.0f)
  {
    //check if the circle position is within the width of the plane.
    Vector2 edge = _b.m_normal.Right();
    float circlePos = Vector2::Dot(_a.m_position, edge);
    Range planeRange = _b.MinMaxOnAxis(edge);

    if (planeRange.Contains(circlePos))
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

    //check if the circle radius overlaps the plane.

    //get the distance from the first point of the edge.
    Vector2 diff = _a.m_position - _b.Min();
    float distSq = diff.MagnitudeSq();

    //get the closest point on the plane's edge.
    {
      Vector2 diff2 = _a.m_position - _b.Max();
      float distSq2 = diff.MagnitudeSq();

      if (distSq2 < distSq)
      {
        diff = diff2;
        distSq = distSq2;
      }
    }

    float radSq = _a.m_radius * _a.m_radius;

    if (distSq <= radSq)
    {
      _data.a = static_cast<Collider*>(&_a);
      _data.b = static_cast<Collider*>(&_b);
      //get the overlap
      _data.overlap = -(sqrt(distSq) - _a.m_radius);
      //get the collision normal
      _data.normal = diff.Normalized();
      //say that there has been a collision
      return true;
    }
  }

  //no collision
  return false;
}

bool CollisionManager::CheckCollision(Polygon& _lhs, Polygon& _rhs, CollisionData& _data)
{
  bool collided = false;

  Vector2 direction = (_lhs.m_position - _rhs.m_position).Normalized();

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

bool CollisionManager::CheckCollision(Polygon& _a, Plane& _b, CollisionData& _data)
{
  //TODO: do SAT with each edge of the polygon for accurate collision.
  //      - problem with width of plane.

  //project the polygon on the plane.
  Range r = _a.MinMaxOnAxis(_b.m_normal);
  float pos = Vector2::Dot(_b.m_position, _b.m_normal);
  
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

bool CollisionManager::CheckEdgeCollisions(Polygon& _a, Polygon& _b, CollisionData& _data)
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
  float totalMass = _a.m_invMass + _b.m_invMass;
  if (totalMass != .0f)
  {
    //totalMass = 1.f / totalMass;
  }

  //total bounciness
  float bounce = _a.m_bounciness * _b.m_bounciness; 

  //if there is an overlap, separate the particles
  if (_data.overlap >= .0f)
  {
    //move the objects apart by their overlap amount in the direction of the collision normal.
    //use the mass to move lighter objects further.
    _a.m_position += _data.normal * ((_data.overlap + .001f) * (_a.m_invMass / totalMass));
    _b.m_position -= _data.normal * ((_data.overlap + .001f) * (_b.m_invMass / totalMass));
  }
  
  //Get the reflection velocity total magnitude
  //Get the relative velocity on the normal and apply bounce. Divide by total mass so it can be distributed by mass. 
  float p = ((1.f + bounce) * (Vector2::Dot(_a.m_velocity - _b.m_velocity, _data.normal))) / totalMass;

  //apply the velocity change in opposite directions and apply in proportion to the mass
  _a.m_velocity -= _data.normal * (p * _a.m_invMass);
  _b.m_velocity += _data.normal * (p * _b.m_invMass); 
}
#pragma once

#include "Maths.h"
#include "Range.h"
#include "Renderer.h"

#include "CollisionData.h"
#include "ItemCollider.h"
#include "AABBTreeItemCollider.h"

class Circle;
class Polygon;
class Plane;

enum class ColliderType
{
  AABB,
  CIRCLE,
  POLYGON,
  PLANE
};

class Collider
{
  friend class CollisionManager;
private:
  ColliderType type;

protected:
  Vector2 position;
  Vector2 velocity;

  float invMass;
  float bounciness;

	Rect m_aabb;

	QuadTree::ItemCollider m_item;
	AABBTree::ItemCollider m_aabbItem;

public:
  Collider(ColliderType _type, const Vector2 &_position, const Vector2 &_velocity);
  virtual ~Collider(void);

  virtual void Update(float _deltaTime);
  virtual void Draw(Renderer &_renderer) { }

  virtual bool CheckCollision(const Collider &_other, CollisionData &_data) const = 0;
  virtual bool CheckCollision(const Circle   &_other, CollisionData &_data) const = 0;
  virtual bool CheckCollision(const Polygon  &_other, CollisionData &_data) const = 0;
  virtual bool CheckCollision(const Plane    &_other, CollisionData &_data) const = 0;

  virtual Range MinMaxOnAxis(const Vector2 &_axis) const = 0;

  const Vector2 &Position(void) const { return position; }
  const Vector2 &Velocity(void) const { return velocity; }

	const Rect &AABB(void) const { return m_aabb; }

	QuadTree::Item *AsItem(void) const;
	AABBTree::Item *AsAABBItem(void) const;
};


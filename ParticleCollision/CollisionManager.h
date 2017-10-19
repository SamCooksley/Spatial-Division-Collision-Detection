#pragma once

#include <vector>
#include <memory>

#include "Collider.h"
#include "Polygon.h"
#include "Circle.h"
#include "Plane.h"

#include "CollisionData.h"
#include "QuadTree.h"
#include "AABBTree.h"

class CollisionManager
{
public:
	enum class DivisionType
	{
		NONE,
		QUAD,
		AABB
	};
private:
	DivisionType m_divisionType;

  std::vector<std::shared_ptr<Collider>> colliders;

	QuadTree::QuadTree m_quadTree;
	AABBTree::AABBTree m_aabbTree;

public:
  CollisionManager(void);
  ~CollisionManager(void);

  void Collide(void);
	void Draw(Renderer &_renderer);

	QuadTree::QuadTree &GetTree(void) { return m_quadTree; }
	AABBTree::AABBTree &GetAABBTree(void) { return m_aabbTree; }

  void Collide(Collider &_lhs, Collider &_rhs);

  void Add(const std::shared_ptr<Collider> &_collider) { colliders.emplace_back(_collider); }

	void SetDivisionType(DivisionType _type) { m_divisionType = _type; }
	DivisionType GetDivisionType(void) const { return m_divisionType; }

public:
  static bool CollisionOnAxis(const Collider &_a, const Collider &_b, const Vector2 &_axis);
  
  static bool CheckCollision(const Circle &_a, const Circle &_b, CollisionData &_data);
  static bool CheckCollision(const Circle &_a, const Polygon &_b, CollisionData &_data);
  static bool CheckCollision(const Circle &_a, const Plane &_b, CollisionData &_data);
  static bool CheckCollision(const Polygon &_a, const Polygon &_b, CollisionData &_data);
  static bool CheckCollision(const Polygon &_a, const Plane &_b, CollisionData &_data);

  static bool CheckEdgeCollisions(const Polygon &_a, const Polygon &_b, CollisionData &_data);

  static void ResolveCollision(Collider &_a, Collider &_b, const CollisionData &_data);
};


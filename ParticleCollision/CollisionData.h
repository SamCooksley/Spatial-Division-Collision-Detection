#pragma once

#include "Maths.h"

class Collider;

class CollisionData
{
public:
  float overlap; 
  Vector2 normal;
  Vector2 position;
  Collider *a;
  Collider *b;
};
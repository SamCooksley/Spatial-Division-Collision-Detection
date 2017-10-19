#ifndef _GAME_H_
#define _GAME_H_

/**
 * \brief Manages the application.
 */

#include <vector>

#include "Window.h"
#include "Renderer.h"

#include "CollisionManager.h"
#include "Collider.h"

class Game
{
private:
  bool done; 

  float deltaTime;

  Window window;    
  Renderer renderer; 

  CollisionManager collisionManager;
  std::vector<std::shared_ptr<Collider>> colliders;

	Rect m_spawnRect;

public:
  Game(void);
  ~Game(void); 

  int Run(void);

  void Quit(void) { done = true; } 

  const Window &GetWindow(void) const { return window; }

  void AddPolygons(int _count);
  void AddCircles(int _count);
  void AddPlane(const Vector2 &_position, const Vector2 &_normal, float _width);

private:
  bool Init(void); 
  void Loop(void); 
  void Exit(void);

  void HandleEvents(void); 
  void Update(void);  
  void Render(void); 
};

#endif //_GAME_H_
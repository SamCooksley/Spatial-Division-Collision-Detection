#ifndef _GAME_H_
#define _GAME_H_

#include <vector>

#include "Window.h"
#include "Renderer.h"

#include "Text.h"

#include "CM_BruteForce.h"
#include "CM_QuadTree.h"
#include "CM_AABBTree.h"

/**
 * \brief Manages the application.
 */

class Game
{
 public:
  int Run(); //!< Start the application.

  void Quit(); //!< Close the application. Exit the main loop.

  void AddPolygons(int _count); //!< Add polygons to the scene.
  void AddCircles(int _count); //!< Add circles to the scene.

  /**
   * \brief Add a plane to the scene.
   * \param [in] _position Position of the plane.
   * \param [in] _normal   Orientation of the plane.
   * \param [in] _width    Size of the plane.
   */
  void AddPlane(const Vector2& _position, const Vector2& _normal, float _width);

 private:
  bool Init(); //!< Setup the application.
  void Loop(); //!< Main loop.
  void Exit(); //!< Shutdown the application.

  void HandleEvents(); //!< Update 
  void Update(); //!< Run the simulation.
  void Render(); //!< Draw all the objects.

  bool m_done; //!< Should the application quit.

  float m_deltaTime; //!< Time taken for the previous frame to be completed.

  Window m_window; //!< Window of the application.
  Renderer m_renderer; //!< Renderer to draw to the window.

  CollisionManager* m_current;

  CM_BruteForce m_brute;
  CM_QuadTree m_quad;
  CM_AABBTree m_aabb;

  std::vector<std::shared_ptr<Collider>> m_colliders; //!< List of objects in the scene.

  Rect m_spawnRect; //!< Area to spawn objects.

  std::unique_ptr<Text> m_timeText;
  std::unique_ptr<Text> m_minText;
  std::unique_ptr<Text> m_maxText;
};

#endif //_GAME_H_
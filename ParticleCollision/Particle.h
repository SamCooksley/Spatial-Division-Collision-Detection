#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <memory>

#include "Maths.h"
#include "Sprite.h"

#include "SpatialHash.h"

class Game;

/**
 * \brief Collection of data required to resolve a collision.
 * Could be expanded. eg. collision position.
 */
struct CollisionData
{
  float overlap;  //!< Amount of overlap between objects.
  Vector2 normal; //!< Normal of the collision.
};

class Particle
{
private:
  unsigned int m_id; //!< Unique identification of the particle.

  Game *m_game; //!< Game instance. Used to get data such as window size.

  Vector2 m_position; //!< Position.
  Vector2 m_velocity; //!< Movement.

  float m_radius; //!< Size.

  float m_invMass;    //!< Used to calculate the momentum of the particle. 
  float m_bounciness; //!< Amount of momentum conservation. Coefficient of restitution.

  Sprite m_sprite; //!< Image to display.  

public:
  /**
   * \brief Constructor.
   *
   * \param [in] _game    Game.
   * \param [in] _id      ID.
   * \param [in] _pos     Position.
   * \param [in] _radius  Half size.
   * \param [in] _vel     Velocity.
   * \param [in] _texture Image to draw.
   */
  Particle(Game *_game, unsigned int _id, const Vector2 &_pos, float _radius, const Vector2 &_vel, const std::shared_ptr<Texture> &_texture);
  ~Particle(void); //!< Destructor.

  /**
   * \brief Update the position of the particle.
   *
   * \param [in] _deltaTime Frame time.
   */
  void Update(float _deltaTime);

  /**
   * \brief Adds the particle to a bucket in a hash table.
   *
   * \param [in, out] _hashTable HashTable to add the particle to.
   */
  void AddToHashTable(SpatialHash<Particle*> &_hashTable);

  /**
   * \brief Checks collision for a plane.
   *
   * \param [in]  _position Position of the plane.
   * \param [in]  _normal   Perpendicular direction of the plane.
   * \param [out] _data     Output collision information.
   * \return Returns true if there is a collision.
   */
  bool CheckCollision(const Vector2 &_position, const Vector2 &_normal, CollisionData &_data);

  /**
   * \brief Change the position and velocity of the particle from a collision with a static object.
   *
   * \param [in] _data Collision data to react from.
   */
  void CollisionResponse(const CollisionData &_data);

  /**
   * \brief Checks and resolves collision against planes.
   *
   * \param [in] _position Position of the plane.
   * \param [in] _normal   Perpendicular direction of the plane.
   */
  void Collision(const Vector2 &_position, const Vector2 &_normal);

  /**
   * \brief Draw the particle.
   *
   * \param [in, out] _renderer Renderer to draw to.
   */
  void Render(Renderer &_renderer);

  /**
   * \brief Set the size of the particle.
   *
   * \param [in] _radius Half size of the particle.
   */
  void SetRadius(float _radius);

public:
  /**
   * \brief Check if two particles are colliding (overlapping).
   *
   * \param [in]  _p1
   * \param [in]  _p2
   * \param [out] _data Output collision information.
   * \return Returns true if there is a collision.
   */
  static bool CheckCollision(const Particle &_p1, const Particle &_p2, CollisionData &_data);

  /**
   * \brief Update the position and velocities of particles from a collision. 
   *
   * \param [in] _p1    
   * \param [in] _p2   
   * \param [in] _data Collision data.
   */
  static void CollisionResponse(Particle &_p1, Particle &_p2, const CollisionData &_data);

  friend bool operator==(const Particle &_lhs, const Particle &_rhs); // get access to id
};

/**
 * \brief Checks if the particles are the same
 * compares the id of each particle.
 *
 * \param [in] _lhs Particle 1.
 * \param [in] _rhs Particle 2.
 * \return Returns true if they are the same.
 */
bool operator==(const Particle &_lhs, const Particle &_rhs);

#endif //_PARTICLE_H_
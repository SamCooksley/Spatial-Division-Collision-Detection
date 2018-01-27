#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <memory>

class Texture;
class Renderer;

/**
 * \brief Draws a texture at a specific size.
 */

class Sprite
{
 public:
  Sprite(); //!< Constructor.

  /**
   * \brief Constructor.
   * \param [in] _texture Texture to display.
   */
  Sprite(const std::shared_ptr<Texture> &_texture);
  ~Sprite(); //!< Destructor.

  /**
   * \brief Initialise
   *
   * \param [in] _texture      Set the texture to use.
   * \param [in] _width        Width of the sprite.
   * \param [in] _height       Height of the sprite.
   */
  void Init(const std::shared_ptr<Texture>& _texture, int _width, int _height); 

  void Draw(Renderer& _renderer, int _x, int _y); //!< Draw the sprite.

  void SetSize(int _width, int _height); //!< Set the size of the sprite.

  int GetWidth() const; //!< Get the width.
  int GetHeight() const; //!< Get the height.

 private:
  std::shared_ptr<Texture> m_texture; //!< Texture to use.

  int m_width;  //!< Output width.
  int m_height; //!< Output height.
};

#endif //_SPRITE_H_
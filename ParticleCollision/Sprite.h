#ifndef _SPRITE_H_
#define _SPRITE_H_

/**
 * \brief Draws a texture at a specific size.
 */

#include <memory>

class Texture;
class Renderer;

class Sprite
{
private:
  std::shared_ptr<Texture> m_texture; //!< Texture to use.

  int m_width;  //!< Output width.
  int m_height; //!< Output height.

public:
  Sprite(void); //!< Constructor.

  /**
   * \brief Constructor.
   * \param [in] _texture Texture to display.
   */
  Sprite(const std::shared_ptr<Texture> &_texture);

  ~Sprite(void); //!< Destructor.

  /**
   * \brief Initialise
   *
   * \param [in] _texture      Set the texture to use.
   * \param [in] _width        Width of the sprite.
   * \param [in] _height       Height of the sprite.
   */
  void Init(const std::shared_ptr<Texture> &_texture, int _width, int _height); 

  void Draw(Renderer &_renderer, int _x, int _y); //!< Draw the sprite.

  void SetSize(int _width, int _height) { m_width = _width; m_height = _height; } //!< Set the size of the sprite.

  int GetWidth(void) const { return m_width;   } //!< Get the width.
  int GetHeight(void) const { return m_height; } //!< Get the height.
};

#endif //_SPRITE_H_
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <string>
#include <SDL_image.h>

class Font;
class Renderer;

/**
 * \brief Wrapper class for SDL_Texture.
 */

class Texture 
{
 public:
  Texture(); //!< Constructor.
  ~Texture(); //!< Destructor.
  
  void Destroy(); //!< Free the texture.

  /**
   * \brief Create a blank texture.
   * \param [in] Renderer Constrains for the texture.
   * \param [in] _width   The width of the texture.
   * \param [in] _height  The height of the texture.
   * \param [in] _format  Texture format
   * \param [in] _access  Usage of the texture.
   * \return Returns true if texture was successfully created.
   */
  bool Create(Renderer& _renderer, int _width, int _height, int _format = SDL_PIXELFORMAT_RGBA8888, int _access = SDL_TEXTUREACCESS_STREAMING);

  /**
   * \brief Load a texture from a file.
   * \param [in] _renderer Constrains for the texture.
   * \param [in] _filename Path where the image is located.
   * \return Returns true if texture was successfully created.
   */
  bool Load(Renderer& _renderer, const std::string& _filename);
  
  /**
   * \brief Create a texture from text.
   * \param [in] _renderer Constrains for the texture.
   * \param [in] _font     Font of the text.
   * \param [in] _text     Text to write.
   * \param [in] _colour   Colour of the text.
   * \return Returns true if texture was successfully created.
   */
  bool CreateFromText(
    Renderer& _renderer,
    const Font& _font, 
    const std::string& _text,
    const SDL_Color& _colour
  );

  /**
   * \brief Draw the texture to a renderer.
   * \param [in] _renderer The renderer to draw to.
   * \param [in] _src      The portion of the texture to draw.
   * \param [in] _dst      The destination position and size
   * \param [in] _angle    The angle to draw the texture at.
   * \param [in] _centre   The pivot point of the angle
   * \param [in] _flip     Should be texture be flipped?
  */
  void Draw(
    Renderer& _renderer,
    SDL_Rect* _src = nullptr, 
    SDL_Rect* _dst = nullptr, 
    float _angle = 0.0f, 
    SDL_Point* _centre = nullptr,
    SDL_RendererFlip _flip = SDL_FLIP_NONE
  );
  
  /**
   * \brief Draw the texture to the renderer.
   * \param [in] _renderer The renderer to draw to.
   * \param [in] _x        The destination x position.
   * \param [in] _y        The destination y position.
   */
  void Draw(Renderer& _renderer, int _x, int _y);

  int GetWidth() const;  //!< Get the width of the texture.
  int GetHeight() const; //!< Get the height of the texture.

 private:
  SDL_Texture* m_texture; //!< Texture.

  int m_width;  //!< Width of the texture.
  int m_height; //!< Height of the texture.
};

#endif //_TEXTURE_H_
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "SDL.h"

class Window;

/**
 * \brief Container class for a renderer.
 *
 * Encapsulates the SDL_Renderer class for
 * increased usability.
 */

class Renderer
{
 public:
  Renderer();  //!< Constructor.
  ~Renderer(); //!< Destructor.

  /**
   * \brief Creates a renderer.
   *
   * \param [in] _window Window to draw to.
   * \return Returns true if the renderer was succesfully created.
   */
  bool Create(const Window& _window);

  void Destroy(); //!< Destroy the renderer.

  void Clear();  //!< Clear the renderer to the clear colour.
  void Render(); //!< Draws the renderer to the window.

  /**
   * \brief Sets the colour to clear the renderer.
   *
   * \param [in] _r The red component.
   * \param [in] _g The greed component.
   * \param [in] _b The blue component.
   */
  void SetClearColour(Uint8 _r, Uint8 _g, Uint8 _b);

  /**
   * \brief Sets the drawing colour of the renderer.
   *
   * \param [in] _r The red component.
   * \param [in] _g The greed component.
   * \param [in] _b The blue component.
   * \param [in] _a The alpha component. Default 255.
   */
  void SetRenderColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a = 255);

  SDL_Renderer* Get() const; //!< Get the SDL renderer.

 private:
  SDL_Renderer* m_renderer; //!< Renderer.
  SDL_Color m_clearColour;  //!< Colour to clear the screen to.
};

#endif //_RENDERER_H_
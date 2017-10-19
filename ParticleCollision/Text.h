#ifndef _TEXT_H_
#define _TEXT_H_

/*
 * \brief Class to draw text.
 */

#include <string>
#include <memory>

#include <SDL.h>

#include "Font.h"
#include "Texture.h"

class Text 
{
private:
  std::shared_ptr<Font> m_font; //!< Font to draw the text
  std::string m_text; //!< Text to write.
  SDL_Color m_colour; //!< Colour of the text.

  Texture m_texture; //!< Output texture of the text.
 
  bool m_dirty; //!< Should the texture be updated?

public:
  Text(void);  //!< Constructor.
  ~Text(void); //!< Destructor.

  void Draw(Renderer &_renderer, int _x, int _y); //!< Draw the text.

  void SetFont(const std::shared_ptr<Font> &_font) { m_font = _font; m_dirty = true; } //!< Set the font to use.

  void SetText(const std::string &_text) { m_text = _text; m_dirty = true; } //!< Set the text to write.
  
  void SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a = 255); //!< Set the colour of the text.

  int GetWidth (void) const { return m_texture.GetWidth();  } //!< Get the width of the outputted text.
  int GetHeight(void) const { return m_texture.GetHeight(); } //!< Get the height of the outputted text. 

private:
  void UpdateTexture(Renderer &_renderer); //!< Update the texture with the new data.
};

#endif _TEXT_H_
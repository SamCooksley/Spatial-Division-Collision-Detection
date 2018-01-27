#ifndef _TEXT_H_
#define _TEXT_H_

#include <string>
#include <memory>

#include <SDL.h>

#include "Font.h"
#include "Texture.h"

/*
 * \brief Class to draw text.
 */

class Text 
{
 public:
  Text();  //!< Constructor.
  ~Text(); //!< Destructor.

  void Draw(Renderer &_renderer, int _x, int _y); //!< Draw the text.

  void SetFont(const std::shared_ptr<Font>& _font); //!< Set the font to use.

  void SetText(const std::string& _text); //!< Set the text to write.
  
  void SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a = 255); //!< Set the colour of the text.

  int GetWidth() const; //!< Get the width of the outputted text.
  int GetHeight() const; //!< Get the height of the outputted text. 

 private:
  void UpdateTexture(Renderer& _renderer); //!< Update the texture with the new data.

  std::shared_ptr<Font> m_font; //!< Font to draw the text
  std::string m_text; //!< Text to write.
  SDL_Color m_colour; //!< Colour of the text.

  Texture m_texture; //!< Output texture of the text.

  bool m_dirty; //!< Should the texture be updated?

};

#endif _TEXT_H_
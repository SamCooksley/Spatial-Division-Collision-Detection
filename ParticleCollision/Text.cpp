#include "Text.h"

Text::Text(void)
{
  m_dirty = true;
  m_colour.r = m_colour.g = m_colour.b = 0;
  m_colour.a = 1;
}

Text::~Text(void)
{
}

void Text::UpdateTexture(Renderer &_renderer)
{
  //if there is no font or text, create a blank texture
  if (!m_font || m_text.length() == 0)
  { 
    m_texture.Create(_renderer, 0, 0, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC);
  }
  else
  {
    m_texture.CreateFromText(_renderer, *m_font.get(), m_text, m_colour);
  }

  //say that it has been updated
  m_dirty = false;
}

void Text::Draw(Renderer &_renderer, int _x, int _y)
{
  //update the texture if the data has been changed.
  if (m_dirty) { UpdateTexture(_renderer); }

  //draw the text
  m_texture.Draw(_renderer, _x, _y);
}

void Text::SetColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a)
{
  m_colour.r = _r, m_colour.g = _g, m_colour.b = _b;
  m_colour.a = _a;
  m_dirty = true;
}
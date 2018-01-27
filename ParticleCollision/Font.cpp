#include "Font.h"

Font::Font() :
  m_font(nullptr)
{ }

Font::~Font()
{
  Destroy();
}

void Font::Destroy()
{
  //if there is a font, delete it.
  if (m_font != nullptr)
  {
    TTF_CloseFont(m_font);
    m_font = nullptr;
  }
}

bool Font::Load(const std::string &_filename, int _size)
{
  Destroy(); //Destroy any existing font

  m_font = TTF_OpenFont(_filename.c_str(), _size); //load the font
  return m_font != nullptr; //!< return true if there is a font.
}

int Font::Height() const
{
  return TTF_FontLineSkip(m_font);
}

TTF_Font *Font::Get() const
{
  return m_font;
}
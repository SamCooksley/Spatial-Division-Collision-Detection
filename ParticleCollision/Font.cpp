#include "Font.h"

Font::Font(void)
{
  m_font = nullptr;
}

Font::~Font(void)
{
  Destroy();
}

void Font::Destroy(void)
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
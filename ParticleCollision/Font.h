#ifndef _FONT_H_
#define _FONT_H_

/**
 * \brief Wrapper class for SDL_TTF font.
 */

#include <string>

#include <SDL_ttf.h>

class Font
{
private:
  TTF_Font *m_font; //!< Font.

public:
  Font(void);  //!< Constructor.
  ~Font(void); //!< Destructor.

  void Destroy(void); //!< Delete the font.

  /**
   * \brief Load font from file.
   *
   * \param [in] _filename Path of the font.
   * \param [in] _size     Size of the font.
   */
  bool Load(const std::string &_filename, int _size);

  int Height(void) const { return TTF_FontLineSkip(m_font); }

  TTF_Font *Get(void) { return m_font; } //!< Get the SDL_TTF font.
};

#endif //_FONT_H_
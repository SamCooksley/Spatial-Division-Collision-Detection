#ifndef _FONT_H_
#define _FONT_H_

#include <string>

#include "SDL_ttf.h"

/**
 * \brief Wrapper class for SDL_TTF font.
 */

class Font
{
 public:
  Font();  //!< Constructor.
  ~Font(); //!< Destructor.

  void Destroy(); //!< Delete the font.

  /**
   * \brief Load font from file.
   *
   * \param [in] _filename Path of the font.
   * \param [in] _size     Size of the font.
   */
  bool Load(const std::string &_filename, int _size);

  int Height() const; //!< Get the font height.

  TTF_Font *Get() const; //!< Get the SDL font.

 private:
  TTF_Font *m_font; //!< Font.
};

#endif //_FONT_H_
#ifndef _WINDOW_H_
#define _WINDOW_H_

/** 
 * \brief Container class for a window.
 *
 * Encapsulates the SDL_Window class for
 * increased usability. 
 */

struct SDL_Window;
struct SDL_WindowEvent;

class Window
{
private:
  SDL_Window *m_window; //!< Window.
  unsigned int m_id;

  int m_width;
  int m_height;

public:
  Window(void);  //!< Constructor.
  ~Window(void); //!< Destructor.

  /**
   * \brief Creates the window.
   *
   * \param [in] _title  Title of the window.
   * \param [in] _width  The width (size in x) of the window.
   * \param [in] _height The height (size in y) of the window.
   * \return Returns true if the window was successfully created.
   */
  bool Create(const char *_title, int _width, int _height);

  void Destroy(void); //!< Destroy the window.

  void HandleEvent(const SDL_WindowEvent &_event); //1< Handle any events related to the window.

  int GetWidth (void) const { return m_width;  } //!< Get the width of the window.
  int GetHeight(void) const { return m_height; } //!< Get the height of the window.

  SDL_Window *Get(void) const { return m_window; } //!< Get the SDL window.
};

#endif //_WINDOW_H_
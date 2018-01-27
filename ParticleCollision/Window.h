#ifndef _WINDOW_H_
#define _WINDOW_H_


struct SDL_Window;
struct SDL_WindowEvent;

/**
 * \brief Container class for a window.
 *
 * Encapsulates the SDL_Window class for
 * increased usability.
 */

class Window
{
 public:
  Window();  //!< Constructor.
  ~Window(); //!< Destructor.

  /**
   * \brief Creates the window.
   *
   * \param [in] _title  Title of the window.
   * \param [in] _width  The width (size in x) of the window.
   * \param [in] _height The height (size in y) of the window.
   * \return Returns true if the window was successfully created.
   */
  bool Create(const char *_title, int _width, int _height);

  void Destroy(); //!< Destroy the window.

  void HandleEvent(const SDL_WindowEvent& _event); //!< Handle any events related to the window.

  int GetWidth() const; //!< Get the width of the window.
  int GetHeight() const; //!< Get the height of the window.

  SDL_Window* Get() const; //!< Get the SDL window.

 private:
  SDL_Window *m_window; //!< Window.
  unsigned int m_id; //!< Window id.

  int m_width; //!< Window width.
  int m_height; //!< Window height.
};

#endif //_WINDOW_H_
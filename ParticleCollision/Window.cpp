#include "Window.h"

#include <SDL.h>

Window::Window(void)
{
  //state that there is no window.
  m_window = nullptr;

  m_width = m_height = 0;
}

Window::~Window(void)
{
  //make sure the window is destroyed when
  //the container is deleted.
  Destroy();
}

bool Window::Create(const char *_title, int _width, int _height)
{
  //Create the window
  m_window = SDL_CreateWindow(_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_SHOWN);
  //if the window failed to be created, display an error message.
  if (m_window == nullptr)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to create Window!", SDL_GetError(), m_window);
  }

  //get the id of the window
  m_id = SDL_GetWindowID(m_window);

  //set the size of the window
  m_width = _width;
  m_height = _height;

  //return true if a window exists.
  return m_window != nullptr;
}

void Window::Destroy(void)
{
  //if a window exists, destroy it
  if (m_window != nullptr)
  {
    SDL_DestroyWindow(m_window);
    //reset the data
    m_window = nullptr;
    m_id = 0;
    m_width = m_height = 0;
  }
}

void Window::HandleEvent(const SDL_WindowEvent &_event)
{
  //only act to events related to this window
  //if its not the same window, exit
  if (_event.windowID != m_id) { return; }

  switch (_event.event)
  {
    //if the window has been resized, update the width and height
    case SDL_WINDOWEVENT_RESIZED:
    {
      m_width = _event.data1;
      m_height = _event.data2;
      break;
    }
  }
}
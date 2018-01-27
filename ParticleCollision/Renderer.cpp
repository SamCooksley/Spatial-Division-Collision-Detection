#include "Renderer.h"

#include "Window.h"

Renderer::Renderer() :
  m_renderer(nullptr)
{
  //Start with the render clear colour as black.
  SetClearColour(0, 0, 0);
  m_clearColour.a = 0;
}

Renderer::~Renderer()
{
  //make sure the renderer is destroyed when
  //the container is deleted.
  Destroy();
}

bool Renderer::Create(const Window& _window)
{
  //create the renderer
  m_renderer = SDL_CreateRenderer(_window.Get(), -1, SDL_RENDERER_ACCELERATED);
  //if the window failed to be created, display an error message.
  if (m_renderer == nullptr)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to create Renderer!", SDL_GetError(), _window.Get());
  }

  //return true if the renderer exists
  return m_renderer != nullptr;
}

void Renderer::Destroy()
{
  //if a renderer exists, destroy it
  if (m_renderer != nullptr)
  {
    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
  }
}

void Renderer::Clear()
{
  //set the draw colour to the clear colour
  SetRenderColour(m_clearColour.r, m_clearColour.g, m_clearColour.b);
  //clear the renderer in the clear colour.
  SDL_RenderClear(m_renderer);
}

void Renderer::Render()
{
  //draw the renderer to the screen.
  SDL_RenderPresent(m_renderer);
}

void Renderer::SetClearColour(Uint8 _r, Uint8 _g, Uint8 _b)
{
  //assign the colour
  m_clearColour.r = _r, m_clearColour.g = _g, m_clearColour.b = _b;
}

void Renderer::SetRenderColour(Uint8 _r, Uint8 _g, Uint8 _b, Uint8 _a)
{
  //set the draw colour of the renderer.
  SDL_SetRenderDrawColor(m_renderer, _r, _g, _b, _a);
}

SDL_Renderer* Renderer::Get() const
{
  return m_renderer;
}
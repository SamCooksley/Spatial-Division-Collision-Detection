#include "Texture.h"

#include <SDL_ttf.h>

#include "Renderer.h"
#include "Font.h"

Texture::Texture() :
  m_texture(nullptr),
  m_width(0), m_height(0)
{ }

Texture::~Texture()
{
  Destroy();
}

void Texture::Destroy()
{
  //if a texture exists, destroy it
  if (m_texture != nullptr)
  {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr; 
  }
  //reset the size
  m_width = m_height = 0;
}

bool Texture::Create(Renderer& _renderer, int _width, int _height, int _format, int _access)
{
  Destroy(); //destroy any existing texture

  m_width = _width;
  m_height = _height;
  //create the texture
  m_texture = SDL_CreateTexture(_renderer.Get(), _format, _access, m_width, m_height);
  return m_texture != nullptr;
}

bool Texture::Load(Renderer& _renderer, const std::string& _filename)
{
  Destroy(); //destroy any existing texture
  
  //load the image
  SDL_Surface *surface = IMG_Load(_filename.c_str());
  
  if (surface == nullptr)
  {
    printf("Unabled to load image %s. SDL_image Error: %s\n", _filename.c_str(), IMG_GetError());
    return false;
  }

  //create a texture
  m_texture = SDL_CreateTextureFromSurface(_renderer.Get(), surface);
  
  if (m_texture == nullptr)
  {
    printf("Unable to create texture from %s. SDL Error: %s\n", _filename.c_str(), SDL_GetError());
    SDL_FreeSurface (surface);
    return false;
  }

  m_width = surface->w;
  m_height = surface->h;
  
  //destroy the surface
  SDL_FreeSurface (surface);
  
  return m_texture != nullptr;
}

bool Texture::CreateFromText(Renderer& _renderer, const Font& _font, const std::string& _text, const SDL_Color& _colour)
{
  Destroy(); //destroy any existing texture
 
  //create surface from text
  SDL_Surface* surface = TTF_RenderText_Blended(_font.Get(), _text.c_str(), _colour);

  if (surface == nullptr)
  {
    printf("Unable to render text surface. SDL_ttf Error: %s\n", TTF_GetError());
    return false;
  }

  //create texture
  m_texture = SDL_CreateTextureFromSurface(_renderer.Get(), surface);
  
  if (m_texture == nullptr)
  {
    printf("Unable to create texture from rendered text. SDL Error: %s\n", SDL_GetError());
  }
  else
  {
    m_width = surface->w;
    m_height = surface->h;
  }
  
  //destroy surface
  SDL_FreeSurface (surface);
  
  return m_texture != nullptr;
}

void Texture::Draw(Renderer &_renderer, SDL_Rect *_src, SDL_Rect *_dst, float _angle, SDL_Point *_centre, SDL_RendererFlip _flip)
{
  SDL_RenderCopyEx(_renderer.Get(), m_texture, _src, _dst, _angle, _centre, _flip);
}

void Texture::Draw(Renderer &_renderer, int _x, int _y)
{
  //draw in the size of image 
  SDL_Rect dest = { _x, _y, m_width, m_height };
  Draw(_renderer, nullptr, &dest);
}

int Texture::GetWidth() const
{
  return m_width;
}

int Texture::GetHeight() const
{
  return m_height;
}
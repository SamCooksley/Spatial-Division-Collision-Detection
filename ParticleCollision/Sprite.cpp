#include "Sprite.h"

#include "Renderer.h"
#include "Texture.h"

Sprite::Sprite(void)
{
  m_width = m_height = 0;
}

Sprite::Sprite(const std::shared_ptr<Texture> &_texture) : m_texture(_texture)
{
  //set the size to the image size.
  m_width = _texture->GetWidth();
  m_height = _texture->GetHeight();
}

Sprite::~Sprite(void)
{
}

void Sprite::Init(const std::shared_ptr<Texture> &_texture, int _width, int _height)
{
  m_texture = _texture;
  m_width = _width;
  m_height = _height;
}

void Sprite::Draw(Renderer &_renderer, int _x, int _y)
{
  //only draw if there is a texture
  if (m_texture)
  {
    //centre the position
    SDL_Rect dst = { _x - m_width / 2, _y - m_height / 2, 
                     m_width,          m_height         };
    //draw the object
    m_texture->Draw(_renderer, nullptr, &dst);
  }
}
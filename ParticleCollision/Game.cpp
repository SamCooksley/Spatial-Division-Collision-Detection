#include "Game.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <sstream>
#include <time.h>

#include "Texture.h"
#include "Font.h"
#include "Sprite.h"
#include "Maths.h"

#include "Timer.h"

int Game::Run()
{
  bool success = Init();

  if (success)
  {
    Loop();
  }

  Exit();

  return success ? 0 : -1;
}

bool Game::Init()
{
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to initialise SDL!", SDL_GetError(), nullptr);
    return false;
  }

  if (m_window.Create("Particle Collision", 800, 600) == false)
  {
    return false;
  }

  if (m_renderer.Create(m_window) == false)
  {
    return false;
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
  {
    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    return false;
  }

  if (TTF_Init() < 0)
  {
    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    return false;
  } 

  m_renderer.SetClearColour(240, 240, 240);

  auto font = std::make_shared<Font>();
  font->Load("resources/fonts/arial.ttf", 24);

  m_profiler = std::make_unique<Profiler>(font);

  m_spawnRect = Rect(100, 100, 700, 500);
  
  AddPlane(Vector2(100, 600/2), Vector2( 1.0f,  0.0f), 400);
  AddPlane(Vector2(700, 600/2), Vector2(-1.0f,  0.0f), 400);
  AddPlane(Vector2(800/2, 100), Vector2( 0.0f,  1.0f), 600);
  AddPlane(Vector2(800/2, 500), Vector2( 0.0f, -1.0f), 600);

  //AddPolygons(50);
  AddCircles(200);

  m_current = &m_brute;

  return true;
}

void Game::Loop()
{
  m_deltaTime = 0;
  m_done = false;

  Timer timer;

  //repeat until the program should end
  while (!m_done)
  {
    //calculate delta time
    m_deltaTime = timer.Seconds();
    timer.Reset();

    HandleEvents();
    Update();
    Render();
  }
}

void Game::Exit()
{
  m_profiler.release();

  //destroy the window and renderer
  m_renderer.Destroy();
  m_window.Destroy();

  //quit sdl
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

void Game::HandleEvents()
{
  SDL_Event e;
  //loop through all the events
  //repeat until there are no more events
  while( SDL_PollEvent( &e ) != 0 ) 
  {
    switch (e.type)
    {
      //If it was a window event, send it to the window
      case SDL_WINDOWEVENT: { m_window.HandleEvent(e.window); break; }
      //If the close button has been pressed, end the program
      case SDL_QUIT:        { Quit();                         break; } 
      //if a key has been released
      case SDL_KEYUP:
      {
        //get the key name
        switch (e.key.keysym.scancode)
        {
          case SDL_SCANCODE_ESCAPE: { Quit(); break; }
          case SDL_SCANCODE_1: { m_current = &m_brute; break; }
          case SDL_SCANCODE_2: { m_current = &m_quad;  break; }
          case SDL_SCANCODE_3: { m_current = &m_aabb;  break; }
          case SDL_SCANCODE_R: { m_profiler->Reset(); break; }
        }
        break;
      }
    }
  }
}

void Game::Update()
{
  m_profiler->Update(m_deltaTime);

  m_current->Reset();

  //update all the particles
  for (auto& c : m_colliders)
  {
    //move the particle
    c->Update(m_deltaTime);
    m_current->Insert(c);
  }

  m_current->Collide();
}

void Game::Render()
{
  //clear the renderer
  m_renderer.Clear();
  
  m_renderer.SetRenderColour(255, 0, 0);

  m_current->Draw(m_renderer);

  m_renderer.SetRenderColour(0, 0, 0);

  //Draw all the particles.
  for (auto& c : m_colliders)
  {
    c->Draw(m_renderer);
  }

  m_profiler->Render(m_renderer);

  //Display to the window.
  m_renderer.Render(); 
}

void Game::Quit()
{
  //stop repeating the main loop.
  m_done = true;
}

void Game::AddPolygons(int _count)
{
  int width = static_cast<int>(m_spawnRect.Width());
  int height = static_cast<int>(m_spawnRect.Height());

  //create _count polygons.
  for (int i = 0; i < _count; ++i)
  {
    //get a random position inside the the spawn rect.
    Vector2 pos(
      m_spawnRect.min.x + rand() % width,
      m_spawnRect.min.y + rand() % height
    );
    Vector2 vel(rand() % 20 / 5.0f - 2, rand() % 20 / 5.0f - 2); 
    vel *= 10.0f;
    auto poly = std::make_shared<Polygon>(pos, vel);
    
    m_colliders.emplace_back(poly);  
    m_aabb.Add(poly);
  }
}

void Game::AddCircles(int _count)
{
  int width = static_cast<int>(m_spawnRect.Width());
  int height = static_cast<int>(m_spawnRect.Height());

  //create _count circles.
  for (int i = 0; i < _count; ++i)
  {
    //get a random position inside the spawn rect.
    Vector2 pos(
      m_spawnRect.min.x + rand() % width,
      m_spawnRect.min.y + rand() % height
    );
    Vector2 vel(rand() % 20 / 5.0f - 2, rand() % 20 / 5.0f - 2); 
    vel *= 30.0f;
    auto circle = std::make_shared<Circle>(pos, vel, 2.f + rand() % 10);
    
    m_colliders.emplace_back(circle);
    m_aabb.Add(circle);
  }
}

void Game::AddPlane(const Vector2 &_position, const Vector2 &_normal, float _width)
{
  auto p = std::make_shared<Plane>(_position, _normal, _width);
  m_colliders.emplace_back(p);
  m_aabb.Add(p);
}
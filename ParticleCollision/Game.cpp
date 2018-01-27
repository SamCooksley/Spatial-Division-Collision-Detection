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

	m_spawnRect = Rect(100, 100, 700, 500);
  
  AddPlane(Vector2(100, 600/2), Vector2( 1.0f,  0.0f), 400);
  AddPlane(Vector2(700, 600/2), Vector2(-1.0f,  0.0f), 400);
  AddPlane(Vector2(800/2, 100), Vector2( 0.0f,  1.0f), 600);
  AddPlane(Vector2(800/2, 500), Vector2( 0.0f, -1.0f), 600);

  AddPolygons(2);
  AddCircles(50);


	m_collisionManager.SetDivisionType(CollisionManager::DivisionType::AABB);
  return true;
}

void Game::Loop()
{
  //store the time of the last frame
  Uint32 prevTime = SDL_GetTicks();

  //store the display fps of last frame
  float prevFPS = 1;

  m_deltaTime = 0;
  m_done = false;

  //repeat until the program should end
  while (!m_done)
  {
    //calculate delta time
    //get the current time
    Uint32 time = SDL_GetTicks();
    //get the time difference from the previous frame and convert to seconds
    m_deltaTime = (time - prevTime) / 1000.0f;
    //prevent division by 0
    m_deltaTime = (m_deltaTime <= 0.0f? 0.001f: m_deltaTime); 
    prevTime = time; //update the previous time

    HandleEvents();
    Update();
    Render();
  }
}

void Game::Exit()
{
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
          case SDL_SCANCODE_ESCAPE: { Quit();                  break; }
					case SDL_SCANCODE_1: { m_collisionManager.SetDivisionType(CollisionManager::DivisionType::NONE); break; }
					case SDL_SCANCODE_2: { m_collisionManager.SetDivisionType(CollisionManager::DivisionType::QUAD); break; }
					case SDL_SCANCODE_3: { m_collisionManager.SetDivisionType(CollisionManager::DivisionType::AABB); break; }
        }
        break;
      }
    }
  }
}

void Game::Update()
{
	QuadTree::QuadTree &tree = m_collisionManager.GetTree();
	tree.Reset();

	AABBTree::AABBTree &aabbtree = m_collisionManager.GetAABBTree();
	//aabbtree.Reset();

  //update all the particles
  for (auto& c : m_colliders)
  {
    //move the particle
    c->Update(m_deltaTime);
		if (m_collisionManager.GetDivisionType() == CollisionManager::DivisionType::QUAD)
		{
			tree.Insert(c->AsItem());
		}
  }

	if (m_collisionManager.GetDivisionType() == CollisionManager::DivisionType::AABB)
	{
		aabbtree.Update();
	}

  m_collisionManager.Collide();
}

void Game::Render()
{
  //clear the renderer
  m_renderer.Clear();
	
  m_collisionManager.Draw(m_renderer);

  m_renderer.SetRenderColour(0, 0, 0);

  //Draw all the particles.
  for (auto& c : m_colliders)
  {
    c->Draw(m_renderer);
  }
	
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
    std::shared_ptr<Polygon> poly = std::make_shared<Polygon>(pos, vel);
    
    m_colliders.emplace_back(poly);   
    m_collisionManager.Add(poly);
    //add to the broad-phase collision AABB tree.
    m_collisionManager.GetAABBTree().Insert(poly->AsAABBItem());
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
    std::shared_ptr<Circle> circle = std::make_shared<Circle>(pos, vel, 2 + rand() % 1);
    
    m_colliders.emplace_back(circle);
    m_collisionManager.Add(circle);
    //add to the broad-phase collision AABB tree.
    m_collisionManager.GetAABBTree().Insert(circle->AsAABBItem());
  }
}

void Game::AddPlane(const Vector2 &_position, const Vector2 &_normal, float _width)
{
  auto p = std::make_shared<Plane>(_position, _normal, _width);
  m_colliders.emplace_back(p);
  m_collisionManager.Add(p);
  //add to the broad-phase collision AABB tree.
  m_collisionManager.GetAABBTree().Insert(p->AsAABBItem());
}
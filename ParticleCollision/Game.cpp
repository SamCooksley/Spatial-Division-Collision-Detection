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

Game::Game(void)
{
  done = false; 
  deltaTime = 0;
}

Game::~Game(void)
{
}

int Game::Run(void)
{
  bool success = Init();

  if (success)
  {
    Loop();
  }

  Exit();

  return (success? 0 : -1);
}

bool Game::Init(void)
{
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to initialise SDL!", SDL_GetError(), nullptr);
    return false;
  }

  if (window.Create("Particle Collision", 800, 600) == false)
  {
    return false;
  }

  if (renderer.Create(window) == false)
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

  renderer.SetClearColour(240, 240, 240);

	m_spawnRect = Rect(100, 100, 700, 500);

  AddPlane(Vector2(100, 600/2), Vector2( 1.0f,  0.0f), 400);
  AddPlane(Vector2(700, 600/2), Vector2(-1.0f,  0.0f), 400);
  AddPlane(Vector2(800/2, 100), Vector2( 0.0f,  1.0f), 600);
  AddPlane(Vector2(800/2, 500), Vector2( 0.0f, -1.0f), 600);

  AddPolygons(2);
  AddCircles(50);
	collisionManager.SetDivisionType(CollisionManager::DivisionType::AABB);
  return true;
}

void Game::Loop(void)
{
  //store the time of the last frame
  Uint32 prevTime = SDL_GetTicks();

  //store the display fps of last frame
  float prevFPS = 1;

  //repeat until the program should end
  while (!done)
  {
    //calculate delta time
    //get the current time
    Uint32 time = SDL_GetTicks();
    //get the time difference from the previous frame and convert to seconds
    deltaTime = (time - prevTime) / 1000.0f;
    //prevent division by 0
    deltaTime = (deltaTime <= 0.0f? 0.001f: deltaTime); 
    prevTime = time; //update the previous time

    HandleEvents();
    Update();
    Render();
  }
}

void Game::Exit(void)
{
  //destroy the window and renderer
  renderer.Destroy();
  window.Destroy();

  //quit sdl
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

void Game::HandleEvents(void)
{
  SDL_Event e;
  //loop through all the events
  //repeat until there are no more events
  while( SDL_PollEvent( &e ) != 0 ) 
  {
    switch (e.type)
    {
      //If it was a window event, send it to the window
      case SDL_WINDOWEVENT: { window.HandleEvent(e.window); break; }
      //If the close button has been pressed, end the program
      case SDL_QUIT:        { Quit();                         break; } 
      //if a key has been released
      case SDL_KEYUP:
      {
        //get the key name
        switch (e.key.keysym.scancode)
        {
          case SDL_SCANCODE_ESCAPE: { Quit();                  break; }
					case SDL_SCANCODE_1: { collisionManager.SetDivisionType(CollisionManager::DivisionType::NONE); break; }
					case SDL_SCANCODE_2: { collisionManager.SetDivisionType(CollisionManager::DivisionType::QUAD); break; }
					case SDL_SCANCODE_3: { collisionManager.SetDivisionType(CollisionManager::DivisionType::AABB); break; }
        }
        break;
      }
    }
  }
}

void Game::Update(void)
{
	QuadTree::QuadTree &tree = collisionManager.GetTree();
	tree.Reset();

	AABBTree::AABBTree &aabbtree = collisionManager.GetAABBTree();
	//aabbtree.Reset();

  //update all the particles
  for (auto& c : colliders)
  {
    //move the particle
    c->Update(deltaTime);
		if (collisionManager.GetDivisionType() == CollisionManager::DivisionType::QUAD)
		{
			tree.Insert(c->AsItem());
		}
  }

	if (collisionManager.GetDivisionType() == CollisionManager::DivisionType::AABB)
	{
		aabbtree.Update();
	}

  collisionManager.Collide();
}

void Game::Render(void)
{
  //clear the renderer
  renderer.Clear(); 
	
	collisionManager.Draw(renderer);

  renderer.SetRenderColour(0, 0, 0);

  //draw all the particles
  for (auto& c : colliders)
  {
    c->Draw(renderer);
  }
	
  renderer.Render(); //Display to the window
}

void Game::AddPolygons(int _count)
{
  for (int i = 0; i < _count; i++)
  {
		Vector2 pos(
			m_spawnRect.min.x + rand() % (int)(m_spawnRect.max.x - m_spawnRect.min.y),
			m_spawnRect.min.y + rand() % (int)(m_spawnRect.max.y - m_spawnRect.min.y)
		);
    Vector2 vel(rand() % 20 / 5.0f - 2, rand() % 20 / 5.0f - 2); 
    vel *= 10.0f;
    std::shared_ptr<Polygon> poly = std::make_shared<Polygon>(pos, vel);
    colliders.emplace_back(poly);
    collisionManager.Add(poly);
		collisionManager.GetAABBTree().Insert(poly->AsAABBItem());
  }
}

void Game::AddCircles(int _count)
{
  for (int i = 0; i < _count; i++)
  {
    Vector2 pos(
			m_spawnRect.min.x + rand() % (int)(m_spawnRect.max.x - m_spawnRect.min.x),
			m_spawnRect.min.y + rand() % (int)(m_spawnRect.max.y - m_spawnRect.min.y)
		);
    Vector2 vel(rand() % 20 / 5.0f - 2, rand() % 20 / 5.0f - 2); 
    vel *= 30.0f;
    std::shared_ptr<Circle> poly = std::make_shared<Circle>(pos, vel, 2 + rand() % 1);
    colliders.emplace_back(poly);
    collisionManager.Add(poly);
		collisionManager.GetAABBTree().Insert(poly->AsAABBItem());
  }
}

void Game::AddPlane(const Vector2 &_position, const Vector2 &_normal, float _width)
{
  auto p = std::make_shared<Plane>(_position, _normal, _width);
  colliders.emplace_back(p);
  collisionManager.Add(p);

	collisionManager.GetAABBTree().Insert(p->AsAABBItem());
}
#include <SDL2/SDL.h>
#include <vector>

#ifndef GAME_MASTER_H
#define GAME_MASTER_H

#ifdef LOGGING_ON
// define a simple loggin macro
#define gm_Log(msg)(printf("[ %s ][%i]: %s\n",__FILE__,__LINE__,msg))
#else
#define gm_Log(msg)()
#endif

// include component classes
// I just prefer to include the cpp files 
// instead of header files
#include "gameObject.cpp"
#include "textureManager.cpp"
#include "sound.cpp"
#include "background.cpp"
#include "dragon.cpp"
#include "missle.cpp" 
#include "explosion.cpp"
#include "fire.cpp"

#define INIT_FLAGS (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 576

// frame rate capping at 60 fps
const int FPS = 60;
const int TICKS_PER_FRAME = 1000/ FPS ;


void gm_init();
void gm_shutdown();
void gm_registerObject(GameObject * object);
void gm_logError(const char * message){cerr << "[Error: "<<message<<"] "<<SDL_GetError()<<endl;}
void gm_handleEvents();
void gm_update();
void gm_render();
void gm_logic();
void gm_fireMissle();
void gm_spitFire();
bool gm_checkCollision(SDL_Rect * missle_rect);

vector<GameObject *>gameObjects;
Missles * missle_handler;
Explosion * explosions[10] = {NULL};
int counter = 0;

Dragon * dragon;
SoundManager sound;

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;
SDL_Event event;

bool running = false;
bool paused = false;



/*
	@gm_shutdown 
		Free memory occupied by game objects

*/
void gm_shutdown()
{


	for(int i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


/*
	@gm_init 
		initialize SDL and create game objects

*/
void gm_init()
{
	// init sdl2
	if(SDL_Init(INIT_FLAGS) < 0)
	{
		gm_logError("Unable to init SDL");
		exit(EXIT_FAILURE);
	}else
	{
		// create the window
		window = SDL_CreateWindow("TEST V-1",0,0,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
		if(!window)
		{
			gm_logError("Unable to create window");
			exit(EXIT_FAILURE);
		}else
		{
			// create the renderer
			renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(!renderer)
			{
				printf("Failed to create renderer\nSDL_Error: %s\n",SDL_GetError());
				exit(EXIT_FAILURE);
			}
		}
	
	}

	// load sound and music files
	sound.loadMusic("main_music","assets/Pacman.wav");
	sound.playMusic("main_music");
	sound.loadSound("explosion","assets/explosion.wav");

	// create the dragon object
	dragon = new Dragon(renderer,"assets/images/dragon.png",8);

	// init background 
	Background * background = new Background(renderer);

	// init the Missles class
	missle_handler =new Missles(renderer);

	// register game objects for rendering
	gm_registerObject(background);
	gm_registerObject(dragon);

	running = true;
	paused = false;

}


inline void gm_registerObject(GameObject * object)
{
	gameObjects.push_back(object);
}


void gm_handleEvents()
{
	// handle keyboard keypress event
	while(SDL_PollEvent(&event) != 0)
	{
		if(event.type == SDL_QUIT)
			running = false;

		if(event.type == SDL_KEYDOWN)
		{
			if(event.key.keysym.sym == SDLK_p)
			{
				paused = !paused;
			}
			if(event.key.keysym.sym == SDLK_SPACE)
			{
				gm_spitFire();
			}
		}

	}
	// get current keyboard state 
	const Uint8 * keyState = SDL_GetKeyboardState(NULL);
	
	if(!paused)
	{
		if(keyState[SDL_SCANCODE_DOWN])
			dragon->move_down();
		if(keyState[SDL_SCANCODE_UP])
			dragon->move_up();
		if(keyState[SDL_SCANCODE_LEFT])
			dragon->move_left();
		if(keyState[SDL_SCANCODE_RIGHT])
			dragon->move_right();
	}
}


void gm_render()
{
	// clear the screen
	SDL_RenderClear(renderer);

	// render registered gameobjects
	for(int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->render();
	}

	// render on screen missles
	missle_handler->render();

	// render explosions if any
	for(int i = 0; i < 10; i++)
	{
		if(explosions[i] != NULL)
		{
			explosions[i]->render();
		}
	}
	// copy buffer to the screen
	SDL_RenderPresent(renderer);
}

void gm_update()
{

	// calling update method for registered objects
	for(int i = 0 ; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update();
	}
	// same here
	missle_handler->update();
	for(int i = 0; i < 10; i++)
	{
		if(explosions[i] != NULL)
		{
			if(explosions[i]->done)
			{
				delete explosions[i];
				explosions[i] = NULL;
			}
			else
				explosions[i]->update();
		}	
	}
}


void gm_logic()
{
	// check for collision and create explosion
	if(counter == 60)
	{
		gm_fireMissle();
		counter = 0;
	}
	else
		counter ++;


	SDL_Rect missle_rect;
	if(gm_checkCollision(&missle_rect))
	{
		for(int i = 0; i < 10; i++)
		{
			if(explosions[i] == NULL)
			{
				// create explosion animation and play sound
				explosions[i]= new Explosion(renderer,missle_rect);
				sound.playSound("explosion");
				break;
			}
		}
	}


}

void gm_fireMissle()
{
	// fire a missle at the dragon
	missle_handler->fire(dragon->getY());
}

void gm_spitFire()
{
	Fire * fire = new Fire(renderer,dragon->getRect());
	gm_registerObject(fire);
}

bool gm_checkCollision(SDL_Rect * missle_rect)
{
	//cout << "Checking collisions"<<endl;
	return missle_handler->check_collision(dragon->getRect(),missle_rect);
}

void gm_mainloop()
{
	// the game loop
	while(running)
	{
		gm_handleEvents();
		if(!paused)
		{
			gm_logic();
			gm_update();
			gm_render();
		}
	}
	gm_shutdown();

}

#endif
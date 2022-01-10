#include "gameObject.cpp"

#ifndef MISSLE_H
#define MISSLE_H
#define MAX_MISSLE_COUNT 5

class Missle : public GameObject
{
public:
	Missle(SDL_Renderer * renderer,int y)
	{
		cout << "[_+_] Firing missle"<<endl;
		// create missle image texture 
		this->texture = new Texture(renderer);	
		this->texture->loadFromFile("assets/images/missle.png");

		// set missle initial position and dimensions
		this->position.x = 800;
		this->position.y = y;
		this->position.w = 54;
		this->position.h = 30;
		this->rect = this->position;
	}

	~Missle()
	{
		delete this->texture;
		this->texture = NULL;
	}
	
	// render to screen
	void render();
	
	// update 
	void update();
	
	SDL_Rect rect;
};

void Missle::render()
{
	this->texture->renderResize(NULL,&this->position,true);
}

void Missle::update()
{
	this->position.x -= 2;
	this->rect = this->position;
}

class Missles
{
public: 
	Missles(SDL_Renderer * renderer);
	~Missles();
	// create a missle instance
	void fire(int y);
	// renderer all missle instances
	void render();
	
	// update
	void update();
	// check missle collision
	bool check_collision(SDL_Rect dragon,SDL_Rect * m);
private:
	Missle * sprites[5] = {NULL};
	SDL_Renderer * renderer;
};

Missles::Missles(SDL_Renderer * renderer)	
{
	// copy the rendering context
	this->renderer = renderer;
}


void Missles::fire(int y)
{

	// search for available missle space 
	// if cell is free
	// create a Missle instance and break;
	// else ignore
	for(int i = 0; i < MAX_MISSLE_COUNT ; i++)
	{
		if(this->sprites[i] == NULL)
		{
			//cout << "Chosen missle sprite id "<<i<<endl;
			this->sprites[i] = new Missle(this->renderer,y+80);
			break;
		}
	}
}

Missles::~Missles()
{
	for(int i = 0 ; i < MAX_MISSLE_COUNT ; i++)
	{
		delete this->sprites[i];
		this->sprites[i] = NULL;
	};
}

void Missles::render()
{
	for(int i = 0 ; i < MAX_MISSLE_COUNT; i++)
	{
		if(this->sprites[i] != NULL)
			this->sprites[i]->render();
	}
}

void Missles::update()
{
	for(int i = 0; i < MAX_MISSLE_COUNT; i++)
	{
		if(this->sprites[i] != NULL)
		{
			this->sprites[i]->update();
			// if the missle leaves the screen destroy it
			if(this->sprites[i]->rect.x <= 0)
			{
				delete this->sprites[i];
				this->sprites[i] = NULL;
			}
		}

	}
}

bool Missles::check_collision(SDL_Rect dragon,SDL_Rect * m)
{
	dragon.y += 30;
	dragon.h -= 30;

	for(int i = 0; i < MAX_MISSLE_COUNT; i++)
	{
		if(this->sprites[i] != NULL)
		{
			if(this->sprites[i]->rect.x >= dragon.x && dragon.x +dragon.h >= this->sprites[i]->rect.x)
			{
				if(this->sprites[i]->rect.y >= dragon.y && dragon.y + dragon.h >= this->sprites[i]->rect.y )
				{
					m->x = this->sprites[i]->rect.x;
					m->y = this->sprites[i]->rect.y;
					delete this->sprites[i];
					this->sprites[i] = NULL;
					return true;
				}
			}
		}
	}
	return false;
}
#endif
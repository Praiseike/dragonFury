#include "gameObject.cpp"

#ifndef BACKGROUND_H
#define BACKGROUND_H

class Background : public GameObject
{
	public:
		Background(SDL_Renderer * renderer);
		~Background();
		void loadImages();
		void update(){}
		void render();

		SDL_Renderer * bRenderer;
		Texture ** textures;
};

Background::~Background()
{
	for(int i = 0 ; i < 2; i++)
	{
		delete this->textures[i];
		this->textures[i] = NULL;
	}
	delete this->textures;
	this->textures = NULL;
}

Background::Background(SDL_Renderer * renderer)
{
	this->bRenderer = renderer; 
	this->loadImages();
}

void Background::loadImages()
{
	this->textures = new Texture *[sizeof(Texture *) * 2]; // create space for 2 images

	this->textures[0] = new Texture(this->bRenderer);
	this->textures[0]->loadFromFile("assets/images/background/sky.png");
	this->textures[1] = new Texture(this->bRenderer);
	this->textures[1]->loadFromFile("assets/images/background/sky.png");


}

void Background::render()
{
	static int offset;
	this->textures[0]->render(offset,0);
	this->textures[1]->render(offset + this->textures[1]->getWidth(),0);
	offset -= 3;
	if(offset < -this->textures[1]->getWidth())
		offset = 0;
}

#endif
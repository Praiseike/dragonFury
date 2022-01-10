#include "gameObject.cpp"

#ifndef DRAGON_H
#define DRAGON_H

/*
	@Animation
		This class handles the loading and rendering of animations 
		from sprite textures
*/

class Animation : public GameObject
{
public:

	Animation(SDL_Renderer * renderer ,const char * filename,int frameCount,int cols = 1,int rows = 1);
	~Animation();
	// render animation frame
	void render();
	// update frame counter
	void update();

protected:
	SDL_Renderer * aRenderer;
	int aFrameCount = 0;
	int frame_width = 0;
	int frame_height = 0;
	float currentFrame = 0;
	int speed;
	SDL_Rect * frames = NULL;
};

Animation::~Animation()
{
	delete this->texture;
	if(frames)
	{
		delete this->frames;
		this->frames = NULL;
	}
	this->texture = NULL;
}

/*
	renderer -> a pointer to the main rendering context
	filename -> path to sprite image file
	frameCount -> Number of frames in the sprite
	cols -> Number of columns
	rows -> Number of rows
*/
Animation::Animation(SDL_Renderer * renderer ,const char * filename, int frameCount,int cols,int rows)
{
	this->aFrameCount = frameCount;
	this->aRenderer = renderer;
	// load sprite texture
	this->texture = new Texture(this->aRenderer);
	this->texture->loadFromFile(filename);

	// allocate memory to store frame dimensions
	this->frames = new SDL_Rect[frameCount ];

	frame_width = this->texture->getWidth() / cols;
	frame_height = this->texture->getHeight() / rows;

	int x = 0;
	int y = 0;
	// autmatically compute frame cells from the texture image
	// e.g

	for(int i = 0; i< frameCount; i++)
	{
		this->frames[i].x = x;
		this->frames[i].y = y;
		this->frames[i].w = frame_width;
		this->frames[i].h = frame_height;
		x += frame_width;

		if((i+1) % cols == 0 && cols > 1)
		{
			x = 0;
			y += frame_height;
		}
	}	
}


void Animation::render()
{
	SDL_Rect clip;

	clip.x = this->frames[(int)this->currentFrame].x;
	clip.y = this->frames[(int)this->currentFrame].y;
	clip.w = this->frame_width;
	clip.h = this->frame_height;
	this->texture->renderResize(&clip,&this->position,false);
}

void Animation::update()
{
	if(this->currentFrame < this->aFrameCount)
		this->currentFrame += 0.2;
	else
		this->currentFrame = 0;
}
#endif
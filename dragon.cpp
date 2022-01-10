#include "animation.cpp"

#ifndef CUSTOM_DRAGON_H
#define CUSTOM_DRAGON_H

/*

	@Dragon
		This is the dragon Player class
		It is also a derived class from the Animation class.
		It handles movement.
*/

class Dragon : public Animation
{
	int screen_x,screen_y;
public:
	Dragon(SDL_Renderer * renderer ,const char * filename, int frameCount) : Animation(renderer,filename,frameCount,4,4)
	{
		cout << "[_+_] Creating dragon "<<endl;
		this->position.x = 100;
		this->position.y = 100;
		this->position.w = this->texture->getWidth() / 4;
		this->position.h = this->texture->getHeight() / 4;
		this->speed = 4;
		// get screen dimensions from the renderer
		SDL_GetRendererOutputSize(renderer,&this->screen_x,&this->screen_y);
	}

	~Dragon()
	{
		cout << "[_-_] Destroying dragon"<<endl;
	}
	inline void move_left();
	inline void move_right();
	inline void move_up();
	inline void move_down();
};


void Dragon::move_down()
{
	if(this->position.y + this->frames[0].h - 50 < this->screen_y)		
		this->position.y += this->speed;
}

void Dragon::move_right()
{
	if(this->position.x + this->frames[0].w< this->screen_x)
		this->position.x += this->speed;
}

void Dragon::move_left()
{
	if(this->position.x > 0)
		this->position.x -= this->speed;
}
void Dragon::move_up()
{
	if(this->position.y > -70)
		this->position.y -= this->speed;
}

#endif
#include "animation.cpp"
#ifndef EXPLOSION_H
#define EXPLOSION_H


/*

	@Explosion
		This class is a derived class from the Animation class
		It creates a custom explosion animation

*/

class Explosion: public Animation
{
public:
	Explosion(SDL_Renderer * renderer,SDL_Rect target) : Animation(renderer,"assets/images/explosion.png",16,16,1) 
	{
		cout << "[_+_] Creating explosion"<<endl;
		this->position.x = target.x;
		this->position.y = target.y;
	}
	void update();
	void render();
	bool done = false;
};

void Explosion::update()
{
	
	if(this->currentFrame < this->aFrameCount)
		this->currentFrame += 0.2;
	else{
		this->done = true;
	}
}

void Explosion::render()
{
	SDL_Rect clip;

	clip.x = this->frames[(int)this->currentFrame].x;
	clip.y = this->frames[(int)this->currentFrame].y;
	clip.w = this->frame_width;
	clip.h = this->frame_height;
	this->texture->render(this->position.x,this->position.y,&clip);
}

#endif // EXPLOSION_H
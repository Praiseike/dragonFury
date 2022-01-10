#include "animation.cpp"

#ifndef DRAGON_BREATH_H
#define DRAGON_BREATH_H

class Fire : public Animation
{

public:
	Fire(SDL_Renderer * renderer,SDL_Rect dragonRect) : Animation(renderer,"assets/images/fire.png",25,5,5)
	{
		this->position.x = dragonRect.x + 140;
		this->position.y = dragonRect.y + 50;
		this->position.w = 100;
		this->position.h = 60;
		this->speed = 4;
	
	}

	void update();
private:

};

void Fire::update()
{
	if(this->currentFrame < this->aFrameCount)
		this->currentFrame += 0.2;
	else{
		this->currentFrame = 0;
	}

	this->position.x += this->speed;

}


#endif //DRAGON_BREATH_H
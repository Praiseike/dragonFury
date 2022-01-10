#include "textureManager.cpp"

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
public:
	GameObject(){}
	// get width and height
	inline int getWidth() { return this->position.w; }
	inline int getHeight() { return this->position.h; }
	
	// get x and y coordinates
	inline int getX() { return this->position.x; }
	inline int getY() { return this->position.y; }
	
	// get bounding rect
	inline SDL_Rect getRect() { return this->position; }
	
	virtual void render(){}
	virtual void move(){}
	virtual void update(){} // _/----\_	
	virtual ~GameObject(){} // (==-0-0) - this was a pain in the ass
							// A virtual destructor
protected:
	Texture * texture;
	SDL_Rect position;
};

#endif
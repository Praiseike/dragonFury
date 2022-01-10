#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


class Texture
{
	public: 
		Texture(){}
		Texture(SDL_Renderer * r);
		Texture(SDL_Renderer * r,const char * font);
		~Texture();
		// load texture from file 
		bool loadFromFile(std::string path );
		
		// deallocate memory
		void free();

		
		// create the image from font string	
		#if defined(SDL_TTF_MAJOR_VERSION)
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor = {0x0,0x0,0x0});
		#endif
		// render to screen
		void render(int x,int y,SDL_Rect * clip = NULL,double angle = 0.0,SDL_Point * center = NULL,SDL_RendererFlip flip = SDL_FLIP_NONE);
		void render(int x, int y,SDL_Rect * dest,SDL_Rect * src);
		void renderResize(SDL_Rect * src,SDL_Rect * dest,bool flip = false);
		// set texture color modulation
		void setColor(Uint8 red,Uint8 green, Uint8 blue);
		
		// set blendMode
		void setBlendMode(SDL_BlendMode blending);

		// set alpha
		void setAlpha(Uint8 alpha);

		// getDimensions
		inline int getWidth();
		inline int getHeight();

	private:
		SDL_Texture * mTexture = NULL;
		TTF_Font * gFont = NULL;
		SDL_Renderer * gRenderer = NULL;
		int mWidth;
		int mHeight;
};


Texture::Texture(SDL_Renderer * r,const char * font)
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

	this->gFont = TTF_OpenFont(font,24);
}
Texture::Texture(SDL_Renderer * r)
{
	this->gRenderer = r;
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}


Texture::~Texture()
{
	free();
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool Texture::loadFromRenderedText(std::string textString, SDL_Color textColor)
{
	// get rid of pre-existing texture
	free();

	SDL_Surface * surface = TTF_RenderText_Solid(this->gFont,textString.c_str(),textColor);
	if(surface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error : %s\n",TTF_GetError());
		return false;
	}
	else
	{
		mTexture = SDL_CreateTextureFromSurface(this->gRenderer,surface);
		if(mTexture == NULL)
		{
			printf("Unable to create texture from rendererd text! SDL Error: %s \n",SDL_GetError());
			return false;
		}
		else
		{
			mWidth = surface->w;
			mHeight = surface->h;
		}
		SDL_FreeSurface(surface);
	}
	return mTexture != NULL;

}
#endif

bool Texture::loadFromFile(std::string path)
{
	free();
	SDL_Texture * newTexture = NULL;
	SDL_Surface * loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error %s\n",path.c_str(),IMG_GetError());
	}
	else
	{
		// COlor key image;
		SDL_SetColorKey(loadedSurface,SDL_TRUE,SDL_MapRGB(loadedSurface->format,0,0xff,0xff));

		// create texture from surface
		newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
		if(newTexture == NULL)
		{
			printf("Unable to create texture from %s SDL Error: %s\n",path.c_str(),SDL_GetError());
		}
		else{
			// get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		// free the loaded surface
		SDL_FreeSurface(loadedSurface);

	}
	mTexture = newTexture;
	return mTexture != NULL;
}


void Texture::free()
{
	if(mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = mHeight = 0;
	}
}


void Texture::render(int x, int y,SDL_Rect * clip,double angle , SDL_Point * center ,SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = {x,y,mWidth,mHeight};

	if(clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(gRenderer,mTexture,clip,&renderQuad,angle,center,flip);
}

void Texture::renderResize(SDL_Rect * src,SDL_Rect * dest,bool flip)
{
	if(flip)
		SDL_RenderCopyEx(gRenderer,mTexture,src,dest,0,NULL,SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopy(gRenderer,mTexture,src,dest);

}

void Texture::render(int x, int y,SDL_Rect * dest, SDL_Rect * src)
{
	SDL_RenderCopy(gRenderer,mTexture,dest,src);
}


void Texture::setColor(Uint8 red,Uint8 green,Uint8 blue)
{
	SDL_SetTextureColorMod(mTexture,red,green,blue);
}


void Texture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(mTexture,blending);
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(mTexture,alpha);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}


#endif
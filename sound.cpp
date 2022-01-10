#include <map>
#include <SDL2/SDL_mixer.h>

#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

class SoundManager
{
public:
	SoundManager();
	~SoundManager();
	// load and store music file with id
	void loadMusic(const char * id,const char * filename);
	// load and store sound file with id
	void loadSound(const char * id,const char * filename);
	// play stored music file with id as reference
	void playMusic(const char * id);
	// play stored sound file with id as reference
	void playSound(const char * id);




private:
	Mix_Music * music;
	std::map <const char *, Mix_Chunk *> store;
};


SoundManager::~SoundManager()
{
	for(int i = 0 ; i < store.size() ; i ++ )
	{
	//	Mix_FreeChunk(store[])
	}
	Mix_CloseAudio();
}

SoundManager::SoundManager()
{
	printf("[_+_] Sound system init\n");
	
	int audioRate = 44100;

	if(Mix_OpenAudio(audioRate,MIX_DEFAULT_FORMAT,2,4096) < 0)
	{
		printf("Error: Unable to open audio! Mix_Error %s\n",Mix_GetError());
	}	
}

void SoundManager::loadMusic(const char * id,const char * filename)
{
	this->music = Mix_LoadMUS(filename);
	if(!this->music)
	{
		printf("Unable to load music file %s! Mix_Error %s\n",filename,Mix_GetError());
	}
	else{
		printf("Loaded music file '%s' to [0x%x]\n",filename,this->music);
	}
}

void SoundManager::loadSound(const char * id,const char * filename)
{

	Mix_Chunk * t  =  Mix_LoadWAV(filename);
	if(t == NULL)
	{
		printf("Error: %s\n",Mix_GetError());
	}
	else
	{
		this->store[id] = t;
		printf("[_+_] Loaded sound file '%s' to [0x%x]\n",filename,this->store[id]);
	}


}

void SoundManager::playMusic(const char * id)
{
	Mix_PlayMusic(this->music,0);
}

void SoundManager::playSound(const char * id)
{
	Mix_PlayChannel(0,this->store[id],0);
}

#endif
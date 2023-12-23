#pragma once

#include <SDL_mixer.h>

class Music
{
public:
	Music(const std::string& filepath);
	~Music();

	Mix_Music* GetNative() { return m_Music; }

	static Ref<Music> Create(const std::string& filepath);

private:
	Mix_Music* m_Music;
};

class Sound
{
public:
	Sound(const std::string& filepath);
	~Sound();
	
	Mix_Chunk* GetNative() { return m_Chunk; }

	static Ref<Sound> Create(const std::string& filepath);

private:
	Mix_Chunk* m_Chunk;
};
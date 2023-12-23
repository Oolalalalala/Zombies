#include "pch.h"
#include "Audio.h"

Music::Music(const std::string& filepath)
{
	m_Music = Mix_LoadMUS(filepath.c_str());

	if (!m_Music)
		CORE_ERROR("Failed to load music, filepath = \"{0}\"", filepath);
}

Music::~Music()
{
	Mix_FreeMusic(m_Music);
}

Ref<Music> Music::Create(const std::string& filepath)
{
	return CreateRef<Music>(filepath);
}

Sound::Sound(const std::string& filepath)
{
	m_Chunk = Mix_LoadWAV(filepath.c_str());
	
	if (!m_Chunk)
		CORE_ERROR("Failed to load sound, filepath = \"{0}\"", filepath);
}

Sound::~Sound()
{
	Mix_FreeChunk(m_Chunk);
}

Ref<Sound> Sound::Create(const std::string& filepath)
{
	return CreateRef<Sound>(filepath);
}

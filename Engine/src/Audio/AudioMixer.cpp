#include "pch.h"
#include "AudioMixer.h"

#include <atomic>
#include <SDL.h>

static int s_ChannelCount = 8;
static std::atomic<int> s_OccupiedChannelCount = 0;

void AudioMixer::Initialize()
{
	Mix_Init(MIX_INIT_MP3);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
		CORE_CRITICAL("Failed to initialize SDL_AUDIO");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		CORE_CRITICAL("Failed to initialize SDL_MIXER, error : ", Mix_GetError());

	Mix_AllocateChannels(s_ChannelCount);

	Mix_ChannelFinished([](int cnannels) { s_OccupiedChannelCount--; });
}

void AudioMixer::ShutDown()
{
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void AudioMixer::PlayMusic(Ref<Music> music, float volume, float fadeTime)
{
	Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
	Mix_FadeInMusic(music->GetNative(), -1, fadeTime * 1000);
}

void AudioMixer::PlaySound(Ref<Sound> sound, float volume, float fadeTime)
{
	if (s_OccupiedChannelCount == s_ChannelCount)
	{
		CORE_WARN("Sound is not played, all {0} channels are in use", s_ChannelCount);
		return;
	}

	Mix_VolumeChunk(sound->GetNative(), volume * MIX_MAX_VOLUME);
	Mix_FadeInChannel(-1, sound->GetNative(), 0, fadeTime * 1000);

	s_OccupiedChannelCount++;
}

void AudioMixer::StopMusic(Ref<Music> music, float fadeTime)
{
	Mix_FadeOutMusic(fadeTime * 1000);
}

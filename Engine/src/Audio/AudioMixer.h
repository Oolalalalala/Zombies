#pragma once

#include "Audio.h"


class AudioMixer
{
public:
	static void Initialize();
	static void ShutDown();

	static void PlayMusic(Ref<Music> music, float volume = 1.0f, float fadeTime = 0.0f);
	static void PlaySound(Ref<Sound> sound, float volume = 1.0f, float fadeTime = 0.0f);

	static void StopMusic(Ref<Music> music, float fadeTime = 0.0f);
};
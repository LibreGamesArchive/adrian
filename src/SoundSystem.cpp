/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2003  Bhanu Chetlapalli, Vamsi Krishna, Swamy Suman
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "SoundSystem.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string.h>
#include "globals.h"

SoundSystem::SoundSystem(void)
{
	initialized = false;

	audio_rate = 44100;
	audio_format = AUDIO_S16;
	audio_channels = 2;
	audio_buffers = 4096;

	memset(sounds, 0, sizeof(sounds));
	for (int i = 0; i < MAX_SOUNDS; i++) {
		channels[i] = -1;
		channelFadeStatus[i] = false;
	}
}

SoundSystem::~SoundSystem()
{
}

int SoundSystem::Initialize(void)
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		printf("Error Initializing Sound SubSystem\nSound will be Disabled\n");
		return -1;
	}

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Unable to open audio!\n");
		return -1;
	}

	initialized = true;

	return 0;
}

void SoundSystem::Destroy(void)
{
	UnloadAll();
	initialized = false;
	Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void SoundSystem::UnloadAll(void)
{
	if (!initialized)
		return;

	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[i]) {
			printf("Unloading Sound in Channel: %d\n", i);
			Mix_FreeChunk(sounds[i]);
			sounds[i] = NULL;
		}
	}
}

int SoundSystem::Load(const char *wavfile, int idx, int defaultRepeat)
{
	if (!initialized)
		return -1;

	if ((sounds[idx] = Mix_LoadWAV(wavfile)) == NULL) {
		printf("Cannot Load %s into %d channel\n", wavfile, idx);
		return -1;
	}
	repeatCount[idx] = defaultRepeat;

	return 0;
}

void SoundSystem::HaltAllChannels(void)
{
	if (!initialized)
		return;

	Mix_HaltChannel(-1);
	for (int i = 0; i < MAX_SOUNDS; i++)
		channels[i] = -1;
}

bool SoundSystem::PlaySound(SoundType s, int loop)
{
	if (!initialized)
		return false;

	if ((channels[s] = Mix_PlayChannel(s, sounds[s], loop)) < 0) {
		printf("Unable to play sound %d\n", s);
		return false;
	}
	return true;
}

bool SoundSystem::PlaySound(SoundType s)
{
	return PlaySound(s, repeatCount[s]);
}

/*void ChannelFadeCallback( int channel )
{
	switch( channel )
	{
		case SOUNDTYPE_BACKGROUND:
//			if( channelFadeStatus[channel] )		//Fade out
			{
				soundSystem->Play( SOUNDTYPE_BACKGROUND );
//				channelFadeStatus[channel] = false;
//				Mix_FadeOutChannel( soundSystem->channels[SOUNDTYPE_BACKGROUND] , 10000 );
			}
			else	// Do fade in
			{
				Mix_FadeInChannel( channel ,  ,  );
				channelFadeStatus[channel] = true;
			}
			return;
		case SOUNDTYPE_MENU:
			return;
	}
}*/

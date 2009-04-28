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
	if (!initialized)
		return;
	Mix_HaltChannel(-1);
}

int SoundSystem::Initialize(void)
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		printf("Error Initializing Sound SubSystem\nSound will be Disabled\n");
		return -1;
	}

	if (Mix_OpenAudio
	    (audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Unable to open audio!\n");
		return -1;
	}

	initialized = true;

	if (LoadWavs() < 0) {
		return -1;
	}

	return 0;
}

int SoundSystem::LoadWavs(void)
{
	if (!initialized)
		return -1;

	sounds[SOUNDTYPE_SHOOT] = Mix_LoadWAV("wavs/laser.wav");
	sounds[SOUNDTYPE_BACKGROUND] = Mix_LoadWAV("wavs/background.wav");

	return 0;
}

int SoundSystem::Play(SoundType s)
{
	if (!initialized)
		return -1;

	switch (s) {
	case SOUNDTYPE_ALARM:
		HaltAllChannels();
		if (!sounds[SOUNDTYPE_ALARM]) {
			if ((sounds[SOUNDTYPE_ALARM] = Mix_LoadWAV("wavs/alarm.wav")) == NULL) {
				printf("Cannot Load wavs/alarm.wav\n");
				return -1;
			}
		}
		PlaySound(SOUNDTYPE_ALARM, 3);
		return 0;

	case SOUNDTYPE_BACKGROUND:
		if (!sounds[SOUNDTYPE_BACKGROUND]) {
			if ((sounds[SOUNDTYPE_BACKGROUND] = Mix_LoadWAV("wavs/background.wav")) == NULL) {
				printf("Cannot load wavs/background.wav\n");
				return -1;
			}
		}
		PlaySound(SOUNDTYPE_BACKGROUND, -1);
//                      Mix_FadeOutChannel( channels[SOUNDTYPE_BACKGROUND] , 10000 );
		return 0;
	case SOUNDTYPE_MENU:
		if (sounds[SOUNDTYPE_MENU] == NULL) {
			if ((sounds[SOUNDTYPE_MENU] =
			     Mix_LoadWAV("wavs/menu.wav")) == NULL) {
				printf("Cannot load wavs/menu.wav\n");
				return -1;
			}
		}
		PlaySound(SOUNDTYPE_MENU, -1);
		return 0;
	case SOUNDTYPE_MENU_TING:
		if (sounds[SOUNDTYPE_MENU_TING] == NULL) {
			if ((sounds[SOUNDTYPE_MENU_TING] =
			     Mix_LoadWAV("wavs/ting.wav")) == NULL) {
				printf("Cannot load wavs/ting.wav\n");
				return -1;
			}
		}
		PlaySound(SOUNDTYPE_MENU_TING, 0);
		return 0;
	case SOUNDTYPE_SHOOT:
		if (!sounds[SOUNDTYPE_SHOOT]) {
			if ((sounds[SOUNDTYPE_SHOOT] =
			     Mix_LoadWAV("wavs/laser.wav")) == NULL) {
				printf("Cannot load wavs/laser.wav\n");
				return -1;
			}
		}
		PlaySound(SOUNDTYPE_SHOOT, 1);
		return 0;
	}

	return -1;
}

void SoundSystem::HaltAllChannels(void)
{
	Mix_HaltChannel(-1);
	for (int i = 0; i < MAX_SOUNDS; i++)
		channels[i] = -1;
}

bool SoundSystem::PlaySound(SoundType s, int loop)
{
	if ((channels[s] = Mix_PlayChannel(s, sounds[s], loop)) < 0) {
		printf("Unable to play sound %d\n", SOUNDTYPE_MENU);
		return false;
	}
	return true;
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

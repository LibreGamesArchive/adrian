#ifndef					__GAME_SOUNDSYSTEM_H__
#define					__GAME_SOUNDSYSTEM_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <SDL/SDL_mixer.h>
#include <stdio.h>

#define			MAX_SOUNDS		20

extern void ChannelFadeCallback(int channel);

typedef enum {
	SOUNDTYPE_ALARM = 1,
	SOUNDTYPE_BACKGROUND,
	SOUNDTYPE_MENU,
	SOUNDTYPE_SHOOT
} SoundType;

class SoundSystem {
	friend void ChannelFadeCallback(int channel);

 public:
	 SoundSystem(void);
	~SoundSystem();

	int Initialize(void);
	int LoadWavs(void);

	int Play(SoundType);

	void HaltAllChannels(void);

	bool PlaySound(SoundType s, int loop);

 private:
	 bool initialized;

	Mix_Chunk *sounds[MAX_SOUNDS];
	int channels[MAX_SOUNDS];
	bool channelFadeStatus[MAX_SOUNDS];

	int audio_rate;
	Uint16 audio_format;
	int audio_channels;
	int audio_buffers;

};

#endif				/*      __GAME_SOUNDSYSTEM_H__  */

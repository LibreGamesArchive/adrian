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
	SOUNDTYPE_BACKGROUND = 1,
	SOUNDTYPE_MENU = 1,
	SOUNDTYPE_MENU_TING = 2,
	SOUNDTYPE_SHOOT = 2,
	SOUNDTYPE_ALARM = 3,
} SoundType;

class SoundSystem {
	friend void ChannelFadeCallback(int channel);

 public:
	 SoundSystem(void);
	~SoundSystem();

	int Initialize(void);
	void Destroy(void);

	int Load(const char *wavfile, int idx, int defaultRepeat);
	void UnloadAll(void);

	void HaltAllChannels(void);

	bool PlaySound(SoundType s);
	bool PlaySound(SoundType s, int loop);

 private:
	 bool initialized;

	Mix_Chunk *sounds[MAX_SOUNDS];
	int repeatCount[MAX_SOUNDS];
	int channels[MAX_SOUNDS];
	bool channelFadeStatus[MAX_SOUNDS];

	int audio_rate;
	Uint16 audio_format;
	int audio_channels;
	int audio_buffers;

};

#endif				/*      __GAME_SOUNDSYSTEM_H__  */

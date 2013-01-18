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


#ifndef					__GAME_SOUNDSYSTEM_H__
#define					__GAME_SOUNDSYSTEM_H__

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

	static bool flag_no_sound;

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

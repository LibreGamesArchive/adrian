/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2013  Bhanu Chetlapalli
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

#ifndef __MD2_H__
#define __MD2_H__

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>

#include "texture.h"

#define MAX_FILENAME_LEN    	64
#define	MAX_LOADED_MD2_MODELS	32

#define	MSEC_PER_MD2FRAME	200

#define	ARRAY_SIZE(x)	(((int)sizeof(x)/(int)sizeof(x[0])))
enum AnimType {
	ANIMTYPE_INVALID = -1,
	ANIMTYPE_STAND = 0,
	ANIMTYPE_ATTACK,
	ANIMTYPE_RUN,
	ANIMTYPE_DEATH,
	ANIMTYPE_DEATH2,
	ANIMTYPE_DEATH3,
	ANIMTYPE_INVISIBLE,
	MAX_NUM_ANIMATIONS
};

struct AnimTypeMap {
	const char nm[16];
	int len;
	AnimType type;
};

/*
 * Source: http://en.wikipedia.org/wiki/MD2_%28file_format%29#File_format
 */
struct MD2Header {
#define	MD2_IDENT_VAL	(('I'<<0) | ('D' << 8) | ('P' << 16) | ('2' << 24))
	int ident;		// Magic number. Must be equal to "IDP2"
#define	MD2_VERSION_VAL	8
	int version;	// MD2 version. Must be equal to 8
	int skinwidth;	// Width of the texture
	int skinheight; // Height of the texture
	int framesize;	// Size of one frame in bytes
	int num_skins;	// Number of textures
	int num_xyz;	// Number of vertices
	int num_st;		// Number of texture coordinates
	int num_tris;	// Number of triangles
	int num_glcmds; // Number of OpenGL commands
	int num_frames; // Total number of frames
#define	MD2_SKIN_NAME_LEN	64
	int ofs_skins;	// Offset to skin names (each skin name is an unsigned char[64] and are null terminated)
	int ofs_st;		// Offset to s-t texture coordinates
	int ofs_tris;	// Offset to triangles
	int ofs_frames; // Offset to frame data
	int ofs_glcmds; // Offset to OpenGL commands
	int ofs_end;	// Offset to end of file
};

struct MD2TexCoords {
	short 	s;
	short 	t;
};

struct MD2Tris {
	 short vi[3];
	 short ti[3];
};

struct TexCoords {
	float s;
	float t;
};

struct MD2VertCoords {
	 unsigned char v[3];
	 unsigned char normi;
};
struct MD2Frame {
	float scale[3];
	float translate[3];
	char name[16];
	MD2VertCoords vc[0];
};

struct Vertex3 {
	float x;
	float y;
	float z;
};

class KeyFrame {
	public:
		float scale[3];
		float translate[3];
		char name[16];

		Vertex3 *v;


		KeyFrame(MD2Frame *fm, int nv);
		~KeyFrame();

};

struct Animation {
	public:
		int animid;
		char name[16];
		int frameStart;
		int frameEnd;
};

class MD2;
struct MD2ModelTable {
	char filename[MAX_FILENAME_LEN];
	MD2 *ptr;
	int ref;
};

class AnimObj {
	public:
		AnimObj(void);
		float x, y, z;
		float facingAngle;
		AnimType currentAnimation;

	private:
		int beginTime;

		/* For single shot cases */
		AnimType nextAnimation;
		int endTime;

		const Animation *const*anim;

		static MD2ModelTable md2ModelTable[MAX_LOADED_MD2_MODELS];

	public:
		const MD2 *getMD2Base(const char *fn);
		void putMD2Base(const MD2 *);

		bool getFrames(int *frm1, int *frm2, float *fraction);
		void setAnimation(AnimType at, int fixed_reps = 0, AnimType next = ANIMTYPE_INVALID);

		inline AnimType getCurAnimation(void) const
		{
			return currentAnimation;
		}
};

class MD2 {
	friend class AnimObj;
	private:
		char fn[MAX_FILENAME_LEN];
		MD2Header h;

		TexCoords *texCoords;
		MD2Tris *tris;

		KeyFrame **frames;

		int readSt(FILE *f, void *buf, int sz, unsigned int num, int off);

		Animation *anim[MAX_NUM_ANIMATIONS];
		AnimType lookupAnimation(const char *name);
		int addToAnimation(MD2Frame *mf, int frameno);

		void Animate(AnimObj *ao) const;

		Texture *tex;

	public:
		MD2();
		~MD2();

		int Load(const char *fn);
		void Unload(void);

		int getNumFrames(AnimType) const;

		void render(AnimObj *ao) const;
};

#endif /* __MD2_H__ */


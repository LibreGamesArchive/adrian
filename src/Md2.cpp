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

#include "Md2.h"
#include "globals.h"
#include <string.h>
#include <SDL/SDL_image.h>
#include <ctype.h>

#define MAX_FILENAME_LEN    64

MD2::MD2(void)
{
	memset(anim, 0, sizeof(anim));
	texID = 73;

	currentAnimation = ANIMTYPE_INVALID;
}

MD2::~MD2()
{
}

int MD2::readSt(FILE *f, void *buf, int sz, unsigned int num, int off)
{
	if (buf == NULL) {
		fprintf(stderr, "Memory buffer is NULL for MD2 Modelfile(%s)\n", fn);
		return -1;
	}
	if (fseek(f, off, SEEK_SET) != 0) {
		fprintf(stderr, "Seeking to offset(%d) of MD2 Modelfile(%s) failed\n",
				off, fn);
		return -1;
	}
	if (fread(buf, sz, num, f) != num) {
		fprintf(stderr, "Reading structures from MD2 Modelfile(%s) failed\n",
				fn);
		return -1;
	}

	return 0;
}

AnimType MD2::lookupAnimation(const char *name)
{
#define V(x)    x, (sizeof(x)-1)
	static const AnimTypeMap animTypeMap[] = {
		{ V("stand"),  ANIMTYPE_STAND },
		{ V("death"),  ANIMTYPE_DEATH },
		{ V("attack"), ANIMTYPE_ATTACK },
		{ V("run"),    ANIMTYPE_RUN },
	};

	int i;
	for (i = 0; i < ARRAY_SIZE(animTypeMap); i++) {
		if (!strncmp(name, animTypeMap[i].nm, animTypeMap[i].len)) {
			return animTypeMap[i].type;
		}
	}
	return ANIMTYPE_INVALID;
}

int MD2::addToAnimation(MD2Frame *mf, int frameno)
{
	AnimType at = lookupAnimation(mf->name);
	if (at == ANIMTYPE_INVALID)
		return -1;

	if (anim[at] == NULL) {
		anim[at] = new Animation;
		anim[at]->animid = at;
		strcpy(anim[at]->name, mf->name);
		anim[at]->frameStart = frameno;
	}
	anim[at]->frameEnd = frameno;
	return 0;
}

#define	GF(x)	do {\
	if (x) \
		goto fail; \
} while(0)
#define	FREE(x) do {\
	if (x != NULL) \
		free(x); \
	x = NULL; \
} while(0)

int MD2::Load(const char *fn)
{
	int i, j;
	int framesize;
	int err = -1;
	FILE *f;
	MD2TexCoords * tmpcoords = NULL;
	MD2Frame *mf = NULL;

	if ((f = fopen(fn, "r")) == NULL) {
		fprintf(stderr, "Opening MD2 Modelfile(%s) failed\n", fn);
		goto fail;
	}
	
	strncpy(this->fn, fn, MAX_FILENAME_LEN-1);

	if (fread(&h, sizeof(MD2Header), 1, f) != 1) {
		fprintf(stderr, "Reading Header of MD2 Modelfile(%s) failed\n", fn);
		goto fail;
	}

	if (h.ident != MD2_IDENT_VAL || h.version != MD2_VERSION_VAL) {
		fprintf(stderr, "Illegal values in MD2 hdr(%s) ID(%#0x Exp:%#0x) "
				"Ver(%#0x Exp:%#0x) "
				"Possible corruption or wrong format.\n",
				fn, h.ident, MD2_IDENT_VAL, h.version, MD2_VERSION_VAL);
		goto fail;
	}

	/* Load Textures */
	for (i = 0; i < h.num_skins; i++) {
		char texnm[MD2_SKIN_NAME_LEN];
		GF(readSt(f, texnm, MD2_SKIN_NAME_LEN, 1, h.ofs_skins + i*MD2_SKIN_NAME_LEN));
//		printf("Loading MD2 Texture(%d): %s\n", i, texnm);
	}

	/* Load Texture Coordinates */
	GF((tmpcoords = (MD2TexCoords*)malloc(h.num_st * sizeof(MD2TexCoords))) == NULL);
	GF(readSt(f, tmpcoords, sizeof(MD2TexCoords), h.num_st, h.ofs_st));
	GF((texCoords = (TexCoords *)malloc(h.num_st * sizeof(TexCoords))) == NULL);
	for (i = 0; i < h.num_st; i++) {
		texCoords[i].s = (float)tmpcoords[i].s / h.skinwidth;
		texCoords[i].t = (float)tmpcoords[i].t / h.skinheight;
	}
	FREE(tmpcoords);

	/* Load Triangle Coordinate Indices */
	GF((tris = (MD2Tris*)malloc(h.num_tris * sizeof(MD2Tris))) == NULL);
	GF(readSt(f, tris, sizeof(MD2Tris), h.num_tris, h.ofs_tris));

	/* Load Frame information */
	GF((frames = (KeyFrame**)calloc(sizeof(KeyFrame*) , h.num_frames)) == NULL);
	framesize = sizeof(MD2Frame) + h.num_xyz*sizeof(MD2VertCoords);
	GF((mf = (MD2Frame*)malloc(framesize)) == NULL);
	for (i = 0, j = 0; i < h.num_frames; i++) {
		GF(readSt(f, mf, framesize, 1, h.ofs_frames + i*framesize));
		if (addToAnimation(mf, j))
			continue;
		GF((frames[j++] = new KeyFrame(mf, h.num_xyz)) == NULL);
	}
	FREE(mf);

	err = 0;

 fail:
	if (err) {
		FREE(texCoords);
		FREE(tmpcoords);
		FREE(tris);
		FREE(mf);
		for (i = 0; i < MAX_NUM_ANIMATIONS; i++)
			delete anim[i];
	}
	if (f) fclose(f);
	return err;
}

void MD2::Unload(void)
{
	int i;
	FREE(texCoords);
	FREE(tris);
	for (i = 0; i < MAX_NUM_ANIMATIONS; i++)
		delete anim[i];
}

void MD2::Animate(void)
{
	int i, j;
	if (currentAnimation == ANIMTYPE_INVALID) {
		return;
	}
	Animation *a = anim[currentAnimation];
	int curtime = SDL_GetTicks();
	int frm1 = 0;
	int frm2 = 0;
	float fraction = 0.0f;

#define	MSEC_PER_MD2FRAME	200

	int totframes = a->frameEnd - a->frameStart + 1;
	if (totframes) {
		frm1 = a->frameStart + ((curtime - beginTime)/MSEC_PER_MD2FRAME) % totframes;
		frm2 = a->frameStart + ((curtime - beginTime + 1)/MSEC_PER_MD2FRAME) % totframes;
		fraction = ((curtime - beginTime) % MSEC_PER_MD2FRAME) / (float)MSEC_PER_MD2FRAME;
	}

	KeyFrame *k1 = frames[frm1];
	KeyFrame *k2 = frames[frm2];

	glBegin(GL_TRIANGLES);
		/* Draw each triangle */
		for (i = 0; i < h.num_tris; i++) {
			/* Draw each vertex */
			for (j = 0; j < 3; j++) {
				int tIdx = tris[i].ti[j];
				glTexCoord2f(texCoords[tIdx].s, texCoords[tIdx].t);

				int vIdx = tris[i].vi[j];
#define	INTRPOLATE(D) (((k1->v[vIdx].D) * fraction) + ((k2->v[vIdx].D) * (1.0f - fraction)))
				glVertex3f(INTRPOLATE(x), INTRPOLATE(y), INTRPOLATE(z));
			}
		}
	glEnd();
}

void MD2::setAnimation(AnimType at)
{
	beginTime = SDL_GetTicks();
	currentAnimation = at;
}

void MD2::render(float x, float y, float z, float facingAngle)
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
			
	glTranslatef(x, y, z);
	glRotatef(-facingAngle, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, texID);
	Animate();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

int MD2::getNumFrames(AnimType at)
{
	return anim[at]->frameEnd - anim[at]->frameStart;
}

KeyFrame::KeyFrame(MD2Frame *fm, int num_verts)
{
	int i;

	memcpy(scale, fm->scale, sizeof(scale));
	memcpy(translate, fm->translate, sizeof(translate));
	strcpy(name, fm->name);

	v = (Vertex3*)malloc(sizeof(Vertex3) * num_verts);

	for (i = 0; i < num_verts; i++) {
		/* Process vertices */
		v[i].x = fm->vc[i].v[0] * fm->scale[0] + fm->translate[0];
		v[i].z = fm->vc[i].v[1] * fm->scale[1] + fm->translate[1];
		v[i].y = fm->vc[i].v[2] * fm->scale[2] + fm->translate[2];
	}
}

KeyFrame::~KeyFrame()
{
}



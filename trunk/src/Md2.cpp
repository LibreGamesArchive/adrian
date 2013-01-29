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
	memset(&h, 0, sizeof(h));
	tex = NULL;
	texCoords = NULL;
	tris = NULL;
	frames = NULL;
	loaded = false;
}

MD2::~MD2()
{
	if (loaded)
		Unload();
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
	unsigned int n;
	if ((n = fread(buf, sz, num, f)) != num) {
		fprintf(stderr, "Reading structures from MD2 Modelfile(%s) failed (read %d out of %d)\n",
				fn, n, num);
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
		{ V("death"),  ANIMTYPE_DEATH2 },
		{ V("death"),  ANIMTYPE_DEATH3 },
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

	/* Each MD2 has 3 types of death */
	if (at == ANIMTYPE_DEATH && anim[ANIMTYPE_DEATH]) {
		if (frameno > anim[ANIMTYPE_DEATH]->frameStart + 11)
			at = ANIMTYPE_DEATH3;
		else if (frameno > anim[ANIMTYPE_DEATH]->frameStart + 5)
			at = ANIMTYPE_DEATH2;
	}

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
	if ((x) != NULL) \
		free(x); \
	(x) = NULL; \
} while(0)
#define	DELETEP(x) do {\
	if ((x) != NULL) \
		delete (x); \
	(x) = NULL; \
} while(0)


int MD2::Load(const char *fn)
{
	int i, j;
	int framesize;
	int err = -1;
	FILE *f;
	MD2TexCoords * tmpcoords = NULL;
	MD2Frame *mf = NULL;
	char texNm[256];
	int l = strlen(fn);

	if ((f = fopen(fn, "rb")) == NULL) {
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
//	for (i = 0; i < h.num_skins; i++) {
//		char texnm[MD2_SKIN_NAME_LEN];
//		GF(readSt(f, texnm, MD2_SKIN_NAME_LEN, 1, h.ofs_skins + i*MD2_SKIN_NAME_LEN));
//		printf("Loading MD2 Texture(%d): %s\n", i, texnm);
//	}
	strcpy(texNm, fn);
	texNm[l-3] = 't';
	texNm[l-2] = 'g';
	texNm[l-1] = 'a';
	GF((tex = new Texture(texNm)) == NULL);
	GF(tex->Load());

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
	loaded = true;

 fail:
	if (err) {
		FREE(texCoords);
		FREE(tmpcoords);
		FREE(tris);
		FREE(mf);
		if (frames) {
			for (i = 0; i < h.num_frames; i++)
				DELETEP(frames[i]);
			FREE(frames);
		}
		for (i = 0; i < MAX_NUM_ANIMATIONS; i++)
			DELETEP(anim[i]);
		DELETEP(tex);
	}
	if (f) fclose(f);
	return err;
}

void MD2::Unload(void)
{
	int i;
	loaded = false;
	FREE(texCoords);
	FREE(tris);
	for (i = 0; i < h.num_frames; i++)
		DELETEP(frames[i]);
	FREE(frames);
	for (i = 0; i < MAX_NUM_ANIMATIONS; i++)
		DELETEP(anim[i]);
	DELETEP(tex);
	memset(&h, 0, sizeof(h));
}

void MD2::Animate(AnimObj *ao) const
{
	int i, j;
	int frame1, frame2;
	float fraction;

	if (!ao->getFrames(&frame1, &frame2, &fraction))
		return;

	KeyFrame *k1 = frames[frame1];
	KeyFrame *k2 = frames[frame2];

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

void MD2::render(AnimObj *ao) const
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
			
	glTranslatef(ao->x, ao->y, ao->z);
	glRotatef(-ao->facingAngle, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, tex->GetTexID());
	Animate(ao);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}


/******************************************************************/
/* AnimObj Definitions Start from here                            */
/******************************************************************/
AnimObj::AnimObj(void)
{
	x = y = z = facingAngle = 0;
	frozen = 0;
	currentAnimation = ANIMTYPE_INVALID;
}

bool AnimObj::getFrames(int *frame1, int *frame2, float *fraction)
{
	int curtime = SDL_GetTicks();

	if (endTime <= curtime) {
		if (nextAnimation != ANIMTYPE_FREEZEFRAME) {
			setAnimation(nextAnimation);
		} else {
			Freeze(endTime-MSEC_PER_MD2FRAME);
		}
	}

	/* Nothing to render */
	if (currentAnimation == ANIMTYPE_INVISIBLE)
		return false;

	assert(currentAnimation != ANIMTYPE_INVALID);

	if (frozen) {
		curtime = frozen;
	}

	const Animation *a = anim[currentAnimation];
	int totframes = a->frameEnd - a->frameStart + 1;

	*frame1 = a->frameStart + ((curtime - beginTime)/MSEC_PER_MD2FRAME) % totframes;
	*frame2 = a->frameStart + ((curtime - beginTime + 1)/MSEC_PER_MD2FRAME) % totframes;
	*fraction = ((curtime - beginTime) % MSEC_PER_MD2FRAME) / (float)MSEC_PER_MD2FRAME;

	return true;
}

void AnimObj::Freeze(int freezeTime)
{
	if (freezeTime)
		frozen = freezeTime;
	else if (!frozen)
		frozen = SDL_GetTicks();
}

void AnimObj::Unfreeze(void)
{
	frozen = 0;
}

void AnimObj::setAnimation(AnimType at, int fixed_reps, AnimType next)
{
	beginTime = SDL_GetTicks();

	if (at == ANIMTYPE_DEATH) {
		switch(rand() % 3) {
			case 2:
				at = ANIMTYPE_DEATH3;
				break;
			case 1:
				at = ANIMTYPE_DEATH2;
				break;
		}
	}
	if (fixed_reps > 0) {
		const Animation *a = anim[at];
		int num_frames = (a->frameEnd - a->frameStart + 1) * fixed_reps;
		endTime = beginTime + num_frames * MSEC_PER_MD2FRAME;
		nextAnimation = next;
	} else {
		endTime = MAX_END_TIME;
		nextAnimation = ANIMTYPE_INVALID;
	}
	currentAnimation = at;
}

const MD2* AnimObj::getMD2Base(const char *fn)
{
	int i;
	for (i = 0; md2ModelTable[i].ptr != NULL; i++) {
		if (!strcmp(md2ModelTable[i].filename, fn)) {
			md2ModelTable[i].ref++;
			this->anim = md2ModelTable[i].ptr->anim;
			return md2ModelTable[i].ptr;
		}
	}

	/* OK we need to instantiate the model */
	MD2 *m = NULL;
	m = new MD2;
	if (m->Load(fn) == 0) {
		md2ModelTable[i].ref = 1;
		md2ModelTable[i].ptr = m;
		strcpy(md2ModelTable[i].filename, fn);
	} else {
		delete m;
		return NULL;
	}
	this->anim = m->anim;
	return m;
}

void AnimObj::putMD2Base(const MD2 *m)
{
	int i;
	for (i = 0; i < MAX_LOADED_MD2_MODELS; i++) {
		if (md2ModelTable[i].ptr == m) {
			md2ModelTable[i].ref--;
			if (md2ModelTable[i].ref == 0) {
				MD2 *tmp = md2ModelTable[i].ptr;
				md2ModelTable[i].filename[0] = '\0';
				md2ModelTable[i].ptr = NULL;
				tmp->Unload();
				delete tmp;
				break;
			}
		}
	}
}

MD2ModelTable AnimObj::md2ModelTable[MAX_LOADED_MD2_MODELS];

/******************************************************************/
/* Keyframe Definitions Start from here                           */
/******************************************************************/
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



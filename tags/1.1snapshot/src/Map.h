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


#ifndef			__GAME_MAP_H__
#define			__GAME_MAP_H__

#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
//#include <GL/glu.h>
#include "RenderPass.h"
#include "texture.h"
#include "Building.h"
#include "Sprite.h"
//#include "staticloader.h"

class Map: public RenderableObject{
 public:
	float length;
	float breadth;

	int no_of_buildings;
	Building *buildings;

	int textureID;
	char texname[256];

	int no_blocksx, no_blocksy;
	int blocksize;

	/* Texture and Render information */
	int num_textures;
	Texture **textures;

	int num_sprites;
	Sprite **sprites;

	int num_smodels;
	//SModel **smodels;

 public:
	 Map(void);
	~Map();

	void Initialize(void);
	void Destroy(void);

	int LoadFile(const char *mapdir = "maps/default");
	void Render(void);
	void TransparentRender(void);

	GLuint getTextureID(const char *fn);
	const char * getTextureFn(GLuint tid);

	void Dump(void);

};

#endif				/*      __GAME_MAP_H__  */

/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2009  Bhanu Chetlapalli, Vamsi Krishna
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


#ifndef		__TEXTURE_H__
#define		__TEXTURE_H__
#include <GL/glew.h>
#include <SDL/SDL_image.h>

#define	MAX_IMG_PATH_LEN	256
#define	INVALID_TEXTURE_ID	(0xFFFFFFFF)
class Texture {
	private:
		char fn[MAX_IMG_PATH_LEN];
		char *extension;
		bool loaded;
		GLuint texid;

	public:
		Texture(const char *fn);
		~Texture();

		int Load(GLuint tid = INVALID_TEXTURE_ID);
		int Unload(void);

        //helper function to create texture from surface.
        static void CreateTexFromSurf(SDL_Surface *surf, GLuint texid, bool reversed=false);

		inline GLuint GetTexID(void)
		{
			return texid;
		}
		inline const char *GetFn(void)
		{
			return fn;
		}

		bool isTextureSame(const char *path);

};

#endif	/*	__TEXTURE_H__	*/

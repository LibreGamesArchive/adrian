#ifndef		__TEXTURE_H__
#define		__TEXTURE_H__

#include <SDL/SDL_image.h>
#include <gl/gl.h>
#include <gl/glu.h>

#define	MAX_IMG_PATH_LEN	256

class Texture {
	private:
		char fn[MAX_IMG_PATH_LEN];
		bool loaded;

	public:
		Texture(const char *fn);
		~Texture();

		int Load(void);
		int Unload(void);

		int SetTexID(GLuint texid);
		int GetTexID(void);

};

#endif	/*	__TEXTURE_H__	*/

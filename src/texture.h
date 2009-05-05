#ifndef		__TEXTURE_H__
#define		__TEXTURE_H__

#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

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

		int GetTexID(void);

};

#endif	/*	__TEXTURE_H__	*/

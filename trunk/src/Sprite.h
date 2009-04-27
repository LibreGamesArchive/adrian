#ifndef		__SPRITE_H__
#define		__SPRITE_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

class Sprite
{
 private:
	float centx, centz;
	GLint texid;
 public:
	Sprite(GLint texid, float x, float y);
	~Sprite();

	void Render(void);
	
};

#endif	/*	__SPRITE_H__	*/


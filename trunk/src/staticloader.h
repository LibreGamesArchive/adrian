#ifndef		__STATICLOADER_H__
#define		__STATICLOADER_H__

#include <GL/gl.h>
#include <GL/glu.h>

#include <assimp.h>        // Plain-C interface
#include <aiScene.h>       // Output data structure
#include <aiPostProcess.h> // Post processing flags

class SModel
{
 private:
	const aiScene *scene;

 public:
	float x, y, z;
	float angle, scalefactor;
	float xoff, yoff, zoff;

	GLuint list;

	SModel(void);
	~SModel();

	int Load(const char *fn);

	void Render(void);

};

#endif	/*	__STATICLOADER_H__	*/


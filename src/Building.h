#ifndef				__GAME_BUILDING_H__
#define				__GAME_BUILDING_H__

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Building {
 private:
	char filepath[256];
 public:
	int buildingID;
	int buildingType;

	GLuint dlist;

	float x1;
	float x2;
	float y1;
	float y2;
	float h;

	float bx1;
	float bx2;
	float by1;
	float by2;

	int textureID;

 public:
	 Building(void);
	~Building();

	int Load(const char *fn);

	bool isInside(float x, float y);
	void setType(int);
	void Render(void);
	void Dump(void);

};

#endif				/*      __GAME_BUILDING_H__     */

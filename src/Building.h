#ifndef				__GAME_BUILDING_H__
#define				__GAME_BUILDING_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

class Building {
 public:
	int buildingID;
	int buildingType;

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

	 Building(void);
	~Building();

	bool isInside(float x, float y);
	void setType(int);
	void Render(void);
	void Dump(void);

};

#endif				/*      __GAME_BUILDING_H__     */

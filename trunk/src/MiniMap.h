#ifndef				__GAME_MINIMAP_H__
#define				__GAME_MINIMAP_H__

#include "Map.h"

class MiniMap {
 public:
//              Map * map;

	MiniMap();
	~MiniMap();

	void Render(void);

	float xconvfactor;
	float yconvfactor;

	GLuint mapbuildings;

	int GenerateDisplayLists(void);

	bool isMouseOver(int sx, int sy, float &, float &);

};

#endif				/*      __GAME_MINIMAP_H__      */

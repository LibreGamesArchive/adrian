#include "Sprite.h"
#include "Map.h"
#include "globals.h"
#include "Game.h"


Sprite::Sprite(GLint texid, float x, float z)
{
	this->texid = texid;
	this->centx = x;
	this->centz = z;
}

Sprite::~Sprite()
{
}

void Sprite::Render(void)
{
	float imgwdth = 60;
	float ang = game->camera->angle;
	/* actually 90 + angle */
	float lenx = cos(ang) * imgwdth;
	float lenz = -sin(ang) * imgwdth;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texid);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(centx - lenx, 0, centz - lenz);
		glTexCoord2f(1.0, 0);
		glVertex3f(centx - lenx, 150, centz - lenz);
		glTexCoord2f(0, 0);
		glVertex3f(centx + lenx, 150, centz + lenz);
		glTexCoord2f(0, 1.0);
		glVertex3f(centx + lenx, 0, centz + lenz);
	glEnd();
}


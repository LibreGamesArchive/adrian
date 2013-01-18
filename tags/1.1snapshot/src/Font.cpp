#include "globals.h"
#include <GL/glew.h>
//#include <GL/glu.h>

static float FontTexture[256][2];

void fontInit()
{
	FontTexture[46][0] = .625;
	FontTexture[46][1] = .625;

	FontTexture[63][0] = .742;
	FontTexture[63][1] = .875;

	FontTexture[35][0] = .25;
	FontTexture[35][1] = .5;

	FontTexture[32][0] = .875;
	FontTexture[32][1] = .875;

	FontTexture[48][0] = .875;
	FontTexture[48][1] = .625;

	FontTexture[49][0] = 0;
	FontTexture[49][1] = .75;

	FontTexture[50][0] = .117;
	FontTexture[50][1] = .75;

	FontTexture[51][0] = .25;
	FontTexture[51][1] = .75;

	FontTexture[52][0] = .367;
	FontTexture[52][1] = .75;

	FontTexture[53][0] = .5;
	FontTexture[53][1] = .75;

	FontTexture[54][0] = .625;
	FontTexture[54][1] = .75;

	FontTexture[55][0] = .742;
	FontTexture[55][1] = .75;

	FontTexture[56][0] = .875;
	FontTexture[56][1] = .75;

	FontTexture[57][0] = 0;
	FontTexture[57][1] = .875;

	FontTexture[58][0] = .125;
	FontTexture[58][1] = .875;

	FontTexture[65][0] = 0;
	FontTexture[65][1] = 0;

	FontTexture[66][0] = .117;
	FontTexture[66][1] = 0;

	FontTexture[67][0] = .25;
	FontTexture[67][1] = 0;

	FontTexture[68][0] = .367;
	FontTexture[68][1] = 0;

	FontTexture[69][0] = .5;
	FontTexture[69][1] = 0;

	FontTexture[70][0] = .625;
	FontTexture[70][1] = 0;

	FontTexture[71][0] = .742;
	FontTexture[71][1] = 0;

	FontTexture[72][0] = .875;
	FontTexture[72][1] = 0;

	FontTexture[73][0] = 0;
	FontTexture[73][1] = .125;

	FontTexture[74][0] = .117;
	FontTexture[74][1] = .125;

	FontTexture[75][0] = .25;
	FontTexture[75][1] = .125;

	FontTexture[76][0] = .367;
	FontTexture[76][1] = .125;

	FontTexture[77][0] = .5;
	FontTexture[77][1] = .125;

	FontTexture[78][0] = .625;
	FontTexture[78][1] = .125;

	FontTexture[79][0] = .742;
	FontTexture[79][1] = .125;

	FontTexture[80][0] = .875;
	FontTexture[80][1] = .125;

	FontTexture[81][0] = 0;
	FontTexture[81][1] = .25;
	FontTexture[82][0] = .117;
	FontTexture[82][1] = .25;
	FontTexture[83][0] = .25;
	FontTexture[83][1] = .25;
	FontTexture[84][0] = .367;
	FontTexture[84][1] = .25;
	FontTexture[85][0] = .5;
	FontTexture[85][1] = .25;
	FontTexture[86][0] = .625;
	FontTexture[86][1] = .25;
	FontTexture[87][0] = .742;
	FontTexture[87][1] = .25;
	FontTexture[88][0] = .875;
	FontTexture[88][1] = .25;
	FontTexture[89][0] = 0;
	FontTexture[89][1] = .375;
	FontTexture[90][0] = .117;
	FontTexture[90][1] = .375;
}

void drawfontString(const char *string, float x, float y, float font_width,
					float font_height)
{
	float add_x = (float)10 / (float)128;
	float add_y = (float)15 / (float)128;
	int loop_var = 0;
	int limit;
	const unsigned char *str = (const unsigned char *)string;

	/* Convert coordinates from fixed 100 points to resolution scale */
	x = SCR2RESX(x);
	y = SCR2RESY(y);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GEQUAL, 0.9);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 50);

	if (string == NULL)
		return;

	limit = strlen(string);

	for (loop_var = 0; loop_var < limit; loop_var++) {
		glBegin(GL_POLYGON);
		glTexCoord2f(FontTexture[str[loop_var]][0],
			     FontTexture[str[loop_var]][1]);
		glVertex2f(x + loop_var * font_width, y + font_height);

		glTexCoord2f(FontTexture[str[loop_var]][0],
			     FontTexture[str[loop_var]][1] + add_y);
		glVertex2f(x + loop_var * font_width, y);

		glTexCoord2f(FontTexture[str[loop_var]][0] + add_x,
			     FontTexture[str[loop_var]][1] + add_y);
		glVertex2f(x + loop_var * font_width + font_width, y);

		glTexCoord2f(FontTexture[str[loop_var]][0] + add_x,
			     FontTexture[str[loop_var]][1]);
		glVertex2f(x + loop_var * font_width + font_width,
			   y + font_height);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
}

#ifndef				__GAME_CAMERA_H__
#define				__GAME_CAMERA_H__

#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>

class Camera
{
	public:
		float camx;
		float camy;
		float camz;

		float pointx;
		float pointy;
		float pointz;
		
		float lookx;
		float looky;
		float lookz;

		float distance;
		float height;
		float angle;
		float initx;
		float initz;

		float yfactor;
		
	public:
		Camera( void );
		~Camera();

		void Initialize();

		inline void Update( void )
		{
			glLoadIdentity();
			gluLookAt( camx , camy , camz , pointx , pointy , pointz , lookx , looky , lookz );
		}

		void Move();
		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();

		void Rotate( float a );

		void Zoom( float d );
		
		int ConvertCoordinates( int x , int y , float & , float & );

		int ScrollOver( float x , float y );

};

#endif			/*	__GAME_CAMERA_H__	*/

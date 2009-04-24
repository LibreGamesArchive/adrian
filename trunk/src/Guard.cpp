#include "Guard.h"
#include "globals.h"

#define		PI		3.141

Guard::Guard( char * filename , int texid , float x1 , float y1 , float x2 , float y2 , float speed ,float botangle,int no)
	: Md2( filename )
{
    float tempx,tempy;
    block_convert(tempx,tempy,x1,y1);
    x1 = tempx;
    y1 = tempy;
    block_convert(tempx,tempy,x2,y2);
    x2 = tempx;
	y2 = tempy;

	patrolCoords[0] = x1;
	patrolCoords[1] = y1;
	patrolCoords[2] = x2;
	patrolCoords[3] = y2;

	this->speed = speed;
	facingAngle = botangle;

	guardNo = no;
	
    {
        fov = 250;
        fovAngle = 60;
        fovInterval = (PI/2.0 - PI*fovAngle/180.0)/10.0;
        fovEnd = PI*fovAngle/180.0;
        fovStart = 2*fovEnd;
		fovID = 3;
    }

	PanelTexId = texid;	

	Initialize();
}

Guard::~Guard()
{
}

void Guard::Initialize( )
{
	if((patrolCoords[0] ==  patrolCoords[2]) && (patrolCoords[1] == patrolCoords[3]))
		Stand(patrolCoords[0], patrolCoords[1]);
	else
		Patrol( patrolCoords[0], patrolCoords[1], patrolCoords[2], patrolCoords[3]);

	Alive = true;
	showbeam = true;
	selected = false;
}

int Guard::Run( float dx , float dy )
{
	Compute( dx , dy );

	destx = dx;
	desty = dy;
	
	if( status != RUNNING )
	{
		status = RUNNING;
		setAnimation( RUN );
	}

	return 0;
}

void Guard::Death( void )
{
    status = DEAD;
    DeathFrameCount = getNumFrames( DEATH );
    setAnimation( DEATH );
}


int Guard::Stand( float x, float y)
{
	Md2::x = curx = x;
	Md2::y = 25;
	Md2::z = cury = y;
	status = STANDING;
	setAnimation( STAND );
}

int Guard::Attack( float x , float y )
{
	status = ATTACKING;
	AttackFrameCount = 20*getNumFrames( ATTACK );
	setAnimation( ATTACK );
}

int Guard::NextMove( void )
{
	switch( status )
	{
		case STANDING:
			return 0;
        case ATTACKING:
       		if( AttackFrameCount == 0 );
				Stand(patrolCoords[0], patrolCoords[1]);
            return 0;
        case DEAD:
            if( DeathFrameCount == 0 )
            	Alive = false;
            return 0;

		case PATROL:
			{
				if( curx <= destx + speed && curx >= destx - speed )
				{
					if( cury <= desty + speed && cury >= desty - speed )
					{
						curx = destx;
						cury = desty;
						if( onwardPatrol )
						{
							destx = patrolCoords[0];
							desty = patrolCoords[1];
							onwardPatrol = false;
							Compute( destx , desty );
						}
						else
						{
							destx = patrolCoords[2];
							desty = patrolCoords[3];
							onwardPatrol = true;
							Compute( destx , desty );
						}
						
						return 0;
					}
				}
				if( movingright )
					curx = curx + speed * costheta;
				else
					curx = curx - speed * costheta;
				if ( movingup )
					cury = cury + speed * sintheta;
				else
					cury = cury - speed * sintheta;


				x = curx;
				z = cury;
				return 0;
			}
		case RUNNING:
			{
				if( curx <= destx + speed && curx >= destx - speed )
				{
					if( cury <= desty + speed && cury >= desty - speed )
					{
						curx = destx;
						cury = desty;
						Stand(patrolCoords[0], patrolCoords[1]);
						return 0;
					}
				}
				if( movingright )
					curx = curx + speed * costheta;
				else
					curx = curx - speed * costheta;
				if ( movingup )
					cury = cury + speed * sintheta;
				else
					cury = cury - speed * sintheta;


				x = curx;
				z = cury;
				break;
			}
	}
	return 0;
}

int Guard::Patrol( float x1 , float y1 , float x2 , float y2 )
{
//	Run( x1 , y1 );
	curx = x1;
	cury = y1;
	destx = patrolCoords[2];
	desty = patrolCoords[3];
	Compute( destx , desty );
	onwardPatrol = true;
	status = PATROL;
	setAnimation( RUN );
	return 0;
}

int Guard::Compute( float &dx , float &dy )
{
	float newx;
	float newy;

	if( dx == curx )
		dx -= 0.00001;
		
	float m = (dy-cury)/(dx-curx);

	
	costheta = 1.0/sqrt(1+m*m);
	sintheta = m*costheta;


	movingright = (dx - curx >= 0)?true:false;
	movingup = (dy - cury >= 0)?true:false;

	
	if( sintheta < 0 )
		sintheta = -sintheta;

	if( costheta < 0 )
		costheta = -costheta;

	if( movingright )
		facingAngle = (atan( m )*180.0)/PI;
	else
		facingAngle = 180 + ((atan( m )*180.0)/PI);

	if( dx < -map->length/2 || dy < -map->breadth/2 || dx > map->length/2 || dy > map->breadth/2 )
	{
		printf("Guard is outdise the MAP!\n");

		if( movingright )
		{
			newy = dy + m * (map->length/2 - dx);
			newx = map->length/2;
		}
		else
		{
			newy = dy - m * (dx + map->length/2);
			newx = -map->length/2;
		}

		if( newy > map->breadth/2 )
		{
			newx = (map->breadth/2 - dy)/m + dx;
			newy = map->breadth/2;
		}
		else if( newy < -map->breadth/2 )
		{
			newx = dx - (map->breadth/2 + dy)/m;
			newy = -map->breadth/2;
		}

		dx = newx;
		dy = newy;
		
		return -1;
	}
	
}

bool Guard::mouseOverMe(float x,float y)
{
    float distance;
    float tempx = curx-x,tempy=cury-y;

    distance = tempx*tempx + tempy*tempy;

    if(distance < 625)
        return true;
    else
        return false;
}

void Guard::Fov( void)
{
    float ptx1,pty1;
	float tptx1,tpty1;

    glEnable( GL_ALPHA_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glAlphaFunc( GL_GEQUAL, 0.0 );
    glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, fovID );	
	
    glBegin(GL_POLYGON);

		glTexCoord2f(0.5,0.5);
   		glVertex3f(0,0,0);
	
	    for(float i = fovStart; i >= fovEnd; i-= fovInterval)
    	{
			tptx1 = sin(i);
			tpty1 = cos(i);
        	ptx1 = fov*tptx1;
	        pty1 = fov*tpty1;
			glTexCoord2f(0.5+0.5*tptx1,0.5+0.5*tpty1);
        	glVertex3f(ptx1,0,pty1);
	    }
    	glEnd();

    glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

bool Guard::checkIntruder(float x, float y)
{
    float distance;
    float tempx = curx-x,tempy=cury-y;
    float m;
    float LineAngle;

    m = (y - cury)/(x - curx);
    LineAngle = atan(m)*180/PI;

    if((curx - x) > 0)
        LineAngle += 180;

    distance = tempx*tempx + tempy*tempy;
    if(distance < fov*fov)
    {
		if(distance < map->blocksize*map->blocksize)
			return true;
        if((LineAngle > facingAngle - 30 )&&(LineAngle < facingAngle + 30))
            return true;
        else
            return false;
    }
    else
        return false;
}

bool Guard::inRange(float x, float y,float Angle)
{
    float distance;
    float tempx = curx-x,tempy=cury-y;
    float m;
    float LineAngle;

    m = (y - cury)/(x - curx);
    LineAngle = atan(m)*180/PI;

    if((curx - x) < 0)
        LineAngle += 180;

    distance = tempx*tempx + tempy*tempy;

    if(distance < hero->fov*hero->fov)
    {
        if((LineAngle > Angle -30)&&(LineAngle < Angle + 30))
            return true;
        else
            return false;
    }
    else
        return false;

}

void Guard::Render( void )
{
    if ( !Alive )
        return ;

        float w=25;
        float w2=w/1.414;
        float h=3;
        glColor3f( 1 , 0  , 0 );
        glBegin( GL_POLYGON );
            glVertex3f( curx+w , h , cury );
            glVertex3f( curx+w2 , h , cury+w2 );
            glVertex3f( curx , h , cury+w );
            glVertex3f( curx-w2 , h , cury+w2 );
            glVertex3f( curx-w , h , cury );
            glVertex3f( curx-w2 , h , cury-w2 );
            glVertex3f( curx , h , cury-w );
            glVertex3f( curx+w2 , h , cury-w2 );
        glEnd();
    if((selected)||(showbeam))
    {
    	glColor4f(1.0,0,0,1);
	    glPushMatrix();
    		glTranslatef(curx,0.05*(guardNo+1),cury);
	    	glRotatef(-facingAngle,0,1,0);
		    Fov();
	    glPopMatrix();
    }
    glColor3f( 1.0, 1.0, 1.0 );
    render();
    if( DeathFrameCount > 0 )
        DeathFrameCount-=1;

}

void Guard::Dump( void )
{
	printf("HERO: currentpos (%f , %f)\n",curx,cury);
}

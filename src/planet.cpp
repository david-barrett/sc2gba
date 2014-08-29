#include "keypad.h"     //button registers
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"
#include "bg.h"

#include<math.h>
#include<stdlib.h>

extern u16* OAM;
extern pOAMEntry sprites;
extern pRotData rotData;
extern int* KEYS;

extern s8 pilot;


extern pAsteroid asteroids;

extern int turn;

extern s16 planet_screenx,planet_screeny;

extern double scale;

extern s32 zoom,screenx,screeny;

//extern s32 planet.xpos,planet.ypos;
extern Object planet;

#define GRAVITY_THRESHOLD (s16)255

void CreateAsteroid(int i)
{

	asteroids[i].object.life=1;

	if (i==0)
	{
	asteroids[i].object.xpos=screenx+50;
	asteroids[i].object.ypos=screeny+340;
	asteroids[i].object.xspeed=-2;
	asteroids[i].object.yspeed=-3;
	}
	else if (i==1)
	{
	asteroids[i].object.xpos=screenx+50;
	asteroids[i].object.ypos=screeny-340;
	asteroids[i].object.xspeed=-3;
	asteroids[i].object.yspeed=+3;
	}
	else if (i==2)
	{
		asteroids[i].object.xpos=screenx+500;
		asteroids[i].object.ypos=screeny+50;
		asteroids[i].object.xspeed=-1;
		asteroids[i].object.yspeed=3;
	}
	else if (i==3)
	{
		asteroids[i].object.xpos=screenx-500;
		asteroids[i].object.ypos=screeny-50;
		asteroids[i].object.xspeed=3;
		asteroids[i].object.yspeed=-1;
	}
	else if (i==4)
	{
		asteroids[i].object.xpos=screenx+500;
		asteroids[i].object.ypos=screeny+340;
		asteroids[i].object.xspeed=-2;
		asteroids[i].object.yspeed=-3;
	}


	if (asteroids[i].object.xpos>planet.xpos-32&&asteroids[i].object.xpos<planet.xpos+32&&
		asteroids[i].object.ypos>planet.xpos-32&&asteroids[i].object.ypos<planet.ypos+32)
	{
		asteroids[i].object.xpos=screenx+((asteroids[i].object.xpos-screenx)*-1);
		asteroids[i].object.ypos=screeny+((asteroids[i].object.ypos-screeny)*-1);
		asteroids[i].object.xspeed*=-1;
		asteroids[i].object.yspeed*=-1;

	}



	sprites[26+i].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
    sprites[26+i].attribute1 =SIZE_32 | ROTDATA(26+i) | 240;
    sprites[26+i].attribute2 = SpriteAsteroidStart | PRIORITY(2);
}

void BounceAsteroid(int i)
{
	asteroids[i].object.xspeed*=-1;
	asteroids[i].object.yspeed*=-1;
}

void ProcessAsteroids(pPlayer p1,pPlayer p2)
{
	s16 dist;
	for (int i=0;i<5;i++)
	{

		if (asteroids[i].object.life==-1)
		{
			CreateAsteroid(i);
		}
		if (asteroids[i].object.life==0)
		{
			asteroids[i].object.xspeed=0;
			asteroids[i].object.yspeed=0;
			sprites[26+i].attribute2 = SpriteAsteroidStart+32 | PRIORITY(2);
			asteroids[i].object.life=-1;
		}

		//if out of bounds flip

		if (asteroids[i].object.life>0)
		{
		if (asteroids[i].object.xpos<screenx-(512))
		{
			asteroids[i].object.xpos+=(992);
			asteroids[i].object.xspeed*=-1;
		}
		else if (asteroids[i].object.xpos>screenx+512)
		{
			asteroids[i].object.xpos-=992;
			asteroids[i].object.xspeed*=-1;
		}

		if (asteroids[i].object.ypos<screeny-352)
		{
			asteroids[i].object.yspeed*=-1;
			asteroids[i].object.ypos+=672;
		}
		else if (asteroids[i].object.ypos>screeny+352)
		{
			asteroids[i].object.yspeed*=-1;
			asteroids[i].object.ypos-=672;
		}
		RotateSprite(i+26, 0, zoom,zoom);
		drawOnScreen(&asteroids[i].object.xscreen,&asteroids[i].object.yscreen,asteroids[i].object.xpos,asteroids[i].object.ypos,screenx,screeny,32);
		MoveSprite(&sprites[i+26],asteroids[i].object.xscreen,asteroids[i].object.yscreen);
		}//if life>0
	
	}//end for loop
}//end func

void DrawPlanet()
{
	if (planet.xpos<screencentrex-(960+128))
		planet.xpos+=(1920);

	else if (planet.xpos>screencentrex+(960+128))
		planet.xpos-=(1920);


	if (planet.ypos<screencentrey-(640+128))
		planet.ypos+=(1280);
	else if (planet.ypos>screencentrey+(640+128))
		planet.ypos-=(1280);


	drawOnScreen(&planet_screenx,&planet_screeny,planet.xpos,planet.ypos,screenx,screeny,64);
	RotateSprite(31, 0, zoom, zoom);
	sprites[31].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | planet_screeny;	//setup sprite info, 256 colour, shape and y-coord
	sprites[31].attribute1 =SIZE_64 | ROTDATA(31) | planet_screenx;
	sprites[31].attribute2 = PlanetSprite | PRIORITY(1);
}

int
CalculateGravity (pObject ElementPtr,pObject TestElementPtr)
{
	//EmementPtr = Planet!
	s8 retval, HasGravity,TestHasGravity;

	retval = 0;
	HasGravity = 1;//planet!

	if (TestElementPtr->type==PLAYER)
	{
		pPlayer p =(pPlayer)(TestElementPtr->parent);
		if (p->ship==SKIFF)
			return 0;
	}

		
		if (TestElementPtr != ElementPtr
				&& (TestElementPtr->ignorecollision==0))
				//&& (TestHasGravity =
				//GRAVITY_MASS (TestElementPtr->mass_points + 1)) != HasGravity))
		{
		s16 abs_dx, abs_dy, dx, dy;

			
			dx = ElementPtr->xpos
					- TestElementPtr->xpos;
			dy = ElementPtr->xpos
					- TestElementPtr->xpos;
			
			abs_dx = dx >= 0 ? dx : -dx;
			abs_dy = dy >= 0 ? dy : -dy;
			if (abs_dx <= GRAVITY_THRESHOLD	&& abs_dy <= GRAVITY_THRESHOLD)
			{
				s32 dist_squared;

				dist_squared = (abs_dx * abs_dx)
						+ (abs_dy * abs_dy);
				if (dist_squared <= (GRAVITY_THRESHOLD	* GRAVITY_THRESHOLD))
				{

				//	if (TestHasGravity)
				//	{
				//		retval = 1;						
				//	}
					//else
					{
						s8 angle = FindAngle(TestElementPtr->xpos,TestElementPtr->ypos,
							ElementPtr->xpos,ElementPtr->ypos);
						
						TestElementPtr->xspeed+=((s32)(SIN[angle]*4)>>8);
						TestElementPtr->yspeed+=((s32)(COS[angle]*4)>>8);
/*
						if (TestElementPtr->type== PLAYER)
						{
							pPlayer StarShipPtr=(pPlayer)TestElementPtr->parent;
						
							StarShipPtr->cur_status_flags &= ~SHIP_AT_MAX_SPEED;
							StarShipPtr->cur_status_flags |= SHIP_IN_GRAVITY_WELL;
						}
						*/
					}
				}
			}
		

	}

	return (retval);
}


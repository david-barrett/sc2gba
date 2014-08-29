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


void CreateAsteroid(int i)
{

	asteroids[i].life=1;

	if (i==0)
	{
	asteroids[i].xpos=screenx+50;
	asteroids[i].ypos=screeny+340;
	asteroids[i].xspeed=-2;
	asteroids[i].yspeed=-3;
	}
	else if (i==1)
	{
	asteroids[i].xpos=screenx+50;
	asteroids[i].ypos=screeny-340;
	asteroids[i].xspeed=-3;
	asteroids[i].yspeed=+3;
	}
	else if (i==2)
	{
		asteroids[i].xpos=screenx+500;
		asteroids[i].ypos=screeny+50;
		asteroids[i].xspeed=-1;
		asteroids[i].yspeed=3;
	}
	else if (i==3)
	{
		asteroids[i].xpos=screenx-500;
		asteroids[i].ypos=screeny-50;
		asteroids[i].xspeed=3;
		asteroids[i].yspeed=-1;
	}
	else if (i==4)
	{
		asteroids[i].xpos=screenx+500;
		asteroids[i].ypos=screeny+340;
		asteroids[i].xspeed=-2;
		asteroids[i].yspeed=-3;
	}


	if (asteroids[i].xpos>planetx-32&&asteroids[i].xpos<planetx+32&&
		asteroids[i].ypos>planety-32&&asteroids[i].ypos<planety+32)
	{
		asteroids[i].xpos=screenx+((asteroids[i].xpos-screenx)*-1);
		asteroids[i].ypos=screeny+((asteroids[i].ypos-screeny)*-1);
		asteroids[i].xspeed*=-1;
		asteroids[i].yspeed*=-1;

	}



	sprites[26+i].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
    sprites[26+i].attribute1 = SIZE_32 | ROTDATA(26+i) | 240;
    sprites[26+i].attribute2 = SpriteAsteroidStart | PRIORITY(2);
}

void BounceAsteroid(int i)
{
	asteroids[i].xspeed*=-1;
	asteroids[i].xspeed*=-1;
}

void ProcessAsteroids(pPlayer p1,pPlayer p2)
{
	s16 dist;
	for (int i=0;i<5;i++)
	{

		if (asteroids[i].life==-1)
		{
			CreateAsteroid(i);
		}
		if (asteroids[i].life==0)
		{
			asteroids[i].xspeed=0;
			asteroids[i].yspeed=0;
			sprites[26+i].attribute2 = SpriteAsteroidStart+32 | PRIORITY(2);
			asteroids[i].life=-1;
		}


/* OLD
		if (asteroids[i].life==0)
		{
			CreateAsteroid(i);
		}
*/
		//if out of bounds flip

		if (asteroids[i].life>0)
		{
		if (asteroids[i].xpos<screenx-600)
		{
			asteroids[i].xpos+=1100;
			asteroids[i].xspeed*=-1;
		}
		else if (asteroids[i].xpos>screenx+600)
		{
			asteroids[i].xpos-=1100;
			asteroids[i].xspeed*=-1;
		}

		if (asteroids[i].ypos<screeny-350)
		{
			asteroids[i].xspeed*=-1;
			asteroids[i].ypos+=700;
		}
		else if (asteroids[i].xpos>screenx+350)
		{
			asteroids[i].xspeed*=-1;
			asteroids[i].ypos-=700;
		}



		//planet
		dist=distanceBetweenPoints(asteroids[i].xpos,asteroids[i].ypos,planetx,planety);
		if (dist<64)
		{
		//	print("\n ast hit planet");
		//	asteroids[i].life=0;
			BounceAsteroid(i);
		}
		else
		{

		//if asteroid hit player
		dist=distanceBetweenPoints(p1->xpos,p1->ypos,asteroids[i].xpos,asteroids[i].ypos);
		if (dist<(p1->offset+10))
		{
			//ModifyCrew(p1,-1);
			Bounce(p1);

			BounceAsteroid(i);
			#ifdef DEBUG
			print("\n ast hit p1");
			#endif
		}

		dist=distanceBetweenPoints(p2->xpos,p2->ypos,asteroids[i].xpos,asteroids[i].ypos);
		if (dist<(p2->offset+10))
		{
			//ModifyCrew(p2,-1);
			Bounce(p2);
			BounceAsteroid(i);
			#ifdef DEBUG
			print("\n ast hit p2");
			#endif
		}
		//each other
		for (int j=i+1;j<5;j++)
		{
			//if (i!=j)
			{
				dist=distanceBetweenPoints(asteroids[j].xpos,asteroids[j].ypos,asteroids[i].xpos,asteroids[i].ypos);
				if (dist<25)
				{
					BounceAsteroid(i);
					BounceAsteroid(j);
				}
			}
		}
		}//if life >0

		asteroids[i].xpos+=asteroids[i].xspeed;
		asteroids[i].ypos+=asteroids[i].yspeed;

		RotateSprite(i+26, 0, zoom,zoom);
		drawOnScreen(&asteroids[i].xscreen,&asteroids[i].yscreen,asteroids[i].xpos,asteroids[i].ypos,screenx,screeny,32);
		MoveSprite(&sprites[i+26],asteroids[i].xscreen,asteroids[i].yscreen);

	}//not hit planet
	}//end for loop
}//end func

void DrawPlanet()
{
	drawOnScreen(&planet_screenx,&planet_screeny,planetx,planety,screenx,screeny,64);
	RotateSprite(31, 0, zoom, zoom);
	sprites[31].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | planet_screeny;	//setup sprite info, 256 colour, shape and y-coord
	sprites[31].attribute1 = SIZE_64 | ROTDATA(31) | planet_screenx;
	sprites[31].attribute2 = PlanetSprite | PRIORITY(1);
}

void CalcPlanet(pPlayer pl)
{
	s16 dist=distanceBetweenPoints(pl->xpos,pl->ypos,planetx,planety);

	if (dist<(pl->offset+64)/2)
	{
		#ifdef DEBUG
		print("player hit planet");
		#endif
		ModifyCrew(pl,-2,1);
		Bounce(pl);
	}

	//gravity
	if (pl->ship!=SKIFF&&dist<70+(2*pl->mass))//maybe times by mass
	{
		s16 a = FindAngle(pl->xpos,pl->ypos,planetx,planety);

		s32 x = ((pl->mass+1) * (s32)SIN[a])>>9; //((2) * (s32)SIN[a])>>8;
		s32 y = ((pl->mass+1) * (s32)COS[a])>>9;//((2) * (s32)COS[a])>>8;

		pl->xspeed = pl->xspeed + x;
		pl->yspeed = pl->yspeed + y;

	}

}


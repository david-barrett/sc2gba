
//////////////////////////////////////////////////////////////////////////
// File: 	star control gba.cpp                                            	//
// Description: a sc2 clone			//
// Author:	david barrett 	  					//
// Date: 	4th Aug 2004                                      	//
//////////////////////////////////////////////////////////////////////////

#include "gba.h"	//GBA register definitions
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
extern s8 demo;
extern s8 scaled;
extern s8 v3do;


pTrail trails;
pAsteroid asteroids;

int turn;

s16 planet_screenx,planet_screeny;
//extern s32 planet.xpos,planet.ypos;
extern Object planet;



//extern FIXED SIN2[360];	    //Look-Up Tabless for sign and cosign
//extern FIXED COS2[360];





double scale;

s32 zoom,screenx,screeny;

//centre of screen
//note ships draw from TOP_LEFT

s8 yswap=0;
s8 xswap=0;

int CalculateGravity (pObject ElementPtr,pObject TestElementPtr);


char s[100];
#ifdef DEBUG
void
print(char *s)
{

    asm volatile ("mov r0, %0;" "swi 0xff0000;":        // warning! will crash on hardware!
                  :"r" (s):"r0");

}


void
printi(s32 s)
{
  //  s8 n = s%10;
    switch (s)
    {
		case 0:
			print("0");
			break;

		case 1:
					print("1");
			break;
		case 2:
					print("2");
			break;
		case 3:
					print("3");
			break;
	case 4:
				print("4");
			break;

		case 5:
					print("5");
			break;
	case 6:
				print("6");
			break;
	case 7:
				print("7");
			break;
	case 8:
				print("8");
			break;
	case 9:
				print("9");
			break;
	default:
		print("x");//somethings wrong
}


}

void
print(s32 num)
{
	if (num<0)
	{
		print("-");
		num*=1;
	}
	int digits=10;
	while(digits>1)
		{
			int off=(int)pow(double(10),double(digits-1));//(10^(digits-1));

			if (num>=off)
			{
				int tmp=num/off;

				printi(tmp);
				num=num-(tmp*off);
			}
			else
				printi(0);
			digits--;


	}
	printi(num);

}
#endif

void swap(s32 *one, s32 *two)
{
	s32 three;
	three=*one;
	*one=*two;
	*two=three;
}


//Copy our sprite array to OAM
void CopyOAM()
{
	/*
	u16 loop;
	u16* temp;
	temp = (u16*)sprites;
	for(loop = 0; loop < 128*4; loop++)
	{
		OAM[loop] = temp[loop];
	}
	*/
	//try this - eventually replace CopyOAM with this...
	REG_DMA3SAD = (u32)sprites;
	REG_DMA3DAD = (u32)OAM;
	REG_DMA3CNT = 128 * 4 |DMA_16NOW;
}

//Set sprites to off screen
void InitializeSprites()
{
	u16 loop;
	for(loop = 0; loop < 128 ;loop++)
	{
		sprites[loop].attribute0 = 160;  //y to > 159
		sprites[loop].attribute1 = 240; //x to > 239
		//sprites[loop].attribute2 = 0;
	}
}

//wait for the screen to stop drawing
void WaitForVsync()
{
	while((volatile u16)REG_VCOUNT != 160){}
}




void DrawExplosion(pWeapon w)
{
	s16 life=w->object.life;

	s16 off=0;

	if (life<3)
	{
		if (life==2)
			off=2;
		w->object.size=8;
		sprites[w->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | w->object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[w->sprite].attribute1 =SIZE_8 | ROTDATA(w->sprite) | w->object.xscreen;
    	sprites[w->sprite].attribute2 = FireSprite1+off | PRIORITY(0);
	}
	else if (life<=5)
	{
		off=4;
		if (life==4)
			off=12;
		else if (life==5)
			off=20;
		w->object.size=16;
		sprites[w->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | w->object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[w->sprite].attribute1 =SIZE_16 | ROTDATA(w->sprite) | w->object.xscreen;
	    sprites[w->sprite].attribute2 = FireSprite1+off | PRIORITY(0);
	}

}

void MoveExplosion(pWeapon w)
{
	w->object.life--;
	DrawExplosion(w);
}

void CreateExplosion(pWeapon w,s16 life)
{
	w->object.life=life;;
	w->object.xspeed=0;
	w->object.yspeed=0;
	w->type=EXP;
	w->movefunc=&MoveExplosion;
	w->object.ignorecollision=1;
	w->object.angle=0;
	if (life<3)
		w->object.size=8;
	else
		w->object.size=16;


	drawOnScreen(&(w->object.xscreen),&(w->object.yscreen),
			w->object.xpos,w->object.ypos,screenx,screeny,w->object.size);
	MoveSprite(&sprites[w->sprite], w->object.xscreen, w->object.yscreen);

	DrawExplosion(w);
}



int DetectWeaponToShip(pPlayer p,pWeapon w)
{
	//square (w.object.xpos-4,w.object.ypos-4), (w.object.xpos+4,w.object.ypos+4)
	//square (p.object.xpos-16,w.object.ypos-16), (w.object.xpos+16,w.object.ypos+16)

	s16 offset=p->offset;///2;

	//tmp
//	offset+=(w->object.size>>2);

	if (w->object.xpos>p->object.xpos-offset&&w->object.xpos<p->object.xpos+offset&&w->object.ypos>p->object.ypos-offset&&w->object.ypos<p->object.ypos+offset)
	{
		return 1;
	}
	return 0;

}

int DetectWeaponToWeapon(pWeapon tar,pWeapon w)
{
	//square (w.object.xpos-4,w.object.ypos-4), (w.object.xpos+4,w.object.ypos+4)
	//square (p.object.xpos-16,w.object.ypos-16), (w.object.xpos+16,w.object.ypos+16)

	s16 offset=tar->object.size>>1;


	if (w->object.xpos>tar->object.xpos-offset&&w->object.xpos<tar->object.xpos+offset&&w->object.ypos>tar->object.ypos-offset&&w->object.ypos<tar->object.ypos+offset)
	{
		return 1;
	}
	return 0;

}




//move the sprite
void MoveSprite(OAMEntry* sp, int x, int y)
{
	sp->attribute1 = sp->attribute1 & 0xFE00;  //clear the old x value
	sp->attribute1 = sp->attribute1 | x;

	sp->attribute0 = sp->attribute0 & 0xFF00;  //clear the old y value
	sp->attribute0 = sp->attribute0 | y;
}

void MoveOffscreen(OAMEntry* sp)
{
	MoveSprite(sp,-64,-64);
}

s16 FindAngle(s32 x,s32 y, s32 ox, s32 oy)
{
	s16 angle,final;

	if (x==ox)
	{
		if (y<oy)
			return 180;
		else
			return 0;
	}
	if (y==oy)
	{
		if (x>ox)
			return 270;
		else
			return 90;
	}

	double r = double(y-oy)/double(x-ox);
	//double r = double(oy-y)/double(ox-x);

	angle = (s16) (atan(r) * 180 / PI); //maybe find a way to calc this?
	if (angle<0) //make positive
		angle=angle*-1;

	//normalise to nearest 15.
	//big assumption:tan only returns 0-90?
	//0 =0-7
	//15 = 8-22
	//30 = 23-37
	//45 = 38-52
	//60 = 53-67
	//75 = 68-82
	//90 = 83-90

	if (angle<=11)
		angle=0;
	else if (angle>=12&&angle<=33)
		angle=45>>1;//22.5
	else if (angle>=34&&angle<=56)
		angle=45;
	else if (angle>=57&&angle<=78)
		angle=45+(45>>1);//67.5
	else if (angle>=79)
		angle=90;




	if (ox>x&&oy>y)
		final=90+angle;//right?
	else if (ox>x&&oy<=y)
		final=90-angle;//right
	else if (ox<=x&&oy>y)
		final=270-angle;
	else if (ox<=x&&oy<=y)
		final=270+angle;//right





	return (final);
}

int InRange(s32 xpos,s32 ypos,s32 txpos,s32 typos,s16 range)
{

	return (distanceBetweenPoints(xpos,ypos,txpos,typos)<range);

	/*
	if (object.xpos>tobject.xpos-range&&object.xpos<tobject.xpos+range&&object.ypos>tobject.ypos-range&&object.ypos<tobject.ypos+range)
	{
		return 1;
	}
	return 0;
	*/
}

void MoveBullet(pWeapon w)
{
		if (w->object.life==0)
		{
			sprites[w->sprite].attribute0 = 160;  //y to > 159
			sprites[w->sprite].attribute1 = 240; //x to > 239
			sprites[w->sprite].attribute2 = 0;
			w->object.life--;
		}
		else if (w->object.life>0)
		{
			if (w->movefunc!=0)
				w->movefunc(w);
			else
				w->object.life--;
			drawOnScreen(&(w->object.xscreen),&(w->object.yscreen),
				w->object.xpos,w->object.ypos,screenx,screeny,w->object.size);
			MoveSprite(&sprites[w->sprite], w->object.xscreen, w->object.yscreen);
			RotateSprite(w->sprite, w->object.angle, zoom,zoom);
		}

}


void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale)
{

	s32 pa,pb,pc,pd;

	pa = ((x_scale) * (s32)COS[angle])>>8;    //(do my fixed point multiplies and shift back down)
	pb = ((y_scale) * (s32)SIN[angle])>>8;
	pc = ((x_scale) * (s32)-SIN[angle])>>8;
	pd = ((y_scale) * (s32)COS[angle])>>8;


	rotData[rotDataIndex].pa = pa;  //put them in my data struct
	rotData[rotDataIndex].pb = pb;
	rotData[rotDataIndex].pc = pc;
	rotData[rotDataIndex].pd = pd;
}

s32 distanceBetweenPoints(s32 xpos1,s32 ypos1,s32 xpos2,s32 ypos2)
{
	s32 x,y,z;
	if (xpos1>xpos2)
		x=xpos1-xpos2;
	else
		x=xpos2-xpos1;
	if (ypos1>ypos2)
		y=ypos1-ypos2;
	else
		y=ypos2-ypos1;

	//old way
	//z = (s32)sqrt((x*x)+(y*y));
	//return z;
	
		
	u32 min, max, approx;

   if ( x < y )
   {
      min = x;
      max = y;
   } else {
      min = y;
      max = x;
   }

   approx = ( max * 1007 ) + ( min * 441 );
   if ( max < ( min << 4 ))
      approx -= ( max * 40 );

   // add 512 for proper rounding
   return (( approx + 512 ) >> 10 );


}

inline void findCentre(s32 *x,s32 *y,pPlayer p1,pPlayer p2)
{
	*x = ((p1->object.xpos+p2->object.xpos)/2);//-32;
	*y = ((p1->object.ypos+p2->object.ypos)/2);//-32;
}

void shifteverythingx(pPlayer p1,pPlayer p2,int shift)
{
	//shift everything except the planet

	p1->object.xpos+=shift;
	p2->object.xpos+=shift;
	screenx+=shift;
	int i;
	for (i=0;i<12;i++)
	{
		p1->weapon[i].object.xpos+=shift;
		p2->weapon[i].object.xpos+=shift;
	}
	for (i=0;i<10;i++)
	{
		trails[i].object.xpos+=shift;
	}

	//asteroids
	for (i=0;i<5;i++)
	{
		asteroids[i].object.xpos+=shift;
		/*
		if (asteroids[i].object.xpos<screenx-(960+64))
						{
							asteroids[i].object.xpos+=(1919+64);

						}
						else if (asteroids[i].object.xpos>screenx+(960+64))
						{
							asteroids[i].object.xpos-=(1919+64);

						}
		*/
	}

	//planet
	planet.xpos+=shift;
}

void shifteverythingy(pPlayer p1,pPlayer p2,int shift)
{
	//shift everything except the planet

	p1->object.ypos+=shift;
	p2->object.ypos+=shift;
	screeny+=shift;
	int i;
	for (i=0;i<12;i++)
	{
		p1->weapon[i].object.ypos+=shift;
		p2->weapon[i].object.ypos+=shift;
	}
	for (i=0;i<10;i++)
	{
		trails[i].object.ypos+=shift;
	}
	//asteroids
	for (i=0;i<5;i++)
	{
		asteroids[i].object.ypos+=shift;
		/*
				if (asteroids[i].object.ypos<screencentrey-(640+64))
				{

					asteroids[i].object.ypos+=(1279+64);
				}
				else if (asteroids[i].object.ypos>screencentrex+(640+64))
				{

					asteroids[i].object.ypos-=(1279+64);

		}*/
	}

	//planet
	planet.ypos+=shift;


}


void setScreen(pPlayer p1,pPlayer p2,Bg* bg0,Bg* bg1)
{
	s32 x, y, changex,changey;
	findCentre(&x,&y,p1,p2);

	xswap--;
	if (xswap<0)
		xswap=0;
	yswap--;
	if (yswap<0)
		yswap=0;

	changex = screenx-x;
	changey = screeny-y;
	/*
	bg0.x_scroll-=2*changex;
    bg1.x_scroll-=changex;

	bg0.y_scroll-=2*changey;
    bg1.y_scroll-=changey;
*/

	bg0->x_scroll-=2*changex;
    bg1->x_scroll-=changex;

	bg0->y_scroll-=2*changey;
    bg1->y_scroll-=changey;

	screenx=x;
	screeny=y;

	x=screenx-50;
	y=screenx-50;

	s32 d= distanceBetweenPoints(p1->object.xpos,p1->object.ypos,p2->object.xpos,p2->object.ypos);
	//Calcuate zoom level


	if (d<160)
	{
		zoom = 1<<8; //256
		scale = 1;
	}
	// PC ZOOM
	else if (d < 320&&!scaled) //needs calc
	{
		zoom=1<<9;//512
		scale = 0.5;

	}
	//3DO zoom
	else if (scaled&&d<320)
	{
		zoom=256+(s16)(double(d-160)*3.2);
		scale=0.9-(double(d-160)*0.004);
	}	
	
	else //if (d>=320)
	{
		zoom=768;
		scale = 0.25;
		//scale=4;

		//if reached the end of space swap ships
/*
		s32 xd = p1->object.xpos-p2->object.xpos;
		if (xd<-960 || xd>960)
		{
			if (screenx>1000)
				shifteverythingx(p1,p2,-500);
			else
				shifteverythingx(p1,p2,500);
			swap(&p1->object.xpos,&p2->object.xpos);
		}
		s32 yd = p1->object.ypos-p2->object.ypos;
		if (yd<-640 || yd>640)
		{
			swap(&p1->object.ypos,&p2->object.ypos);
			if (screeny>1000)
				shifteverythingy(p1,p2,-500);
			else
				shifteverythingy(p1,p2,500);
		}


			// want to reset so values dont overflow

			if(screenx<50)
				shifteverythingx(p1,p2,1600);
			if (screenx>1950)
				shifteverythingx(p1,p2,-1600);
			if(screeny<50)
				shifteverythingy(p1,p2,1600);
			if (screeny>1950)
				shifteverythingy(p1,p2,-1600);

				*/
// NEW WAY ??


		s32 xd = p1->object.xpos-p2->object.xpos;
		s32 yd = p1->object.ypos-p2->object.ypos;
		if (xswap==0&&(xd<-960||xd>960))
		{
			xswap=5;
			swap(&p1->object.xpos,&p2->object.xpos);
			#ifdef DEBUG
			print("\nscreen x=");
			print(screenx);
			#endif

			if (screenx<screencentrex)
			{
				p1->object.xpos+=960;
				p2->object.xpos+=960;
				screenx+=960;
			//	shifteverythingx(p1,p2,959);
			}
			else
			{
				screenx-=960;
				p1->object.xpos-=960;
				p2->object.xpos-=960;
			//	shifteverythingx(p1,p2,-959);
			}


		}

		if (yswap==0&&(yd<-640 || yd>640))
		{
			yswap=5;
			swap(&p1->object.ypos,&p2->object.ypos);
			#ifdef DEBUG
			print("\nbefore flipabout to add/minus 500 screen y=");
			print(screeny);
			#endif

			if (screeny<screencentrey)
			{

				p1->object.ypos+=640;
				p2->object.ypos+=640;
				screeny+=640;
		//		shifteverythingy(p1,p2,639);
			}
			else
			{

				screeny-=640;
				p1->object.ypos-=640;
				p2->object.ypos-=640;
				//shifteverythingy(p1,p2,-639);
			}
			#ifdef DEBUG
			print("\nafter flip y=");
			print(screeny);
			#endif

		}

		if (screenx<screencentrex-960)
		{
			#ifdef DEBUG
			print("\nshift x+");
			print("\nscreen x=");
			print(screenx);
			#endif
			shifteverythingx(p1,p2,1700);
		}
		else if (screenx>screencentrex+960)
		{
			#ifdef DEBUG
			print("\nshift x-");
			print("\nscreen x=");
			print(screenx);
			#endif
			shifteverythingx(p1,p2,-1700);
		}

		if (screeny<screencentrey-640)
		{
			#ifdef DEBUG
			print("\nshift y+");
				print("\nscreen now y=");
			print(screeny);
			#endif
			shifteverythingy(p1,p2,1100);
		}
		else if (screeny>screencentrey+640)
		{
			#ifdef DEBUG
			print("\nshift y-");
				print("\nscreen now y=");
			print(screeny);
			#endif
			shifteverythingy(p1,p2,-1100);
		}









	}




	drawOnScreen(&p1->object.xscreen,&p1->object.yscreen,p1->object.xpos,p1->object.ypos,screenx,screeny,32,1);
	drawOnScreen(&p2->object.xscreen,&p2->object.yscreen,p2->object.xpos,p2->object.ypos,screenx,screeny,32,1);
}

void drawOnScreen(s16* x,s16* y,s32 xpos,s32 ypos, s16 screenx, s16 yscreen,s16 size,s16 player)
{
//	*x = (s16)(((xpos-screenx)*scale)+centrex->object.size/scale));
//	*y = (s16)(((ypos-screeny)*scale)+centrey->object.size/scale));


	*x = (s16)(((xpos-screenx)*scale)+centrex-(size));
	*y = (s16)(((ypos-screeny)*scale)+centrey-(size));

//	*x = (s16)(((xpos-screenx)*scale)+centrex);
//	*y = (s16)(((ypos-screeny)*scale)+centrey);

	if (!player)
	{
		//dont draw on screen if out of bounds
		if (*x<0-64||*x>240+64||*y<0-64||*y>160+64)
		{
			*x=304;
			*y=224;
		}
	}

	if (*x <0)
		*x+=512;
	if (*y <0)
		*y+=255;



}
/*
int CanHitOpp(pPlayer pl)
{
	pPlayer opp = (pPlayer)pl->opp;
	s16 angle = FindAngle(pl->object.xpos,pl->object.ypos,opp->object.xpos,opp->object.ypos);

	s16 a1 = 360.object.angle;
	s16 a2 = 360+pl->object.angle;

	s16 x=30;
	if (pl->ship==FURY)
		x=100;
	if (a1>a2-x&&a1<a2+x)//are they roughly at right.object.angle?
	{
		return 1;
	}
	return 0;
}
*/
int TurnAngle(s16 yourangle, s16 desiredangle,s8 precision)
{
	//takes your angle, the angle you want and possilbe a precsion
	//returns 0 if on right path
	//-1 if need to turn left
	//1 if need to turn right

	int diff = yourangle-desiredangle;


	int posdiff=(diff<0)?diff*-1:diff;

	if (posdiff<=precision)
		return 0;

	if (diff<0)
	{
		if (diff<-180)
			return 1;
		else
			return-1;
	}

	//if (your angle>desired object.angle)//else
	else
	{
		if (diff>180)
			return -1;
		else
			return 1;
	}

	return 0;
}

void
collide (pObject ElementPtr0, pObject ElementPtr1)
{
	s16 speed,dx_rel, dy_rel;
	s16 TravelAngle0, TravelAngle1, ImpactAngle0, ImpactAngle1;
	s16 RelTravelAngle, Directness;

	dx_rel = ElementPtr0->xpos
			- ElementPtr1->xpos;
	dy_rel = ElementPtr0->ypos
			- ElementPtr1->ypos;
	ImpactAngle0 = FindAngle (0,0,dx_rel, dy_rel);
	ImpactAngle1 = ModifyAngle(ImpactAngle0,180);

	
	TravelAngle0 = FindAngle (0,0,ElementPtr0->xspeed,ElementPtr0->yspeed);	
	TravelAngle1 = FindAngle (0,0,ElementPtr1->xspeed,ElementPtr1->yspeed);
	dx_rel = ElementPtr0->xspeed - ElementPtr1->xspeed;
	dy_rel = ElementPtr0->yspeed - ElementPtr1->yspeed;
	RelTravelAngle =  (s16)(atan(double(dx_rel/ dy_rel)) * 180 / PI);
	speed = distanceBetweenPoints(0,0,dx_rel,dy_rel);
	
	Directness = ModifyAngle(RelTravelAngle, -1* ImpactAngle0);
	if (Directness <= 90 || Directness >= 270)
			/* shapes just scraped each other but still collided,
			 * they will collide again unless we fudge it.
			 */
	{
	
		Directness = 180;
		ImpactAngle0 = ModifyAngle(TravelAngle0,180);
		ImpactAngle1 = ModifyAngle(TravelAngle1,180);
	
	}


	if (ElementPtr0->xpos == ElementPtr0->lastxpos
			&& ElementPtr0->ypos == ElementPtr0->lastypos
			&& ElementPtr1->xpos == ElementPtr1->lastypos
			&& ElementPtr1->ypos == ElementPtr1->lastypos)
	{
		if (ElementPtr0->state_flags & ElementPtr1->state_flags & DEFY_PHYSICS)
		{
			ImpactAngle0 = ModifyAngle(TravelAngle0,135);//HALF_CIRCLE - OCTANT);
			ImpactAngle1 = ModifyAngle(TravelAngle1,135);//HALF_CIRCLE - OCTANT);
			ElementPtr0->xspeed=0;
			ElementPtr0->yspeed=0;
			ElementPtr1->xspeed=0;
			ElementPtr1->yspeed=0;
		}
		ElementPtr0->state_flags |= (DEFY_PHYSICS | COLLISION);
		ElementPtr1->state_flags |= (DEFY_PHYSICS | COLLISION);
	}

	{
		s16 mass0, mass1;
		long scalar;

		mass0 = ElementPtr0->mass_points /* << 2 */;
		mass1 = ElementPtr1->mass_points /* << 2 */;
		
		scalar = ((s32)(SIN [Directness]* speed  * mass0 * mass1)>>7);

#ifdef DEBUG
		print("\n scalar=");print(scalar);
		print("\n speed=");print(speed);
		print("\n directness=");print(Directness);
#endif

		if (!GRAVITY_MASS (ElementPtr0->mass_points + 1))
		{
			if (ElementPtr0->type==PLAYER)
			{
				pPlayer StarShipPtr = (pPlayer)ElementPtr0->parent;
				
				//StarShipPtr->cur_status_flags &=
				//		~(SHIP_AT_MAX_SPEED | SHIP_BEYOND_MAX_SPEED);
				if (!(ElementPtr0->state_flags & DEFY_PHYSICS))
				{
					if (StarShipPtr->turn_turn < COLLISION_TURN_WAIT)
						StarShipPtr->turn_turn += COLLISION_TURN_WAIT;
					if (StarShipPtr->thrust_turn < COLLISION_THRUST_WAIT)
						StarShipPtr->thrust_turn += COLLISION_THRUST_WAIT;
				}
			}
			
			speed = (s32)(scalar / ((long)mass0 * (mass0 + mass1)));			

			if (speed<3)
				speed=3;
			else if (speed>20)
				speed=20;
			ElementPtr0->xspeed+=(s32)(SIN[ImpactAngle0]*speed)>>8;
			ElementPtr0->yspeed-=(s32)(COS[ImpactAngle0]*speed)>>8;



			

			#ifdef DEBUG
			print("\nspeed=");print(speed);
			print("\n e0 xspeed=");print(ElementPtr0->xspeed);
			print("\n e0 yspeed=");print(ElementPtr0->yspeed);
			#endif

			
			
		}

		if (!GRAVITY_MASS (ElementPtr1->mass_points + 1))
		{
			if (ElementPtr1->type==PLAYER)
			{
				pPlayer StarShipPtr = (pPlayer)ElementPtr0->parent;
				
				//StarShipPtr->cur_status_flags &=
				//		~(SHIP_AT_MAX_SPEED | SHIP_BEYOND_MAX_SPEED);
				if (!(ElementPtr1->state_flags & DEFY_PHYSICS))
				{
					if (StarShipPtr->turn_turn < COLLISION_TURN_WAIT)
						StarShipPtr->turn_turn += COLLISION_TURN_WAIT;
					if (StarShipPtr->thrust_turn < COLLISION_THRUST_WAIT)
						StarShipPtr->thrust_turn += COLLISION_THRUST_WAIT;
				}
			}

			speed = (s16)(scalar / ((long)mass1 * (mass0 + mass1)));
			if (speed<3)
				speed=3;
			else if (speed>20)
				speed=20;
			ElementPtr1->xspeed+=(s32)(SIN[ImpactAngle1]*speed)>>8;
			ElementPtr1->yspeed-=(s32)(COS[ImpactAngle1]*speed)>>8;
			
			
			#ifdef DEBUG
			print("\nspeed=");print(speed);
			print("\n e1 xspeed=");print(ElementPtr1->xspeed);
			print("\n e1 yspeed=");print(ElementPtr1->yspeed);
			#endif
			

			
		}
	}
}

void ProcessCollision(pObject o1, pObject o2)
{
	pPlayer p=0;
	pWeapon w=0;

	s8 o1type=0,o2type=0;
	

	if (o1->type==PLAYER||o1->type==ASTEROID||o1->type==PLANET)//or asteroid or planet
		o1type=1;
	if (o2->type==PLAYER||o2->type==ASTEROID||o2->type==PLANET)//or asteroid or planet
		o2type=1;



	if (o1type&&o2type)	//both objects that collide  - easy!
	{		
		
		//ShipCollide((pPlayer)o1->parent,(pPlayer)o2->parent);
		collide(o1,o2);

		if (o1->type==PLAYER&&o2->type==PLANET)
		{
			ModifyCrew((pPlayer)o1->parent,-2);
		}

		//blazer damage also here?
		if (o1->type==PLAYER&&o2->type==PLAYER)
		{
			pPlayer s1=(pPlayer)o1->parent;
			pPlayer s2=(pPlayer)o2->parent;

			if (s1->blaze)
				ModifyCrew(s2,-2);
			if (s2->blaze)
				ModifyCrew(s1,-2);
		}
		
		return;
	}
	else if (!(o1type||o2type))//both bullets  - easier
	{
		pWeapon w1=(pWeapon)o1->parent;
		pWeapon w2=(pWeapon)o2->parent;
		
		if (w1->parent!=w2->parent)
		{
		//cancel each other
		o1->life=0;
		o2->life=0;
		}
	}
	else //must be 1 bullet and 1 other 
	{
		
		pWeapon w;
		pObject other;
		pPlayer p;

		if (o1type)
		{
			w=(pWeapon)o2->parent;
			other=o1;
		}
		else
		{
			w=(pWeapon)o1->parent;
			other=o2;
		}

		if (other->type==PLAYER)
		{
			
			p=(pPlayer)other->parent;
			if (w->target==p||w->damageparent)
			{
				
				if (w->hitfunc==0)//simple case
				{	
				
					ModifyCrew(p,w->damage);
								
					if (w->type!=LASER)
						CreateExplosion(w,5);
				//	else
				//		w->object.life=0;
				}
				else	
				w->hitfunc(w,p);
			}
		}
		else
		{
			w->object.life=0;
			if (other->type==ASTEROID)
			{
				other->life=0;
			}
		}
	}	

}



int CheckCollision(pObject o1, pObject o2)
{	
	if (distanceBetweenPoints(o1->xpos,o1->ypos,o2->xpos,o2->ypos)<((o1->size+o2->size)>>1))	
		return 1;
		
	if (distanceBetweenPoints(o1->xpos+((o1->xpos+o1->lastxpos)>>1),
			o1->ypos+((o1->ypos+o1->lastypos)>>1),
			o2->xpos+((o2->xpos+o2->lastxpos)>>1),
			o2->ypos+((o2->ypos+o2->lastypos)>>1))<((o1->size+o2->size)>>1))	
		return 2;
		
	return 0;

}


int ModifyAngle(s16 a,int o)
{
	s16 r = a+o;
	if (r>=360)
		r-=360;
	else if (r<0)
		r+=360;
	return r;
}

void ShipCollide(pPlayer p1,pPlayer p2)  //tmp holding pen for this crap
{

  			//this isnt too great
  			if (distanceBetweenPoints(p1->object.xpos,p1->object.ypos,p2->object.xpos,p2->object.ypos)<(p1->offset+p2->offset)&&p1->crew>0&&p2->crew>0)
  			{
			//	ModifyCrew(p1,-2);
			//	ModifyCrew(p2,-2);
			#ifdef DEBUG
			print("players hit each other");
			#endif
				if (p1->object.xspeed==0&&p1->object.yspeed==0)
				{
					p1->object.xspeed=(-p2->object.xspeed)>>1;
					p1->object.yspeed=(-p2->object.yspeed)>>1;
				}
				if (p2->object.xspeed==0&&p2->object.yspeed==0)
				{
					p2->object.xspeed=(-p1->object.xspeed)>>1;
					p2->object.yspeed=(-p1->object.yspeed)>>1;
				}

				Bounce(p1);
				Bounce(p2);

				if (p1->blaze==1)
					ModifyCrew(p2,-3);

				if (p1->blaze==2)
					ModifyCrew(p1,-3);
/*
				s32 x=p1->object.xspeed;
				s32 y=p1->object.yspeed;

				p1->object.xspeed=(p1->object.xspeed*-p2->object.object.mass_points_points+2)+(p2->object.xspeed*p1->object.object.mass_points_points)>>(p2->object.object.mass_points_points+p1->object.object.mass_points_points);
				p1->object.yspeed=(p1->object.yspeed*-p2->object.object.mass_points_points+2)+(p2->object.yspeed*p1->object.object.mass_points_points)>>(p2->object.object.mass_points_points+p1->object.object.mass_points_points);

				p2->object.xspeed=(p2->object.xspeed*-p1->object.object.mass_points_points+2)+(x*p2->object.object.mass_points_points)>>(p1->object.object.mass_points_points+p1->object.object.mass_points_points);
				p2->object.yspeed=(p2->object.yspeed*-p1->object.object.mass_points_points+2)+(y*p2->object.object.mass_points_points)>>(p1->object.object.mass_points_points+p1->object.object.mass_points_points);

/*

				if (p1->object.xspeed==0&&p1->object.yspeed==0&&(p2->object.xspeed!=0||p2->object.yspeed!=0))
				{
					//p1 stationary /p2 not
					s32 x=p2->object.xspeed;
					s32 y=p2->object.xspeed;

					p1->object.xspeed=x/p2->object.object.mass_points_points;
					p1->object.yspeed=y/p2->object.object.mass_points_points;

					p2->object.xspeed=x/(p1->object.object.mass_points_points*2);
					p2->object.yspeed=y/(p1->object.object.mass_points_points*2);
				}
				else if (p2->object.xspeed==0&&p2->object.yspeed==0&&(p1->object.xspeed!=0||p1->object.yspeed!=0))
				{
					//p2 stationary /p1 not
					s32 x=p1->object.xspeed;
					s32 y=p1->object.xspeed;

					p2->object.xspeed=x/p1->object.object.mass_points_points;
					p2->object.yspeed=y/p1->object.object.mass_points_points;

					p1->object.xspeed=x/(p2->object.object.mass_points_points*2);
					p1->object.yspeed=y/(p2->object.object.mass_points_points*2);
				}
				else if ((p2->object.xspeed!=0||p2->object.yspeed!=0)&&(p1->object.xspeed!=0||p1->object.yspeed!=0)) */
				{

					/*
					print("distance between ships");
					print(distanceBetweenPoints(p1->object.xpos,p1->object.ypos,p2->object.xpos,p2->object.ypos));
					print("\n player 1 object.xpos");
										print(p1->object.xpos);
										print("\n player 1 object.ypos");
										print(p1->object.ypos);
										print("\n player 2 object.xpos");
										print(p2->object.xpos);
										print("\n player 2 object.ypos");
					print(p2->object.ypos);

					print("\n player 1 xpseed");
					print(p1->object.xspeed);
					print("\n player 1 ypseed");
					print(p1->object.yspeed);
					print("\n player 2 xpseed");
					print(p2->object.xspeed);
					print("\n player 2 ypseed");
					print(p2->object.yspeed);


					//both ships moving

					s32 Impac.object.angle1=FindAngle(p1->object.xpos,p1->object.ypos,p2->object.xpos,p2->object.ypos);
					s32 Impac.object.angle2=ModifyAngle(Impac.object.angle1,180);
					s32 Trave.object.angle1=distanceBetweenPoints(0,0,p1->object.xspeed,p1->object.yspeed);
					s32 Trave.object.angle2=distanceBetweenPoints(0,0,p2->object.xspeed,p2->object.yspeed);
					print("Imapact.object.angle 1");
					print(Impac.object.angle1);

					print("Imapact.object.angle 2");
					print(Impac.object.angle2);

					//s32 Directness = ModifyAngle(Impac.object.angle1,
					//	-1*FindAngle(p1->object.xpos+p1->object.xspeed,p1->object.ypos+p1->object.yspeed,p2->object.xpos+p2->object.xspeed,p2->object.ypos+p2->object.yspeed));
					s32 Directness = ModifyAngle(Trave.object.angle1,-Trave.object.angle2);

					print("a2");
					print(FindAngle(p1->object.xpos+p1->object.xspeed,p1->object.ypos+p1->object.yspeed,p2->object.xpos+p2->object.xspeed,p2->object.ypos+p2->object.yspeed));
					print("\ndirectness (ipa1 - a2)");
										print(Directness);

					double scalar = SIN [Directness] * double((p1->object.object.mass_points_points * p2->object.object.mass_points_points));

					s32 speed =  (s32)(scalar / double((p1->object.object.mass_points_points * (p1->object.object.mass_points_points + p2->object.object.mass_points_points))))>>8;



					p1->object.xspeed-= (s32)((speed) * (s32)SIN[Impac.object.angle1])>>8;
					p1->object.yspeed+= (s32)((speed) * (s32)COS[Impac.object.angle1])>>8;

					if (p1->object.xspeed+p1->object.yspeed==0)
					{
						p1->object.xspeed-= (s32)((4) * (s32)SIN[Impac.object.angle1])>>8;
						p1->object.yspeed+= (s32)((4) * (s32)COS[Impac.object.angle1])>>8;
					}



					speed =  (s32)(scalar / double((p2->object.object.mass_points_points * (p2->object.object.mass_points_points + p1->object.object.mass_points_points))))>>8;

					p2->object.xspeed-= (s32)((speed) * (s32)SIN[Impac.object.angle2])>>8;
					p2->object.yspeed+= (s32)((speed) * (s32)COS[Impac.object.angle2])>>8;

					if (p2->object.xspeed+p2->object.yspeed==0)
					{
						p2->object.xspeed-= (s32)((4) * (s32)SIN[Impac.object.angle2])>>8;
						p2->object.yspeed+= (s32)((4) * (s32)COS[Impac.object.angle2])>>8;
					}
*/
/*




					s32 x=p1->object.xspeed;
					s32 y=p1->object.xspeed;

					p1->object.xspeed=((p1->object.xspeed*p1->object.object.mass_points_points)-(p2->object.xspeed*(p2->object.object.mass_points_points+1)))/p1->object.object.mass_points_points;
					p1->object.yspeed=((p1->object.yspeed*p1->object.object.mass_points_points)-(p2->object.yspeed*(p2->object.object.mass_points_points+1)))/p1->object.object.mass_points_points;

					p2->object.xspeed=((p2->object.xspeed*p2->object.object.mass_points_points)-(y*(p1->object.object.mass_points_points+1)))/p2->object.object.mass_points_points;
					p2->object.yspeed=((p2->object.xspeed*p2->object.object.mass_points_points)-(x*(p1->object.object.mass_points_points+1)))/p2->object.object.mass_points_points;
					*/
					/*
					print("after");
									print("\n player 1 xpseed");
														print(p1->object.xspeed);
														print("\n player 1 ypseed");
														print(p1->object.yspeed);
														print("\n player 2 xpseed");
														print(p2->object.xspeed);
														print("\n player 2 ypseed");
					print(p2->object.yspeed);
					*/
				}
				/*
					if (p1->object.xspeed==0&&p2->object.xspeed!=0)
					{
									//p1 stationary /p2 not
									s32 x=p2->object.xspeed;
									p1->object.xspeed=x/p2->object.object.mass_points_points;
				}
									s32 y=p2->object.xspeed;


									p1->object.yspeed=y/p2->object.object.mass_points_points;

									p2->object.xspeed=x/(p1->object.object.mass_points_points*2);
									p2->object.yspeed=x/(p1->object.object.mass_points_points*2);
								}
								else if (p2->object.xspeed==0&&p2->object.yspeed==0&&(p1->object.xspeed!=0||p1->object.yspeed!=0))
								{
									//p2 stationary /p1 not
									s32 x=p1->object.xspeed;
									s32 y=p1->object.xspeed;

									p2->object.xspeed=x/p1->object.object.mass_points_points;
									p2->object.yspeed=y/p1->object.object.mass_points_points;

									p1->object.xspeed=x/(p2->object.object.mass_points_points*2);
									p1->object.xspeed=x/(p2->object.object.mass_points_points*2);
								}
								else if ((p2->object.xspeed!=0||p2->object.yspeed!=0)&&(p1->object.xspeed!=0||p1->object.yspeed!=0))
								{
									//both ships moving
									s32 x=p1->object.xspeed;
									s32 y=p1->object.xspeed;

									p1->object.xspeed=(p1->object.xspeed*p1->object.object.mass_points_points)-(p2->object.xspeed*(p2->object.object.mass_points_points+1));
									p1->object.yspeed=(p1->object.yspeed*p1->object.object.mass_points_points)-(p2->object.yspeed*(p2->object.object.mass_points_points+1));

									p2->object.xspeed=(p2->object.xspeed*p2->object.object.mass_points_points)-(y*(p1->object.object.mass_points_points+1));
									p2->object.yspeed=(p2->object.xspeed*p2->object.object.mass_points_points)-(x*(p1->object.object.mass_points_points+1));
				}
				*/

			}
}







void Melee(pPlayer p1,pPlayer p2,Bg *bg0, Bg *bg1)
{
	u32 loop;       //generic loop variable
	#ifdef DEBUG
	print("Melee started");
	#endif
	screenx=100;
	screeny=100;




	


	trails=(pTrail)malloc(sizeof(Trail)*10);
	for (int i=0;i<10;i++)
	{
		trails->object.life=0;
	}

	asteroids=(pAsteroid)malloc(sizeof(Asteroid)*5);
	{
		for(int i=0;i<5;i++)
		{
			asteroids[i].object.type=ASTEROID;
			asteroids[i].object.parent=&asteroids[i];
			asteroids[i].object.life=-1;
		}
	}

	planet.xpos=screencentrex;
	planet.ypos=screencentrey;


	SetupBackground(bg0,bg1);

	//try - should load in sc2.cpp but oam mem seems to be blank...
	LoadPal();

	//LoadShip(p1);
	//LoadShip(p2);
	p1->loadfunc(p1->SpriteStart);
	p2->loadfunc(p2->SpriteStart);

	LoadExp(FireSprite1*16);
	LoadTrail(TrailSprite*16);
	LoadPlanet(PlanetSprite*16);
	LoadAsteroid(SpriteAsteroidStart*16);

//	p1->crew=1;

	InitializeSprites();                       //set all sprites off screen (stops artifact)

	p1->restorefunc(p1);
	p2->restorefunc(p2);





	setScreen(p1,p2,bg0,bg1);

		//create pl1 + pl2
       	sprites[0].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | p1->object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
        sprites[0].attribute1 =SIZE_32 | ROTDATA(0) | p1->object.xscreen;            
        sprites[0].attribute2 = p1->SpriteStart+p1->spriteoffset | PRIORITY(1);                      //pointer to tile where sprite starts

		sprites[13].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT |p2->object.yscreen;//setup sprite info, 256 colour, shape and y-coord
        sprites[13].attribute1 =SIZE_32 | ROTDATA(13) | p2->object.xscreen;           
        sprites[13].attribute2 = p2->SpriteStart+p2->spriteoffset | PRIORITY(1);     //pointer to tile where sprite starts
/*
        //test loop
        	zoom=100;
        	screenx=120;
			screeny=80;
			p1->object.xpos=120;
			p1->object.ypos=80;
			p1->object.angle=0;
			MoveSprite(&sprites[13], -100, -100);
			while (1)
			{

				if(!(*KEYS & KEY_A))                	//if the A key is pressed
					{
				       zoom--;
				    }
					if(!(*KEYS & KEY_B))                	//if the B key is pressed
					{
						zoom++;
    				}
    				drawOnScreen(&p1->object.xscreen,&p1->object.yscreen,p1->object.xpos,p1->object.ypos,screenx,screeny,32,1);
    					RotateSprite(0, p1->object.angle, zoom, zoom);
					MoveSprite(&sprites[0], p1->object.xscreen, p1->object.yscreen);
				WaitForVsync();					//waits for the screen to stop drawing
				CopyOAM();

			}

*/


/*
// test show explos
// handy for testing sprites
        	sprites[60].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[60].attribute1 =SIZE_32 | 100;
			   	sprites[60].attribute2 = SpriteAsteroidStart | PRIORITY(0);
/*
			   	sprites[61].attribute0 = COLOR_256 | SQUARE | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[61].attribute1 =SIZE_8 | 20;
			   	sprites[61].attribute2 = FireSprite1+2 | PRIORITY(0);

			   	sprites[62].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[62].attribute1 =SIZE_16 | 40;
			   	sprites[62].attribute2 = FireSprite1+10 | PRIORITY(0);

			   	sprites[63].attribute0 = COLOR_256 | SQUARE   | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[63].attribute1 =SIZE_16 | 60;
			   	sprites[63].attribute2 = FireSprite1+18 | PRIORITY(0);

			   	sprites[64].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[64].attribute1 =SIZE_16 | 80;
			   	sprites[64].attribute2 = FireSprite1+26 | PRIORITY(0);
*/


//Pilots
//42 - p1 pilot
//43-47 p1 (left/right)/thrust/fire/special
//48 - p2 pilo5
//49-54 = p2
if (pilot)
{
sprites[42].attribute0 = COLOR_256 | TALL  | 0;
sprites[42].attribute1 =SIZE_64 | 509;
sprites[42].attribute2 = p1->SpriteStart+p1->pilot_sprite | PRIORITY(3);
p1->loadpilots(p1);

sprites[48].attribute0 = COLOR_256 | TALL  | 0;
sprites[48].attribute1 =SIZE_64 | 179;
sprites[48].attribute2 = p2->SpriteStart+p2->pilot_sprite | PRIORITY(3);
p2->loadpilots(p2);
}
//load pause

sprites[55].attribute0 = COLOR_256 | TALL  | 240;
sprites[55].attribute1 =SIZE_32 | 160;
sprites[55].attribute2 = PauseSpriteStart | PRIORITY(0);
sprites[56].attribute0 = COLOR_256 | TALL  | 240;
sprites[56].attribute1 =SIZE_32 | 160;
sprites[56].attribute2 = PauseSpriteStart+16 | PRIORITY(0);
sprites[57].attribute0 = COLOR_256 | TALL  | 240;
sprites[57].attribute1 =SIZE_32 | 160;
sprites[57].attribute2 = PauseSpriteStart+32 | PRIORITY(0);
sprites[58].attribute0 = COLOR_256 | TALL  | 240;
sprites[58].attribute1 =SIZE_32 | 160;
sprites[58].attribute2 = PauseSpriteStart+48 | PRIORITY(0);

		SetupStatus(p1,p2);
		//setScreen(p1,p2);
		turn=0;
		WaitForVsync();
		//s8 EndGame1=20;
		//s8 EndGame2=20;
		//UpdateStatus();
		
		p1->object.type=PLAYER;
		p1->object.parent=&p1;
		p2->object.type=PLAYER;
		p2->object.parent=&p2;
		planet.type=PLANET;
		planet.parent=&planet;
		planet.state_flags=DEFY_PHYSICS;
		planet.xspeed=0;
		planet.yspeed=0;
		planet.size=64;
		planet.ignorecollision=0;
		
		Object *GameObjs[32];//list of all objects in the game
		
		
		//setup some pointers
		
		//create lists of ai oppents;
		
		//0 - opp
		//1 - planet
		//2 - 13 - opp weapons
		//14-18 asteroids
		
		p1->objlist[0] = &p2->object;
		p1->objlist[1] = &planet;
		p2->objlist[0] = &p1->object;
		p2->objlist[1] = &planet;
		
		
		
		GameObjs[0]=&p1->object;
		GameObjs[1]=&p2->object;
		GameObjs[31]=&planet;
		
		//hacky
		p1->object.size=p1->offset*2;
		p2->object.size=p2->offset*2;		
		p1->object.life=100;
		p2->object.life=100;
		p1->object.parent=p1;
		p2->object.parent=p2;
		
		for (int i=0;i<12;i++)
		{
			p1->objlist[i+2] = &(p2->weapon[i].object);
			p2->objlist[i+2] = &(p1->weapon[i].object);
			p1->weapon[i].object.type=WEAPON;
			p2->weapon[i].object.type=WEAPON;
			p1->weapon[i].object.ignorecollision=0;	
			p2->weapon[i].object.ignorecollision=0;	
			p1->weapon[i].object.parent=&(p1->weapon[i]);
			p2->weapon[i].object.parent=&(p2->weapon[i]);
			GameObjs[i+2]=&(p1->weapon[i].object);
			GameObjs[i+14]=&(p2->weapon[i].object);
			
		}
	
		for (int i=0;i<5;i++)
		{
			asteroids[i].object.type=ASTEROID;	
			asteroids[i].object.ignorecollision=0;
			asteroids[i].object.mass_points=3;
			asteroids[i].object.size=20;
			p1->objlist[i+14]=&asteroids[i].object;
			p2->objlist[i+14]=&asteroids[i].object;
			GameObjs[i+26]=&asteroids[i].object;
		}
			
		
		while(p1->EndGame&&p2->EndGame)                                //melee loop
        {
        
			//do all collision detection here			
			for (int s=0;s<32;s++)
			{	
				if (GameObjs[s]->life>0&&GameObjs[s]->ignorecollision!=1)
				for (int t=s+1;t<32;t++)
				{
					if (GameObjs[t]->life>0&&GameObjs[t]->ignorecollision!=1)
						if (CheckCollision(GameObjs[s],GameObjs[t]))
							ProcessCollision(GameObjs[s],GameObjs[t]);
				}
				CalculateGravity(&planet,GameObjs[s]);
			}
			
			setScreen(p1,p2,bg0,bg1);
			
			//get keys or ai
			ProcessPlayer(p1);
			ProcessPlayer(p2);
			
			//bullets
			for (int s=0;s<12;s++)
			{
				MoveBullet(&p1->weapon[s]);
				MoveBullet(&p2->weapon[s]);
			}

  			//asteroids
  			ProcessAsteroids(p1,p2);
  			//planet
  			DrawPlanet();
  		//	CalcPlanet(p1);
  		//	CalcPlanet(p2);			

			MoveSprite(&sprites[0], p1->object.xscreen, p1->object.yscreen);

			MoveSprite(&sprites[13], p2->object.xscreen, p2->object.yscreen);




		
		//	DetectBullets(p1);
		//	DetectBullets(p2);

			DrawTrails();


			if (p1->crew>0)
				Regen(p1);
			if (p2->crew>0)
				Regen(p2);

			RotateSprite(0, p1->object.angle, zoom, zoom);
			RotateSprite(13, p2->object.angle, zoom,zoom);

			if (p1->postfunc!=0)
				p1->postfunc(p1);
			if (p2->postfunc!=0)
				p2->postfunc(p2);

			
			UpdateBackground(bg0);
			UpdateBackground(bg1);
			WaitForVsync();					//waits for the screen to stop drawing
			CopyOAM();			//Copies sprite array into OAM.

			if (demo)
			{
				if((!(*KEYS & KEY_START)) || (!(*KEYS & KEY_SELECT)) ||
					(!(*KEYS & KEY_A)) || (!(*KEYS & KEY_B)) ||
					(!(*KEYS & KEY_L)) || (!(*KEYS & KEY_R)))
				return;
			}
			
			for (int j=0;j<32;j++)//dont bother moving planet
			{
				GameObjs[j]->lastxpos=GameObjs[j]->xpos;
				GameObjs[j]->lastypos=GameObjs[j]->ypos;
				
				GameObjs[j]->xpos=GameObjs[j]->xpos+=GameObjs[j]->xspeed;
				GameObjs[j]->ypos=GameObjs[j]->ypos-=GameObjs[j]->yspeed;
			}
			planet.life=2000;//bad but stops it dying




		}//end while one or both ships are destroyed

	//should clear screen
	InitializeSprites();
	WaitForVsync();
	CopyOAM();
}





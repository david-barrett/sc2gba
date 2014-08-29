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


pTrail trails;
pAsteroid asteroids;

int turn;

s16 planet_screenx,planet_screeny;
extern s32 planetx,planety;



//extern FIXED SIN2[360];	    //Look-Up Tabless for sign and cosign
//extern FIXED COS2[360];





double scale;

s32 zoom,screenx,screeny;

//centre of screen
//note ships draw from TOP_LEFT

s8 yswap=0;
s8 xswap=0;

#define ASIZE  512 //128 // 512 ?


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
	s16 life=w->life;

	s16 off=0;

	if (life<3)
	{
		if (life==2)
			off=2;
		w->size=8;
		sprites[w->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | w->yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[w->sprite].attribute1 = SIZE_8 | ROTDATA(w->sprite) | w->xscreen;
    	sprites[w->sprite].attribute2 = FireSprite1+off | PRIORITY(0);
	}
	else if (life<=5)
	{
		off=4;
		if (life==4)
			off=12;
		else if (life==5)
			off=20;
		w->size=16;
		sprites[w->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | w->yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[w->sprite].attribute1 = SIZE_16 | ROTDATA(w->sprite) | w->xscreen;
	    sprites[w->sprite].attribute2 = FireSprite1+off | PRIORITY(0);
	}

}

void CreateExplosion(pWeapon w,s16 life)
{
	w->life=life;;
	w->xspeed=0;
	w->yspeed=0;
	w->type=EXP;
	w->angle=0;
	if (life<3)
		w->size=8;
	else
		w->size=16;


	drawOnScreen(&(w->xscreen),&(w->yscreen),
			w->xpos,w->ypos,screenx,screeny,w->size);
	MoveSprite(&sprites[w->sprite], w->xscreen, w->yscreen);

	DrawExplosion(w);
}



int DetectWeaponToShip(pPlayer p,pWeapon w)
{
	//square (w.xpos-4,w.ypos-4), (w.xpos+4,w.ypos+4)
	//square (p.xpos-16,w.ypos-16), (w.xpos+16,w.ypos+16)

	s16 offset=p->offset;///2;

	//tmp
//	offset+=(w->size>>2);

	if (w->xpos>p->xpos-offset&&w->xpos<p->xpos+offset&&w->ypos>p->ypos-offset&&w->ypos<p->ypos+offset)
	{
		return 1;
	}
	return 0;

}

int DetectWeaponToWeapon(pWeapon tar,pWeapon w)
{
	//square (w.xpos-4,w.ypos-4), (w.xpos+4,w.ypos+4)
	//square (p.xpos-16,w.ypos-16), (w.xpos+16,w.ypos+16)

	s16 offset=tar->size>>1;


	if (w->xpos>tar->xpos-offset&&w->xpos<tar->xpos+offset&&w->ypos>tar->ypos-offset&&w->ypos<tar->ypos+offset)
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
		angle=(angle*-1);

	//normalise to nearest 15.
	//big assumption:tan only returns 0-90?
	//0 =0-7
	//15 = 8-22
	//30 = 23-37
	//45 = 38-52
	//60 = 53-67
	//75 = 68-82
	//90 = 83-90

	if (angle<=7)
		angle=0;
	else if (angle>=8&&angle<=22)
		angle=15;
	else if (angle>=23&&angle<=37)
		angle=15;
	else if (angle>=38&&angle<=52)
		angle=45;
	else if (angle>=53&&angle<=67)
		angle=15;
	else if (angle>=68&&angle<=82)
		angle=75;
	else if (angle<=90)
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
	if (xpos>txpos-range&&xpos<txpos+range&&ypos>typos-range&&ypos<typos+range)
	{
		return 1;
	}
	return 0;
	*/
}



void MoveBullets(pPlayer pl)
{
	pPlayer p;
	pPlayer o;

	for (int i=0;i<12;i++)
	{
		if (pl->weapon[i].life==0)
		{
			sprites[pl->weapon[i].sprite].attribute0 = 160;  //y to > 159
			sprites[pl->weapon[i].sprite].attribute1 = 240; //x to > 239
			sprites[pl->weapon[i].sprite].attribute2 = 0;
			pl->weapon[i].life--;
		}

		if (pl->weapon[i].life>0)
		{

			if (pl->weapon[i].type==SIMPLE||pl->weapon[i].type==ILWRATHFIRE
				||pl->weapon[i].type==LASER)
			{
				if (pl->weapon[i].type==ILWRATHFIRE)
					DrawExplosion(&pl->weapon[i]);
				pPlayer target=(pPlayer)pl->weapon[i].target;
				pl->weapon[i].xpos+=pl->weapon[i].xspeed;
				pl->weapon[i].ypos-=pl->weapon[i].yspeed;


				pl->weapon[i].life--;


			}//simple
			else if (pl->weapon[i].type==UR_FIGHTERS)
			{
				MoveURFighters(&pl->weapon[i]);
			}
			else if (pl->weapon[i].type==UR_FIGHTERS_FIRE)
			{
				pl->weapon[i].life--;
			}
			else if(pl->weapon[i].type==EXP)
			{
				pl->weapon[i].life--;
				DrawExplosion(&pl->weapon[i]);
			}
			else if(pl->weapon[i].type==TRAIL)
			{
					pl->weapon[i].life--;
			}
			else if(pl->weapon[i].type==LIMPET)
			{
				MoveLimpet(&pl->weapon[i]);
			}
			else if(pl->weapon[i].type==CREW)
			{
				MoveCrew(&pl->weapon[i]);
			}
			else if(pl->weapon[i].type==BUTT)
			{
				MoveButt(&pl->weapon[i]);
			}
			else
			{
				(pl->weapon[i].movefunc)(&pl->weapon[i]);
			}
			drawOnScreen(&(pl->weapon[i].xscreen),&(pl->weapon[i].yscreen),
				pl->weapon[i].xpos,pl->weapon[i].ypos,screenx,screeny,pl->weapon[i].size);
			MoveSprite(&sprites[pl->weapon[i].sprite], pl->weapon[i].xscreen, pl->weapon[i].yscreen);
			RotateSprite(pl->weapon[i].sprite, pl->weapon[i].angle, zoom,zoom);
		}//end if life>0
	}//end for loop

}
/*
s8
Intercept (pObject ElementPtr0, pObject ElementPtr1, COUNT
		max_turns, COUNT margin_of_error)
{
	max_turns;
	s32 x1 = ElementPtr0->xpos;//+ElementPtr0->xspeed;
	s32 y1 = ElementPtr0->ypos;//+ElementPtr0->yspeed;
	s32 x2 = ElementPtr1->xpos;//+ElementPtr1->xspeed;
	s32 y2 = ElementPtr1->ypos;//+ElementPtr1->yspeed;

	s32 hx1 = ElementPtr0->xspeed<<1;
	s32 hy1 = ElementPtr0->yspeed<<1;
	s32 hx2 = ElementPtr1->xspeed<<1;
	s32 hy2 = ElementPtr1->yspeed<<1;

	s16 size = ((ElementPtr0->size+ElementPtr1->size)+margin_of_error);
//	for (s16 i=0;i<max_turns;i++)
//	{
		if ((distanceBetweenPoints(x1,y1,x2,y2)<size))
				//||(distanceBetweenPoints(x1+hx1,y1-hy1,x2-hx2,y2-hy2)<size))
			return 1;
//		x1+=ElementPtr0->xspeed;
//		y1+=ElementPtr0->yspeed;
//		x2+=ElementPtr1->xspeed;
//		y1+=ElementPtr1->yspeed;
//	}
	return 0;
}

*/
void DetectBullets(pPlayer pl)
{
	pPlayer target;
	int stop;
	s32 halfx,halfy;
	s16 angle;
	s32 tmp;
//	Object o1,o2;
	for (int i=0;i<12;i++)
	{

		stop=1;  //stop processing when destroyedto save time
		if (pl->weapon[i].life>=0)
		{
			/*
			if (pl->weapon[i].type==LASER)
			{
				angle=ModifyAngle(pl->weapon[i].angle,180);
				halfx=((pl->weapon[i].size * (s32)SIN[pl->angle])>>9);
				halfy=((pl->weapon[i].size * (s32)COS[pl->angle])>>9);
			}
			else
			{
				halfx=pl->weapon[i].xspeed>>1;
				halfy=pl->weapon[i].yspeed>>1;
			}
			*/



			if (pl->weapon[i].type==SIMPLE||pl->weapon[i].type==ILWRATHFIRE
				||pl->weapon[i].type==LASER)
			{
				target=(pPlayer)pl->weapon[i].target;
/*
				if (pl->weapon[i].type!=LASER)
				{
					o1.xpos=pl->weapon[i].xpos-pl->weapon[i].xspeed;
					o1.ypos=pl->weapon[i].ypos-pl->weapon[i].yspeed ;
					o1.xspeed=pl->weapon[i].xspeed;
					o1.yspeed=pl->weapon[i].yspeed;
					o1.size=2;//(pl->weapon[i].size<<2);
				}
				else
				{
					tmp=(pl->weapon[i].size * (s32)SIN[pl->weapon[i].angle])>>9;
					o1.xpos=pl->weapon[i].xpos;//-(tmp<<1);
					o1.xspeed=tmp;

					tmp=(pl->weapon[i].size * (s32)COS[pl->weapon[i].angle])>>9;
					o1.ypos=pl->weapon[i].ypos;//-(tmp<<1) ;
					o1.yspeed=tmp;
					o1.size=2;
				}

				//opp ship
				o2.xpos=target->xpos;//-target->xspeed;
				o2.ypos=target->ypos;//-target->yspeed ;
				o2.xspeed=target->xspeed;
				o2.yspeed=target->yspeed;
				o2.size=target->offset;
*/
				if (DetectWeaponToShip(target,&pl->weapon[i]))
		//		if (Intercept (&o1 , &o2,1,0))
				{
					ModifyCrew(target,pl->weapon[i].damage);
					if (pl->weapon[i].type!=LASER)
						CreateExplosion(&pl->weapon[i],5);/*
					else
					{
						s8 d=nextWeapon(pl);
						if (d>0)
							CreateExplosion(&pl->weapon[d],5);
					}*/
					stop=0;
				}


				//parent ship
				if (pl->weapon[i].damageparent==1&&stop)
				{
					/*
					o2.xpos=pl->xpos;//-pl->xspeed;
					o2.ypos=pl->ypos;//-pl->yspeed ;
					o2.xspeed=pl->xspeed;
					o2.yspeed=pl->yspeed;
					o2.size=pl->offset;
					*/
					if (DetectWeaponToShip((pPlayer)pl->weapon[i].parent,&pl->weapon[i]))
				//	if (Intercept (&o1 , &o2,1,0))
					{
						ModifyCrew((pPlayer)pl->weapon[i].parent,pl->weapon[i].damage);
						if (pl->weapon[i].type!=LASER)
							CreateExplosion(&pl->weapon[i],5);
							/*
						else
						{
							s8 d=nextWeapon(pl);
							if (d>0)
								CreateExplosion(&pl->weapon[d],5);
						}*/
						stop=0;
					}
				}



				//opp weapon

				if (stop)
				{
				for (int j=0;j<12;j++)
				{
					if (target->weapon[j].life>0)
					if (DetectWeaponToWeapon(&target->weapon[j],&pl->weapon[i])==1)
					{
						if (pl->weapon[i].damageparent==0&&target->weapon[j].damageparent==0)
						//ie so two fighters dont kill each other
						{
						pl->weapon[i].life=0;
						if (target->weapon[j].type!=LASER)
						{
							sprites[target->weapon[j].sprite].attribute0 = 160;  //y to > 159
							sprites[target->weapon[j].sprite].attribute1 = 240; //x to > 239
							sprites[target->weapon[j].sprite].attribute2 = 0;
							target->weapon[j].life=-1;
						}
						stop=0;
						break;
						}
					}
				}
				}
		}
		if (pl->weapon[i].type!=EXP)
		{
				s16 dist;
				//planet
				if (stop)
				{
					dist = distanceBetweenPoints(pl->weapon[i].xpos,pl->weapon[i].ypos,planetx,planety);
					if (dist<(pl->weapon[i].size+64)/2)
					{
							pl->weapon[i].life=0;
							stop=0;
					}
				}

				//asteroids
				if (stop)
				{
					for (int j=0;j<5;j++)
					{
						dist = distanceBetweenPoints(pl->weapon[i].xpos,pl->weapon[i].ypos,
							asteroids[j].xpos,asteroids[j].ypos);

						if (dist<(pl->weapon[i].size+25)/2)
						{
							pl->weapon[i].life=0;
							asteroids[j].life=0;
							stop=0;
							break;
						}
					}
				}
				if (stop==0&&pl->weapon[i].type!=EXP&&pl->weapon[i].type!=LASER)
				{
					pl->weapon[i].life=0;
					sprites[pl->weapon[i].sprite].attribute0 = 160;  //y to > 159
					sprites[pl->weapon[i].sprite].attribute1 = 240; //x to > 239
					sprites[pl->weapon[i].sprite].attribute2 = 0;
					pl->weapon[i].life--;
				}
			}
		}
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

	z = (s32)sqrt((x*x)+(y*y));
	return z;
}

inline void findCentre(s32 *x,s32 *y,pPlayer p1,pPlayer p2)
{
	*x = ((p1->xpos+p2->xpos)/2);//-32;
	*y = ((p1->ypos+p2->ypos)/2);//-32;
}

void shifteverythingx(pPlayer p1,pPlayer p2,int shift)
{
	//shift everything except the planet

	p1->xpos+=shift;
	p2->xpos+=shift;
	screenx+=shift;
	int i;
	for (i=0;i<12;i++)
	{
		p1->weapon[i].xpos+=shift;
		p2->weapon[i].xpos+=shift;
	}
	for (i=0;i<10;i++)
	{
		trails[i].xpos+=shift;
	}

	//asteroids
	for (i=0;i<5;i++)
	{
		asteroids[i].xpos+=shift;
		/*
		if (asteroids[i].xpos<screenx-(960+64))
						{
							asteroids[i].xpos+=(1919+64);

						}
						else if (asteroids[i].xpos>screenx+(960+64))
						{
							asteroids[i].xpos-=(1919+64);

						}
		*/
	}

	//planet
	planetx+=shift;
}

void shifteverythingy(pPlayer p1,pPlayer p2,int shift)
{
	//shift everything except the planet

	p1->ypos+=shift;
	p2->ypos+=shift;
	screeny+=shift;
	int i;
	for (i=0;i<12;i++)
	{
		p1->weapon[i].ypos+=shift;
		p2->weapon[i].ypos+=shift;
	}
	for (i=0;i<10;i++)
	{
		trails[i].ypos+=shift;
	}
	//asteroids
	for (i=0;i<5;i++)
	{
		asteroids[i].ypos+=shift;
		/*
				if (asteroids[i].ypos<screencentrey-(640+64))
				{

					asteroids[i].ypos+=(1279+64);
				}
				else if (asteroids[i].ypos>screencentrex+(640+64))
				{

					asteroids[i].ypos-=(1279+64);

		}*/
	}

	//planet
	planety+=shift;


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

	s32 d= distanceBetweenPoints(p1->xpos,p1->ypos,p2->xpos,p2->ypos);
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
	/*
	else if (scaled&&d<320)
	{
		zoom=256+(s16)(double(d-160)*32);
		scale=0.9-(double(d-160)*0.04);
	}
	*/
	//scaled zoom 320-160
	else if (scaled&&d<170)
	{
			zoom=256+32;//512
			scale = 0.86;
	}
	else if (scaled&&d<180)
	{
			zoom=256+64;//512
			scale = 0.82;
	}
	else if (scaled&&d<190)
	{
		zoom=256+64+32;//512
		scale = 0.78;
	}
	else if (scaled&&d<200)
	{
			zoom=256+128;
			scale = 0.74;

	}
	else if (scaled&&d<210)
	{
		zoom=256+128+32;
		scale = 0.70;

	}
	else if (scaled&&d<220)
	{
			zoom=256+192;
			scale = 0.66;

	}
	else if (scaled&&d<230)
	{
		zoom=256+192+32;
		scale = 0.62;

	}
	else if (scaled&&d<240)
	{
			zoom=1<<9;//512
			scale = 0.58;

	}
	else if (scaled&&d<250)
	{
		zoom=512+32;
		scale = 0.54;

	}
	else if (scaled&&d<260)
	{
		zoom=512+64;
		scale = 0.50;
	}
	else if (scaled&&d<270)
	{
		zoom=512+64+32;
		scale = 0.46;
	}
	else if (scaled&&d<280)
	{
		zoom=512+128;
		scale = 0.42;

	}
	else if (scaled&&d<290)
	{
		zoom=512+128+32;
		scale = 0.38;

	}
	else if (scaled&&d<300)
	{
		zoom=512+192;
		scale = 0.34;

	}
	else if (scaled&&d<310)
	{
		zoom=512+192+32;
		scale = 0.30;
	}
	else if (scaled&&d<320)
	{
			zoom=512+256;
			scale = 0.26;

	}

	//END 3DO zoom could replace all this will scaled zoom
	else //if (d>=320)
	{
		zoom=768;
		scale = 0.25;
		//scale=4;

		//if reached the end of space swap ships
/*
		s32 xd = p1->xpos-p2->xpos;
		if (xd<-960 || xd>960)
		{
			if (screenx>1000)
				shifteverythingx(p1,p2,-500);
			else
				shifteverythingx(p1,p2,500);
			swap(&p1->xpos,&p2->xpos);
		}
		s32 yd = p1->ypos-p2->ypos;
		if (yd<-640 || yd>640)
		{
			swap(&p1->ypos,&p2->ypos);
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


		s32 xd = p1->xpos-p2->xpos;
		s32 yd = p1->ypos-p2->ypos;
		if (xswap==0&&(xd<-960||xd>960))
		{
			xswap=5;
			swap(&p1->xpos,&p2->xpos);
			#ifdef DEBUG
			print("\nscreen x=");
			print(screenx);
			#endif

			if (screenx<screencentrex)
			{
				p1->xpos+=960;
				p2->xpos+=960;
				screenx+=960;
			//	shifteverythingx(p1,p2,959);
			}
			else
			{
				screenx-=960;
				p1->xpos-=960;
				p2->xpos-=960;
			//	shifteverythingx(p1,p2,-959);
			}


		}

		if (yswap==0&&(yd<-640 || yd>640))
		{
			yswap=5;
			swap(&p1->ypos,&p2->ypos);
			#ifdef DEBUG
			print("\nbefore flipabout to add/minus 500 screen y=");
			print(screeny);
			#endif

			if (screeny<screencentrey)
			{

				p1->ypos+=640;
				p2->ypos+=640;
				screeny+=640;
		//		shifteverythingy(p1,p2,639);
			}
			else
			{

				screeny-=640;
				p1->ypos-=640;
				p2->ypos-=640;
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




	drawOnScreen(&p1->xscreen,&p1->yscreen,p1->xpos,p1->ypos,screenx,screeny,32,1);
	drawOnScreen(&p2->xscreen,&p2->yscreen,p2->xpos,p2->ypos,screenx,screeny,32,1);
}

void drawOnScreen(s16* x,s16* y,s32 xpos,s32 ypos, s16 screenx, s16 yscreen,s16 size,s16 player)
{
//	*x = (s16)(((xpos-screenx)*scale)+centrex-(size/scale));
//	*y = (s16)(((ypos-screeny)*scale)+centrey-(size/scale));


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
	s16 angle = FindAngle(pl->xpos,pl->ypos,opp->xpos,opp->ypos);

	s16 a1 = 360+angle;
	s16 a2 = 360+pl->angle;

	s16 x=30;
	if (pl->ship==FURY)
		x=100;
	if (a1>a2-x&&a1<a2+x)//are they roughly at right angle?
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

	//if (yourangle>desiredangle)//else
	else
	{
		if (diff>180)
			return -1;
		else
			return 1;
	}

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








void Melee(pPlayer p1,pPlayer p2,Bg *bg0, Bg *bg1)
{
	u32 loop;       //generic loop variable
	#ifdef DEBUG
	print("Melee started");
	#endif
	screenx=100;
	screeny=100;




	//holds planet vars
	//may make these global


	trails=(pTrail)malloc(sizeof(Trail)*10);
	for (int i=0;i<10;i++)
	{
		trails->life=0;
	}

	asteroids=(pAsteroid)malloc(sizeof(Asteroid)*5);
	{
		for(int i=0;i<5;i++)
		{
			asteroids->life=-1;
		}
	}

	planetx=screencentrex;
	planety=screencentrey;


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
       	sprites[0].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | p1->yscreen;	//setup sprite info, 256 colour, shape and y-coord
        sprites[0].attribute1 = SIZE_32 | ROTDATA(0) | p1->xscreen;            //size 32x32 and x-coord
        sprites[0].attribute2 = p1->SpriteStart+p1->spriteoffset | PRIORITY(1);                      //pointer to tile where sprite starts

		sprites[13].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT |p2->yscreen;//setup sprite info, 256 colour, shape and y-coord
        sprites[13].attribute1 = SIZE_32 | ROTDATA(13) | p2->xscreen;           //size 16x16 and x-coord
        sprites[13].attribute2 = p2->SpriteStart+p2->spriteoffset | PRIORITY(1);     //pointer to tile where sprite starts
/*
        //test loop
        	zoom=100;
        	screenx=120;
			screeny=80;
			p1->xpos=120;
			p1->ypos=80;
			p1->angle=0;
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
    				drawOnScreen(&p1->xscreen,&p1->yscreen,p1->xpos,p1->ypos,screenx,screeny,32,1);
    					RotateSprite(0, p1->angle, zoom, zoom);
					MoveSprite(&sprites[0], p1->xscreen, p1->yscreen);
				WaitForVsync();					//waits for the screen to stop drawing
				CopyOAM();

			}

*/


/*
// test show explos
// handy for testing sprites
        	sprites[60].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[60].attribute1 = SIZE_32 | 100;
			   	sprites[60].attribute2 = SpriteAsteroidStart | PRIORITY(0);
/*
			   	sprites[61].attribute0 = COLOR_256 | SQUARE | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[61].attribute1 = SIZE_8 | 20;
			   	sprites[61].attribute2 = FireSprite1+2 | PRIORITY(0);

			   	sprites[62].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[62].attribute1 = SIZE_16 | 40;
			   	sprites[62].attribute2 = FireSprite1+10 | PRIORITY(0);

			   	sprites[63].attribute0 = COLOR_256 | SQUARE   | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[63].attribute1 = SIZE_16 | 60;
			   	sprites[63].attribute2 = FireSprite1+18 | PRIORITY(0);

			   	sprites[64].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[64].attribute1 = SIZE_16 | 80;
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
sprites[42].attribute1 = SIZE_64 | 509;
sprites[42].attribute2 = p1->SpriteStart+p1->pilot_sprite | PRIORITY(3);
p1->loadpilots(p1);

sprites[48].attribute0 = COLOR_256 | TALL  | 0;
sprites[48].attribute1 = SIZE_64 | 179;
sprites[48].attribute2 = p2->SpriteStart+p2->pilot_sprite | PRIORITY(3);
p2->loadpilots(p2);
}
//load pause

sprites[55].attribute0 = COLOR_256 | TALL  | 240;
sprites[55].attribute1 = SIZE_32 | 160;
sprites[55].attribute2 = PauseSpriteStart | PRIORITY(0);
sprites[56].attribute0 = COLOR_256 | TALL  | 240;
sprites[56].attribute1 = SIZE_32 | 160;
sprites[56].attribute2 = PauseSpriteStart+16 | PRIORITY(0);
sprites[57].attribute0 = COLOR_256 | TALL  | 240;
sprites[57].attribute1 = SIZE_32 | 160;
sprites[57].attribute2 = PauseSpriteStart+32 | PRIORITY(0);
sprites[58].attribute0 = COLOR_256 | TALL  | 240;
sprites[58].attribute1 = SIZE_32 | 160;
sprites[58].attribute2 = PauseSpriteStart+48 | PRIORITY(0);

		SetupStatus(p1,p2);
		//setScreen(p1,p2);
		turn=0;
		WaitForVsync();
		//s8 EndGame1=20;
		//s8 EndGame2=20;
		//UpdateStatus();

		while(p1->EndGame&&p2->EndGame)                                //main loop
        {

			ProcessPlayer(p1);
			ProcessPlayer(p2);



  			//should also check for collisions with
  			//other player
  			//this isnt too great
  			if (distanceBetweenPoints(p1->xpos,p1->ypos,p2->xpos,p2->ypos)<(p1->offset+p2->offset)&&p1->crew>0&&p2->crew>0)
  			{
			//	ModifyCrew(p1,-2);
			//	ModifyCrew(p2,-2);
			#ifdef DEBUG
			print("players hit each other");
			#endif
				if (p1->xspeed==0&&p1->yspeed==0)
				{
					p1->xspeed=(-p2->xspeed)>>1;
					p1->yspeed=(-p2->yspeed)>>1;
				}
				if (p2->xspeed==0&&p2->yspeed==0)
				{
					p2->xspeed=(-p1->xspeed)>>1;
					p2->yspeed=(-p1->yspeed)>>1;
				}

				Bounce(p1);
				Bounce(p2);

				if (p1->blaze==1)
					ModifyCrew(p2,-3);

				if (p1->blaze==2)
					ModifyCrew(p1,-3);
/*
				s32 x=p1->xspeed;
				s32 y=p1->yspeed;

				p1->xspeed=(p1->xspeed*-p2->mass+2)+(p2->xspeed*p1->mass)>>(p2->mass+p1->mass);
				p1->yspeed=(p1->yspeed*-p2->mass+2)+(p2->yspeed*p1->mass)>>(p2->mass+p1->mass);

				p2->xspeed=(p2->xspeed*-p1->mass+2)+(x*p2->mass)>>(p1->mass+p1->mass);
				p2->yspeed=(p2->yspeed*-p1->mass+2)+(y*p2->mass)>>(p1->mass+p1->mass);

/*

				if (p1->xspeed==0&&p1->yspeed==0&&(p2->xspeed!=0||p2->yspeed!=0))
				{
					//p1 stationary /p2 not
					s32 x=p2->xspeed;
					s32 y=p2->xspeed;

					p1->xspeed=x/p2->mass;
					p1->yspeed=y/p2->mass;

					p2->xspeed=x/(p1->mass*2);
					p2->yspeed=y/(p1->mass*2);
				}
				else if (p2->xspeed==0&&p2->yspeed==0&&(p1->xspeed!=0||p1->yspeed!=0))
				{
					//p2 stationary /p1 not
					s32 x=p1->xspeed;
					s32 y=p1->xspeed;

					p2->xspeed=x/p1->mass;
					p2->yspeed=y/p1->mass;

					p1->xspeed=x/(p2->mass*2);
					p1->yspeed=y/(p2->mass*2);
				}
				else if ((p2->xspeed!=0||p2->yspeed!=0)&&(p1->xspeed!=0||p1->yspeed!=0)) */
				{

					/*
					print("distance between ships");
					print(distanceBetweenPoints(p1->xpos,p1->ypos,p2->xpos,p2->ypos));
					print("\n player 1 xpos");
										print(p1->xpos);
										print("\n player 1 ypos");
										print(p1->ypos);
										print("\n player 2 xpos");
										print(p2->xpos);
										print("\n player 2 ypos");
					print(p2->ypos);

					print("\n player 1 xpseed");
					print(p1->xspeed);
					print("\n player 1 ypseed");
					print(p1->yspeed);
					print("\n player 2 xpseed");
					print(p2->xspeed);
					print("\n player 2 ypseed");
					print(p2->yspeed);


					//both ships moving

					s32 ImpactAngle1=FindAngle(p1->xpos,p1->ypos,p2->xpos,p2->ypos);
					s32 ImpactAngle2=ModifyAngle(ImpactAngle1,180);
					s32 TravelAngle1=distanceBetweenPoints(0,0,p1->xspeed,p1->yspeed);
					s32 TravelAngle2=distanceBetweenPoints(0,0,p2->xspeed,p2->yspeed);
					print("Imapact angle 1");
					print(ImpactAngle1);

					print("Imapact angle 2");
					print(ImpactAngle2);

					//s32 Directness = ModifyAngle(ImpactAngle1,
					//	-1*FindAngle(p1->xpos+p1->xspeed,p1->ypos+p1->yspeed,p2->xpos+p2->xspeed,p2->ypos+p2->yspeed));
					s32 Directness = ModifyAngle(TravelAngle1,-TravelAngle2);

					print("a2");
					print(FindAngle(p1->xpos+p1->xspeed,p1->ypos+p1->yspeed,p2->xpos+p2->xspeed,p2->ypos+p2->yspeed));
					print("\ndirectness (ipa1 - a2)");
										print(Directness);

					double scalar = SIN [Directness] * double((p1->mass * p2->mass));

					s32 speed =  (s32)(scalar / double((p1->mass * (p1->mass + p2->mass))))>>8;



					p1->xspeed-= (s32)((speed) * (s32)SIN[ImpactAngle1])>>8;
					p1->yspeed+= (s32)((speed) * (s32)COS[ImpactAngle1])>>8;

					if (p1->xspeed+p1->yspeed==0)
					{
						p1->xspeed-= (s32)((4) * (s32)SIN[ImpactAngle1])>>8;
						p1->yspeed+= (s32)((4) * (s32)COS[ImpactAngle1])>>8;
					}



					speed =  (s32)(scalar / double((p2->mass * (p2->mass + p1->mass))))>>8;

					p2->xspeed-= (s32)((speed) * (s32)SIN[ImpactAngle2])>>8;
					p2->yspeed+= (s32)((speed) * (s32)COS[ImpactAngle2])>>8;

					if (p2->xspeed+p2->yspeed==0)
					{
						p2->xspeed-= (s32)((4) * (s32)SIN[ImpactAngle2])>>8;
						p2->yspeed+= (s32)((4) * (s32)COS[ImpactAngle2])>>8;
					}
*/
/*




					s32 x=p1->xspeed;
					s32 y=p1->xspeed;

					p1->xspeed=((p1->xspeed*p1->mass)-(p2->xspeed*(p2->mass+1)))/p1->mass;
					p1->yspeed=((p1->yspeed*p1->mass)-(p2->yspeed*(p2->mass+1)))/p1->mass;

					p2->xspeed=((p2->xspeed*p2->mass)-(y*(p1->mass+1)))/p2->mass;
					p2->yspeed=((p2->xspeed*p2->mass)-(x*(p1->mass+1)))/p2->mass;
					*/
					/*
					print("after");
									print("\n player 1 xpseed");
														print(p1->xspeed);
														print("\n player 1 ypseed");
														print(p1->yspeed);
														print("\n player 2 xpseed");
														print(p2->xspeed);
														print("\n player 2 ypseed");
					print(p2->yspeed);
					*/
				}
				/*
					if (p1->xspeed==0&&p2->xspeed!=0)
					{
									//p1 stationary /p2 not
									s32 x=p2->xspeed;
									p1->xspeed=x/p2->mass;
				}
									s32 y=p2->xspeed;


									p1->yspeed=y/p2->mass;

									p2->xspeed=x/(p1->mass*2);
									p2->yspeed=x/(p1->mass*2);
								}
								else if (p2->xspeed==0&&p2->yspeed==0&&(p1->xspeed!=0||p1->yspeed!=0))
								{
									//p2 stationary /p1 not
									s32 x=p1->xspeed;
									s32 y=p1->xspeed;

									p2->xspeed=x/p1->mass;
									p2->yspeed=y/p1->mass;

									p1->xspeed=x/(p2->mass*2);
									p1->xspeed=x/(p2->mass*2);
								}
								else if ((p2->xspeed!=0||p2->yspeed!=0)&&(p1->xspeed!=0||p1->yspeed!=0))
								{
									//both ships moving
									s32 x=p1->xspeed;
									s32 y=p1->xspeed;

									p1->xspeed=(p1->xspeed*p1->mass)-(p2->xspeed*(p2->mass+1));
									p1->yspeed=(p1->yspeed*p1->mass)-(p2->yspeed*(p2->mass+1));

									p2->xspeed=(p2->xspeed*p2->mass)-(y*(p1->mass+1));
									p2->yspeed=(p2->xspeed*p2->mass)-(x*(p1->mass+1));
				}
				*/

			}
			setScreen(p1,p2,bg0,bg1);


  			//asteroids
  			ProcessAsteroids(p1,p2);
  			//planet
  			DrawPlanet();
  			CalcPlanet(p1);
  			CalcPlanet(p2);



			MoveBullets(p1);

			MoveBullets(p2);

			MoveSprite(&sprites[0], p1->xscreen, p1->yscreen);

			MoveSprite(&sprites[13], p2->xscreen, p2->yscreen);




			turn++;

			if (turn==5)
			{
				turn=0;

			}

			DetectBullets(p1);
			DetectBullets(p2);

			DrawTrails();


			//UpdateStatus();  now we only update when changed
/*
			if (p1->crew>0)
				Regen(p1);
			if (p2->crew>0)
				Regen(p2);
*/			if (p1->crew>0)
				Regen(p1);
			if (p2->crew>0)
				Regen(p2);

			if (p1->postfunc!=0)
				p1->postfunc(p1);
			if (p2->postfunc!=0)
				p2->postfunc(p2);

			RotateSprite(0, p1->angle, zoom, zoom);
			RotateSprite(13, p2->angle, zoom,zoom);

			WaitForVsync();					//waits for the screen to stop drawing
			UpdateBackground(bg0);
			UpdateBackground(bg1);
			CopyOAM();			//Copies sprite array into OAM.

			if (demo)
			{
				if((!(*KEYS & KEY_START)) || (!(*KEYS & KEY_SELECT)) ||
					(!(*KEYS & KEY_A)) || (!(*KEYS & KEY_B)) ||
					(!(*KEYS & KEY_L)) || (!(*KEYS & KEY_R)))
				return;
			}


		}//end while one or both ships are destroyed

	//should clear screen
	InitializeSprites();
	CopyOAM();
}




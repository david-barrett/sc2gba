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

extern u16* OAM;
extern OAMEntry sprites[128];
extern pRotData rotData;

extern Player p1;
extern Player p2;

extern Bg bg0;
extern Bg bg1;

int turn;


extern FIXED SIN2[360];	    //Look-Up Tabless for sign and cosign
extern FIXED COS2[360];
extern FIXED ATAN[360];

double scale;

s32 zoom,screenx,screeny;

//centre of screen
//note ships draw from TOP_LEFT
s32 centrex=120;
s32 centrey=80;

#define ASIZE  512 //128 // 512 ?

/*
   // ARM code
   void print(char *s)
   {
     asm volatile("mov r0, %0;"
                  "swi 0xff0000;"
                  : // no ouput
                  : "r" (s)
                  : "r0");
   }
*/

//Copy our sprite array to OAM
void CopyOAM()
{
	u16 loop;
	u16* temp;
	temp = (u16*)sprites;
	for(loop = 0; loop < 128*4; loop++)
	{
		OAM[loop] = temp[loop];
	}
}

//Set sprites to off screen
void InitializeSprites()
{
	u16 loop;
	for(loop = 0; loop < 128 ;loop++)
	{
		sprites[loop].attribute0 = 160;  //y to > 159
		sprites[loop].attribute1 = 240; //x to > 239
		sprites[loop].attribute2 = 0;
	}
}

//wait for the screen to stop drawing
void WaitForVsync()
{
	while((volatile u16)REG_VCOUNT != 160){}
}

void swap(s32 *one, s32 *two)
{
	s32 three;
	three=*one;
	*one=*two;
	*two=three;
}
/*

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
	else if (life<5)
	{
			off=10;
			if (life==4)
				off=18;
			else if (life==5)
				off=26;
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
	DrawExplosion(w);
}

void DetonateShip(pPlayer pl)
{

	for (int i=0;i<12;i++)
	{
		if (pl->weapon[i].life==-1)
		{
			pl->weapon[i].xpos=pl->xpos+i-8;
			pl->weapon[i].ypos=pl->ypos+i-8;
			CreateExplosion(&pl->weapon[i],5);
		}
	}
}

void Detect(pPlayer p,pWeapon w)
{
	//square (w.xpos-4,w.ypos-4), (w.xpos+4,w.ypos+4)
	//square (p.xpos-16,w.ypos-16), (w.xpos+16,w.ypos+16)

	s16 offset=p->offset/2;

	if (w->xpos>p->xpos-offset&&w->xpos<p->xpos+offset&&w->ypos>p->ypos-offset&&w->ypos<p->ypos+offset)
	{
		p->crew-=w->damage;
		CreateExplosion(w,5);
	}

}


void TurnRight(pPlayer pl)
{
	if (pl->turn_turn==0)
	{
	pl->turn_turn==pl->turn_wait;
	pl->angle+=1;
	if(pl->angle>=360)
		pl->angle-=360;;
	}
}

void TurnLeft(pPlayer pl)
{
	if (pl->turn_turn==0)
	{
	pl->turn_turn==pl->turn_wait;
	pl->angle-=1;
	if(pl->angle<0)
		pl->angle+=360;;
	}
}

void GetInput()
{
	if(!(*KEYS & KEY_UP))                   //if the UP key is pressed
	{
		Thrust(&p1);
	}
	if(!(*KEYS & KEY_DOWN))                 //if the DOWN key is pressed
	{
	}
	if(!(*KEYS & KEY_LEFT))                 //if the LEFT key is pressed
	{
        TurnLeft(&p1);
	}
	if(!(*KEYS & KEY_RIGHT))                //if the RIGHT key is pressed
	{
		TurnRight(&p1);
	}
	if(!(*KEYS & KEY_A))                	//if the A key is pressed
	{
       Fire(&p1);
    }
	if(!(*KEYS & KEY_B))                	//if the B key is pressed
	{
		Special(&p1);
    }
	if(!(*KEYS & KEY_L))                	//if the L key is pressed
	{

	}
	if(!(*KEYS & KEY_R))                	//if the R key is pressed
	{

	}
	if(!(*KEYS & KEY_SELECT))               //if the SELECT key is pressed
	{

	}
	if(!(*KEYS & KEY_START))                //if the START key is pressed
	{
		do
		{
		}while(!(*KEYS & KEY_START));

	}
}

*/

//move the sprite
void MoveSprite(OAMEntry* sp, int x, int y)
{
	sp->attribute1 = sp->attribute1 & 0xFE00;  //clear the old x value
	sp->attribute1 = sp->attribute1 | x;

	sp->attribute0 = sp->attribute0 & 0xFF00;  //clear the old y value
	sp->attribute0 = sp->attribute0 | y;
}
/*
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

int InRange()
{
	return 0;
}


void MoveURFighters(pWeapon ur)
{
	pPlayer target=(pPlayer)ur->target;
	pPlayer parent=(pPlayer)ur->parent;
	ur->status--;
	s16 angle;
	//if out too long return
	if (ur->status==0)
	{
		//been out too long die
		ur->life=-1;
	}
	if (ur->status<200)
	{
		//if reached mother ship dock
		if (0)
		{
			ur->life=-1;
			//mothership crew++;
			parent->crew++;
			return;
		}
		//desired angle = mothership
		angle = FindAngle(ur->xpos,ur->ypos,parent->xpos,parent->ypos);

	}
	else
	{

		//if in range fire
		if (InRange())//calc dist from target
		{
			if (ur->status%3==0) //otherwise const firing
			{
				//fire -
				target->crew--;
			}
			angle=target->angle; // turn to match target angle;
		}
		else // not in range
		{

			// turn towards opp
			angle = FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos);

		}
	}

	//now calc if should turn
	s32 a = angle+360;
	s32 b = ur->angle+360;

	if (a<b)
	{
		ur->angle-=15;
			if (ur->angle<0)
		ur->angle+=360;
	}
	else if (b<a)
	{
		ur->angle+=15;
			if (ur->angle>360)
		ur->angle-=360;
	}


	//if facing right way thrust
	s16 a1 = 360+angle;
	s16 a2 = 360+ur->angle;
	if (a1>a2-30&&a1<a2+30)//thrust if going roughly the right way
	{
		s32 x = ((3) * (s32)SIN[ur->angle])>>8;
		s32 y = ((3) * (s32)COS[ur->angle])>>8;

		ur->xspeed = (ur->xspeed + x)/2;
		ur->yspeed = (ur->yspeed + y)/2;

	}


		//always do
		ur->xpos+=ur->xspeed;
		ur->ypos-=ur->yspeed;
}

void MoveBullets()
{

	for (int i=0;i<12;i++)
			{
				if (p1.weapon[i].life==0)//destroy it
				{
					sprites[p1.weapon[i].sprite].attribute0 = 160;  //y to > 159
					sprites[p1.weapon[i].sprite].attribute1 = 240; //x to > 239
					sprites[p1.weapon[i].sprite].attribute2 = 0;
					MoveSprite(&sprites[p1.weapon[i].sprite], p1.weapon[i].xscreen, p1.weapon[i].yscreen);
					RotateSprite(p1.weapon[i].sprite, p1.weapon[i].angle, zoom,zoom);
					p1.weapon[i].life--;

				}
				if (p1.weapon[i].life>0)
				{
					if (p1.weapon[i].type==SIMPLE)
				    {
						p1.weapon[i].xpos+=p1.weapon[i].xspeed;
						p1.weapon[i].ypos-=p1.weapon[i].yspeed;


						p1.weapon[i].life--;


						Detect(&p2,&p1.weapon[i]);
						if (p1.weapon[i].damageparent==1)
							Detect(&p1,&p1.weapon[i]);

					}//simple
					else if (p1.weapon[i].type==UR_FIGHTERS)
					{
						MoveURFighters(&p1.weapon[i]);
					}
					else if(p1.weapon[i].type==EXP)
					{
						p1.weapon[i].life--;
						DrawExplosion(&p1.weapon[i]);
					}
					drawOnScreen(&p1.weapon[i].xscreen,&p1.weapon[i].yscreen,
					p1.weapon[i].xpos,p1.weapon[i].ypos,screenx,screeny,p1.weapon[i].size);
					MoveSprite(&sprites[p1.weapon[i].sprite], p1.weapon[i].xscreen, p1.weapon[i].yscreen);
					RotateSprite(p1.weapon[i].sprite, p1.weapon[i].angle, zoom,zoom);
				}


				if (p2.weapon[i].life==0)//destroy it
				{

					sprites[p2.weapon[i].sprite].attribute0 = 160;  //y to > 159
					sprites[p2.weapon[i].sprite].attribute1 = 240; //x to > 239
					sprites[p2.weapon[i].sprite].attribute2 = 0;
					MoveSprite(&sprites[p2.weapon[i].sprite], p2.weapon[i].xscreen, p2.weapon[i].yscreen);
					RotateSprite(p2.weapon[i].sprite, p2.weapon[i].angle, zoom,zoom);
					p2.weapon[i].life--;
				}
				if (p2.weapon[i].life>0)
				{
					if (p2.weapon[i].type==SIMPLE)
				    {
					p2.weapon[i].xpos+=p2.weapon[i].xspeed;
					p2.weapon[i].ypos-=p2.weapon[i].yspeed;

					p2.weapon[i].life--;

					Detect(&p1,&p2.weapon[i]);
					if (p2.weapon[i].damageparent==1)
							Detect(&p2,&p2.weapon[i]);
					}//simple
					else if (p1.weapon[i].type==UR_FIGHTERS)
					{
						MoveURFighters(&p2.weapon[i]);
					}
					else if(p2.weapon[i].type==EXP)
					{
						p2.weapon[i].life--;
						DrawExplosion(&p2.weapon[i]);
					}
					drawOnScreen(&p2.weapon[i].xscreen,&p2.weapon[i].yscreen,
					p2.weapon[i].xpos,p2.weapon[i].ypos,screenx,screeny,p2.weapon[i].size);


					MoveSprite(&sprites[p2.weapon[i].sprite], p2.weapon[i].xscreen, p2.weapon[i].yscreen);
					RotateSprite(p2.weapon[i].sprite, p2.weapon[i].angle, zoom,zoom);

				}

			}


}


void Thrust(pPlayer plr)
{
	if (plr->thrust_turn==0)
	{
	plr->thrust_turn==plr->thrust_wait;
	s32 x,y;

	if (plr->angle==plr->thrustangle)
	{
		plr->speed+=plr->accel_inc;
		if (plr->speed>plr->maxspeed)
			plr->speed=plr->maxspeed;
	}
	else
	{
		plr->speed=plr->accel_inc;
		plr->thrustangle=plr->angle;
	}
    x = ((plr->speed) * (s32)SIN[plr->angle])>>8;
	y = ((plr->speed) * (s32)COS[plr->angle])>>8;

	plr->xspeed = (plr->xspeed + x)/(SPEED_REDUCT*2);
	plr->yspeed = (plr->yspeed + y)/(SPEED_REDUCT*2);
	}

}

void Regen(pPlayer pl)
{

	if (pl->batt_turn>0)
		pl->batt_turn--;
	if (pl->turn_turn>0)
		pl->turn_turn--;
	if (pl->thrust_turn>0)
		pl->thrust_turn--;
	if (pl->weapon_turn>0)
		pl->weapon_turn--;
	if (pl->special_turn>0)
		pl->special_turn--;

	if (pl->batt<pl->maxbatt&&pl->batt_turn==0)
	{
		pl->batt_turn=pl->batt_wait;
		pl->batt++;
	}

}
*/
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

s32 distanceBetweenShips()
{
	s32 x,y,z;
	if (p1.xpos>p2.xpos)
		x=p1.xpos-p2.xpos;
	else
		x=p2.xpos-p1.xpos;
	if (p1.ypos>p2.ypos)
		y=p1.ypos-p2.ypos;
	else
		y=p2.ypos-p1.ypos;

	z = (s32)sqrt((x*x)+(y*y));
	return z;
}

void findCentre(s32 *x,s32 *y)
{
	*x = ((p1.xpos+p2.xpos)/2);//-32;
	*y = ((p1.ypos+p2.ypos)/2);//-32;
}

void setScreen()
{
	s32 x, y, changex,changey;
	findCentre(&x,&y);

	changex = screenx-x;
	changey = screeny-y;


	bg0.x_scroll-=2*changex;
    bg1.x_scroll-=changex;

	bg0.y_scroll-=2*changey;
    bg1.y_scroll-=changey;

	screenx=x;
	screeny=y;

	x=screenx-50;
	y=screenx-50;




	s32 d= distanceBetweenShips();
	//Calcuate zoom level
	if (d<160)
	{
		zoom = 1<<8; //256
		scale = 1;
	}
	else if (d < 320) //needs calc
	{
		zoom=1<<9;//512
		scale = 0.5;

	}
	else
	{
		zoom=768;
		scale = 0.25;

		//if reached the end of space swap ships

		s32 xd = p1.xpos-p2.xpos;
		if (xd<-960 || xd>960)
			swap(&p1.xpos,&p2.xpos);
		s32 yd = p1.ypos-p2.ypos;
		if (yd<-640 || yd>640)
			swap(&p1.ypos,&p2.ypos);

	}

	drawOnScreen(&p1.xscreen,&p1.yscreen,p1.xpos,p1.ypos,screenx,screeny,32);
	drawOnScreen(&p2.xscreen,&p2.yscreen,p2.xpos,p2.ypos,screenx,screeny,32);

	//drawOnScreen(&p1.weapon[0].xscreen,&p1.weapon[0].yscreen,p1.weapon[0].xpos,p1.weapon[0].ypos,screenx,screeny);

/*
	p1.xscreen=(s16)((p1.xpos-screenx)*scale)+centrex;//scaling ??
	p1.yscreen=(s16)((p1.ypos-screeny)*scale)+centrey;

	p2.xscreen=(s16)((p2.xpos-screenx)*scale)+centrex;
	p2.yscreen=(s16)((p2.ypos-screeny)*scale)+centrey;

	if (p1.xscreen <0)
		p1.xscreen+=512;
	if (p1.yscreen <0)
		p1.yscreen+=255;

	if (p2.xscreen <0)
		p2.xscreen+=512;
	if (p2.yscreen <0)
		p2.yscreen+=255;
*/
}

void drawOnScreen(s16* x,s16* y,s32 xpos,s32 ypos, s16 screenx, s16 yscreen,s16 size)
{
	*x = (s16)(((xpos-screenx)*scale)+centrex-(size*scale));
	*y = (s16)(((ypos-screeny)*scale)+centrey-(size*scale));


		*x = (s16)(((xpos-screenx)*scale)+centrex-(size));
	*y = (s16)(((ypos-screeny)*scale)+centrey-(size));

	if (*x <0)
		*x+=512;
	if (*y <0)
		*y+=255;

}
/*
void aiTurn(pPlayer ai)
{
	pPlayer opp = (pPlayer)ai->opp;
	//complex AI here!

	//ai->angle = FindAngle(ai->xpos,ai->ypos,opp->xpos,opp->ypos);

	s16 angle = FindAngle(ai->xpos,ai->ypos,opp->xpos,opp->ypos);

	s32 a = angle+360;
	s32 b = ai->angle+360;

	if (a<b)
	{
			TurnLeft(ai);
	}
	else if (b<a)
	{
			TurnRight(ai);
	}

	/*

	s32 t1=ai->angle-angle+720;
	s32 t2=angle-ai->angle+720;

	if (t2<t1)
	{
		TurnLeft(ai);
	}
	else if (t1<t2)
	{
		TurnRight(ai);
	}
*/
/*
 	if (turn==6)
	Fire(ai);
}

*/
void Melee()
{
	u32 loop;       //generic loop variable
	//print("Melee started");

	screenx=100;
	screeny=100;

	SetupBackground();

	InitializeSprites();                       //set all sprites off screen (stops artifact)

	setScreen();

		//create pl1 + pl2
       	sprites[0].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | p1.yscreen;	//setup sprite info, 256 colour, shape and y-coord
        sprites[0].attribute1 = SIZE_32 | ROTDATA(0) | p1.xscreen;            //size 32x32 and x-coord
        sprites[0].attribute2 = p1.SpriteStart | PRIORITY(1);                      //pointer to tile where sprite starts

		sprites[13].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT |p2.yscreen;//setup sprite info, 256 colour, shape and y-coord
        sprites[13].attribute1 = SIZE_32 | ROTDATA(13) | p2.xscreen;           //size 16x16 and x-coord
        sprites[13].attribute2 = p2.SpriteStart | PRIORITY(1);     //pointer to tile where sprite starts

// test show explos
/* handy for testing sprites
        	sprites[60].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[60].attribute1 = SIZE_8 | 8;
			   	sprites[60].attribute2 = FireSprite1 | PRIORITY(0);

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


		for (int i=0;i<15;i++)
		{
			p1.weapon[i].life=0;
			p2.weapon[i].life=0;
		}
		LoadExp(OAMFireSprite1,FireSprite1);
		LoadShips();
		SetupStatus();
		setScreen();
		turn=0;
		WaitForVsync();
		s8 EndGame=20;

	//while(1)
		while(EndGame)                                //main loop
        {

		if (p1.crew>0)
		{
	//	GetInput();//&p1.xpos,&p1.ypos);//the input function, checks for key presses
		p1.xpos+=p1.xspeed;
	    p1.ypos-=p1.yspeed;
		}
		else
		{
			EndGame--;
			//DetonateShip(&p1);

		}
		if (p2.crew>0)
		{
	   // aiTurn(&p2);
	    p2.xpos+=p2.xspeed;
		p2.ypos-=p2.yspeed;
		}
		else
		{
			EndGame--;
		//	DetonateShip(&p2);
		}

	    //setZoom();

		setScreen();

		MoveSprite(&sprites[0], p1.xscreen, p1.yscreen);

		MoveSprite(&sprites[13], p2.xscreen, p2.yscreen);

	//	MoveBullets();


		turn++;

		if (turn==9)
		{
			turn=0;
		}


		RotateSprite(0, p1.angle, zoom, zoom);
		RotateSprite(13, p2.angle, zoom,zoom);

		UpdateStatus();
		WaitForVsync();					//waits for the screen to stop drawing
		UpdateBackground(&bg0);
		UpdateBackground(&bg1);
		CopyOAM();			//Copies sprite array into OAM.
/*
		if (p1.crew>0)
			Regen(&p1);
		if (p2.crew>0)
			Regen(&p2);
*/
	}//end while one or both ships are destroyed
	InitializeSprites();
}



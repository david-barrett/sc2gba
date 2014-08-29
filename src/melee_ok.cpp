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

pTrail trails;

int turn;

s16 planet_screenx,planet_screeny;



extern FIXED SIN2[360];	    //Look-Up Tabless for sign and cosign
extern FIXED COS2[360];

const s8 EXPX[12] = {5,-10,9,-2,7,0,-6,4,-12,13,-14,8};
const s8 EXPY[12] = {-7,0,-15,12,-5,4,-9,10,-4,9,-2,4};



double scale;

s32 zoom,screenx,screeny;

//centre of screen
//note ships draw from TOP_LEFT
s32 centrex=120;
s32 centrey=80;

#define ASIZE  512 //128 // 512 ?


char s[100];

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
}


}

void
print(s32 s)
{

	s32 t=s/10000;
	printi(t);
	s=s-(t*10000);

	t=s/1000;
		printi(t);
	s=s-(t*1000);

	t=s/100;
		printi(t);
	s=s-(t*100);

	t=s/10;
	printi(t);
	s=s-(t*10);

	printi(s%10);

}

void GenerateStart(pPlayer p)
{
	int good=1;
	do
	{
		p->angle=ran(0,359);
		p->xpos=ran(0,4000);
		p->ypos=ran(0,4000);

		s32 xspeed = ((20) * (s32)SIN[p->angle])>>8;
	    s32 yspeed = ((20) * (s32)COS[p->angle])>>8;
	    s32 xpos = p->xpos;
	    s32 ypos = p->ypos;
		//check we aint going to hit a planet
		for (int i=0;i<6;i++)
		{
			s16 dist=distanceBetweenPoints(xpos,ypos,planetx,planety);
			//if (dist<(p->offset+64)/2)
			if (dist<200)
			{
				//bad try again
				good=0;
				break;
			}
			xpos+=xspeed;
			ypos+=yspeed;
		}
	}
	while(good);
}



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
			pl->weapon[i].xpos=pl->xpos+(s32)(EXPX[i]*scale);
			pl->weapon[i].ypos=pl->ypos+(s32)(EXPY[i]*scale);
			CreateExplosion(&pl->weapon[i],5);
		}
	}
}

int DetectWeaponToShip(pPlayer p,pWeapon w)
{
	//square (w.xpos-4,w.ypos-4), (w.xpos+4,w.ypos+4)
	//square (p.xpos-16,w.ypos-16), (w.xpos+16,w.ypos+16)

	s16 offset=p->offset/2;

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

	s16 offset=tar->size/2;

	if (w->xpos>tar->xpos-offset&&w->xpos<tar->xpos+offset&&w->ypos>tar->ypos-offset&&w->ypos<tar->ypos+offset)
	{
		return 1;
	}
	return 0;

}


void TurnRight(pPlayer pl, int i=15)
{
	if (pl->turn_turn==0)
	{
	pl->turn_turn==pl->turn_wait;
	pl->angle+=i;
	if(pl->angle>=360)
		pl->angle-=360;;
	}
}

void TurnLeft(pPlayer pl,int i=15)
{
	if (pl->turn_turn==0)
	{
	pl->turn_turn==pl->turn_wait;
	pl->angle-=i;
	if(pl->angle<0)
		pl->angle+=360;;
	}
}

void GetInput(pPlayer pl)
{
	if(!(*KEYS & KEY_UP))                   //if the UP key is pressed
	{
		Thrust(pl);
	}
	if(!(*KEYS & KEY_DOWN))                 //if the DOWN key is pressed
	{
	}
	if(!(*KEYS & KEY_LEFT))                 //if the LEFT key is pressed
	{
        TurnLeft(pl);
	}
	if(!(*KEYS & KEY_RIGHT))                //if the RIGHT key is pressed
	{
		TurnRight(pl);
	}
	if(!(*KEYS & KEY_A))                	//if the A key is pressed
	{
       Fire(pl);
    }
	if(!(*KEYS & KEY_B))                	//if the B key is pressed
	{
		Special(pl);
    }
	if(!(*KEYS & KEY_L))                	//if the L key is pressed
	{
		print("\n sprite 1 attr 2");
		print(sprites[1].attribute2);

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


void MoveURFighters(pWeapon ur)
{
	s16 angle;
	pPlayer target=(pPlayer)ur->target;

	pPlayer parent=(pPlayer)ur->parent;
	ur->status--;

	//if out too long return
	if (ur->status==0)
	{
		//been out too long die
		ur->life=0;
		return;
	}
	if (target->crew==0&&ur->status>199)
		ur->status=199;
	if (ur->status<200)
	{
		//if reached mother ship dock
		if (DetectWeaponToShip(parent,ur)==1)
		{
			ur->life=-1;
			//mothership crew++;
			ModifyCrew(parent,1);
			return;
		}
		//desired angle = mothership
		angle = FindAngle(ur->xpos,ur->ypos,parent->xpos,parent->ypos);

	}
	else
	{

		//if in range fire
		if (InRange(ur->xpos,ur->ypos,target->xpos,target->ypos,8+(target->offset/2)))//calc dist from target
		{
			if (ur->status%3==0) //otherwise const firing
			{
				//fire -
				FightersFire(ur,FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos));
			}
			angle=target->angle; // turn to match target angle;
		}
		else // not in range
		{

			// turn towards opp
			angle = FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos);

		}
	}

	int ret=TurnAngle(angle,ur->angle,15);
	if (ret==0)
	{
		s32 x = ((6) * (s32)SIN[ur->angle])>>8;
				s32 y = ((6) * (s32)COS[ur->angle])>>8;

				ur->xspeed = (ur->xspeed + x)/2;
		ur->yspeed = (ur->yspeed + y)/2;
	}
	else if (ret<0)
		{
			ur->angle-=15;
					if (ur->angle<0)
			ur->angle+=360;
	}
	else if (ret>0)
		{
			ur->angle+=15;
					if (ur->angle>360)
			ur->angle-=360;
	}
/*
	//now calc if should turn
	s32 a = angle+360;
	s32 b = ur->angle+360;

	if (a<b)
	{
		ur->angle-=15;
							if (ur->angle<0)
			ur->angle+=360
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
*/
	//always do
	ur->xpos+=ur->xspeed;
	ur->ypos-=ur->yspeed;
}

void MoveBullets(pPlayer pl)
{
	pPlayer p;
	pPlayer o;

	s32 xp1 = p->xpos;
	s32 xp2 = o->xpos;

	for (int i=0;i<12;i++)
	{


		if (pl->weapon[i].life>-1)
		{
			print("\n sprite attr 2");
			print(sprites[pl->weapon[i].sprite].attribute2);
		}
		if (pl->weapon[i].life==0)//destroy it
		{

			print("\nattr 1 before ");
			print(sprites[pl->weapon[i].sprite].attribute1);
			sprites[pl->weapon[i].sprite].attribute0 = 160;  //y to > 159
			sprites[pl->weapon[i].sprite].attribute1 = 240; //x to > 239
			sprites[pl->weapon[i].sprite].attribute2 = 0;
			MoveSprite(&sprites[pl->weapon[i].sprite], pl->weapon[i].xscreen, pl->weapon[i].yscreen);
			//RotateSprite(pl->weapon[i].sprite, pl->weapon[i].angle, zoom,zoom);
			pl->weapon[i].life--;
			print("\ndestroyed weapon ");
			print(i);
			print("attr 1 after ");
			print(sprites[pl->weapon[i].sprite].attribute1);


		}
		if (pl->weapon[i].life>0)
		{
			pPlayer target=(pPlayer)pl->weapon[i].target;
			if (pl->weapon[i].type==SIMPLE)
			{
				pl->weapon[i].xpos+=pl->weapon[i].xspeed;
				pl->weapon[i].ypos-=pl->weapon[i].yspeed;


				pl->weapon[i].life--;

				int stop=1;  //stop processing when destroyedto save time
				//opp ship
				if (DetectWeaponToShip(target,&pl->weapon[i]))
				{
					ModifyCrew(target,pl->weapon[i].damage);
					CreateExplosion(&pl->weapon[i],5);
					stop=0;
				}
				//parent ship
				if (pl->weapon[i].damageparent==1&&stop)
					if (DetectWeaponToShip((pPlayer)pl->weapon[i].parent,&pl->weapon[i]))
					{
						ModifyCrew((pPlayer)pl->weapon[i].parent,pl->weapon[i].damage);
						CreateExplosion(&pl->weapon[i],5);
						stop=0;
					}

				//opp weapon
				if (stop)
				{
				for (int j=0;j<12;j++)
				{
					if (target->weapon[j].life>0)
					if (DetectWeaponToWeapon(&target->weapon[j],&pl->weapon[i])==1)
					{
						pl->weapon[i].life=0;
						target->weapon[j].life=0;
						break;
					}
				}
				}
/* TODO
				//planet
				if (stop);

				//asteroids
				if (stop);
*/
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
			drawOnScreen(&(pl->weapon[i].xscreen),&(pl->weapon[i].yscreen),
				pl->weapon[i].xpos,pl->weapon[i].ypos,screenx,screeny,pl->weapon[i].size);
			MoveSprite(&sprites[pl->weapon[i].sprite], pl->weapon[i].xscreen, pl->weapon[i].yscreen);
			RotateSprite(pl->weapon[i].sprite, pl->weapon[i].angle, zoom,zoom);
		}//end if life>0
	}//end for loop

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
	CreateTrail(plr);
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
	if (pl->aiturn>0)
		pl->aiturn--;

	if (pl->batt<pl->maxbatt&&pl->batt_turn==0)
	{
		pl->batt_turn=pl->batt_wait;
		ModifyBatt(pl,1);
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

void findCentre(s32 *x,s32 *y,pPlayer p1,pPlayer p2)
{
	*x = ((p1->xpos+p2->xpos)/2);//-32;
	*y = ((p1->ypos+p2->ypos)/2);//-32;


}

void setScreen(pPlayer p1,pPlayer p2,Bg* bg0,Bg* bg1)
{
	s32 x, y, changex,changey;
	findCentre(&x,&y,p1,p2);

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
	else if (d < 320) //needs calc
	{
		zoom=1<<9;//512
		scale = 0.5;

	}
	else
	{
		zoom=768;
		scale = 0.25;
		//scale=4;

		//if reached the end of space swap ships

		s32 xd = p1->xpos-p2->xpos;
		if (xd<-960 || xd>960)
			swap(&p1->xpos,&p2->xpos);
		s32 yd = p1->ypos-p2->ypos;
		if (yd<-640 || yd>640)
			swap(&p1->ypos,&p2->ypos);

	}

	drawOnScreen(&p1->xscreen,&p1->yscreen,p1->xpos,p1->ypos,screenx,screeny,32,1);
	drawOnScreen(&p2->xscreen,&p2->yscreen,p2->xpos,p2->ypos,screenx,screeny,32,1);

	// want to reset so values dont overflow
	/*
	if(p1->xpos>2000||
		p2->xpos>2000)
	{
		p1->xpos-=1000;
		p2->xpos-=1000;
	}
	if (p1->ypos>2000||
		p2->ypos>2000)

	{
		p1->ypos-=1000;
		p2->ypos-=1000;
	}
	if(p1->xpos<-2000||
		p2->xpos<-2000)
	{
		p1->xpos+=1000;
		p2->xpos+=1000;
	}
	if(p1->ypos<-2000||
		p2->ypos<-2000)
	{
		p1->ypos+=1000;
		p2->ypos+=1000;
	}
	*/

}

void drawOnScreen(s16* x,s16* y,s32 xpos,s32 ypos, s16 screenx, s16 yscreen,s16 size,s16 pl)
{
//	*x = (s16)(((xpos-screenx)*scale)+centrex-(size/scale));
//	*y = (s16)(((ypos-screeny)*scale)+centrey-(size/scale));


	*x = (s16)(((xpos-screenx)*scale)+centrex-(size));
	*y = (s16)(((ypos-screeny)*scale)+centrey-(size));

//	*x = (s16)(((xpos-screenx)*scale)+centrex);
//	*y = (s16)(((ypos-screeny)*scale)+centrey);

	if (!pl)
	{
		//dont draw on screen if out of bounds
		if (*x<0-16||*x>240+16||*y<0-16||*y>160+16)
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

void aiTurn(pPlayer ai)
{
	//do special?
	if (ai->aiturn==0)
	{
		if (ai->ai!=STANDARD)//only do specials on higher skill
		{

			if(ai->aispecial==0)
			{
				if (1)//decide if do special
					ai->aispecial=1;
			}
			if (ai->aispecial==1)//do special
			{
				if (ai->batt<ai->specbatt)
				{
					if(Special(ai)>0)
						ai->aispecial=0;
				}
			}

		}

	pPlayer opp = (pPlayer)ai->opp;
	s16 angle = FindAngle(ai->xpos,ai->ypos,opp->xpos,opp->ypos);
	//s16 desangle=angle;

	int InRangeToFire=InRange(ai->xpos,ai->ypos,opp->xpos,opp->ypos,ai->range);

	int InRangeToBeHit=CanHitOpp(opp)&&InRange(opp->xpos,opp->ypos,ai->xpos,ai->ypos,opp->range);

	if(InRangeToBeHit)
	{
		angle=angle+180;//should work this out better!
		if (angle>360)
			angle-=360;
		int ret=TurnAngle(angle,ai->angle,90);
		if (ret==0)
			Thrust(ai);
		else if (ret<0)
			TurnLeft(ai);
		else if (ret>0)
			TurnRight(ai);

	}

	else if (InRangeToFire)
	{
		//desange=angle still

		//are we pointing towards opp? if so fire
		//s16 a1 = 360+angle;
		//s16 a2 = 360+ai->angle;

		int x=30;
		if (ai->ship==FURY)
			x=100;


		if (ai->aispecial==0&&TurnAngle(angle,ai->angle,x)==0)//fire if going roughly the right way
		{
			Fire(ai);
		}

		int ret=TurnAngle(angle,ai->angle,15);
		if (ret<0)
			TurnLeft(ai);
		else if (ret>0)
			TurnRight(ai);



	}
	else //not in range
	{
		if (ai->range<opp->range)//we have shorter range
		{
			//choose a spot to the side of opponent
			//again better
			angle=angle-40;
		}

		int ret=TurnAngle(angle,ai->angle,30);
		if (ret==0)
			Thrust(ai);
		else if (ret<0)
			TurnLeft(ai);
		else if (ret>0)
			TurnRight(ai);

	}

	ai->aiturn=ai->ai;//*3;

	//do special?
	}	//end if turn
}

int GetNextTrail(int off)
{
	for (int i=0+off;i<5+off;i++)
	{
		if (trails[i].life==0)
			return i;
	}
	return -1;
}

void CreateTrail(pPlayer p)
{
	int t = GetNextTrail((p->plr-1)*5); //=p1=0 or p2=5

	s32 x = ((p->offset) * (s32)SIN[p->angle])>>8;
	s32 y = ((p->offset) * (s32)COS[p->angle])>>8;
	if (t!=-1)
	{
		trails[t].life=5;
		trails[t].xpos=p->xpos-(x);
		trails[t].ypos=p->ypos+(y);
	}
}

void DrawTrails()
{
	for (int i=0;i<10;i++)
	{
		if (trails[i].life>0)
		{
			drawOnScreen(&trails[i].xscreen,&trails[i].yscreen,trails[i].xpos,trails[i].ypos,screenx,screeny,4);
			sprites[32+i].attribute0 = COLOR_256 | SQUARE | MODE_TRANSPARENT | trails[i].yscreen;	//setup sprite info, 256 colour, shape and y-coord
			sprites[32+i].attribute1 = SIZE_8 | trails[i].xscreen;
    		sprites[32+i].attribute2 = TrailSprite | PRIORITY(1);
    		//sprites[32+i].attribute2 = FireSprite1+2 | PRIORITY(0); //if i need to see the trail big
    		trails[i].life--;
		}
		else if (trails[i].life==0)
		{
			MoveOffscreen(&sprites[32+i]);
		}

	}

}

int CreateOutline(pPlayer pl)
{

	s16 b = nextWeapon(pl);

	if (b>=0)
	{
	pl->weapon[b].type=TRAIL;
	pl->weapon[b].life=5;
	pl->weapon[b].damage=0;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=32;
	pl->weapon[b].angle = pl->angle;

	pl->weapon[b].xspeed = 0;
	pl->weapon[b].yspeed = 0;

	pl->weapon[b].xpos = pl->xpos;
	pl->weapon[b].ypos = pl->ypos;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+32 | PRIORITY(2);


    return 1;
	}
	return 0;
}

void ProcessPlayer(pPlayer pl,s8 *EndGame)
{
	if (pl->crew>0)
	{
		if (pl->warp>1)
		{
			CreateOutline(pl);
			pl->warp--;
		}
		else if(pl->warp==1)
		{
			pl->xspeed=0;
			pl->yspeed=0;
			pl->warp=0;
		}
		else
		{
			if (pl->ai==PLAYER1||pl->ai==PLAYER2)
				GetInput(pl);
			else if(pl->ai!=DISABLED)
				aiTurn(pl);

		}
		pl->xpos+=pl->xspeed;
		pl->ypos-=pl->yspeed;
	}
	else
	{
		if (pl->ship==FURY)
			if (DeathFury(pl))
				return;
		*EndGame=*EndGame-1;
		DetonateShip(pl);
	}
}

void Bounce(pPlayer pl)
{
	pl->xspeed*=-1;
	pl->yspeed*=-1;

	pl->xpos+-pl->xspeed;
	pl->xpos+-pl->xspeed;
}

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
		ModifyCrew(pl,-2);
		Bounce(pl);
	}
	//gravity
	if (pl->ship!=SKIFF&&dist<70)//maybe
	{
		s16 a = FindAngle(pl->xpos,pl->ypos,planetx,planety);

		s32 x = ((2) * (s32)SIN[a])>>8;
		s32 y = ((2) * (s32)COS[a])>>8;

		pl->xspeed = pl->xspeed + x;
		pl->yspeed = pl->yspeed + y;

	}
}

void Melee(pPlayer p1,pPlayer p2,Bg *bg0, Bg *bg1)
{
	u32 loop;       //generic loop variable
	print("Melee started");

	screenx=100;
	screeny=100;


	//holds planet vars
	//may make these global


	trails=(pTrail)malloc(sizeof(pTrail)*10);
	for (int i=0;i<10;i++)
	{
		trails->life=0;
	}


	SetupBackground(bg0,bg1);

	//try - should load in sc2.cpp but oam mem seems to be blank...
	LoadPal();
	LoadShip(p1);
	//LoadDreadnaught(p1->ship,p1->OAMStart);
	LoadShip(p2);
	LoadExp(OAMFireSprite1,FireSprite1);
	LoadTrail(OAMTrailSprite);
	LoadPlanet(OAMPlanetSprite);




	InitializeSprites();                       //set all sprites off screen (stops artifact)

	setScreen(p1,p2,bg0,bg1);

		//create pl1 + pl2
       	sprites[0].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | p1->yscreen;	//setup sprite info, 256 colour, shape and y-coord
        sprites[0].attribute1 = SIZE_32 | ROTDATA(0) | p1->xscreen;            //size 32x32 and x-coord
        sprites[0].attribute2 = p1->SpriteStart | PRIORITY(1);                      //pointer to tile where sprite starts

		sprites[13].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT |p2->yscreen;//setup sprite info, 256 colour, shape and y-coord
        sprites[13].attribute1 = SIZE_32 | ROTDATA(13) | p2->xscreen;           //size 16x16 and x-coord
        sprites[13].attribute2 = p2->SpriteStart | PRIORITY(1);     //pointer to tile where sprite starts
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




		SetupStatus(p1,p2);
		//setScreen(p1,p2);
		turn=0;
		WaitForVsync();
		s8 EndGame=20;
		//UpdateStatus();

		while(EndGame)                                //main loop
        {

			ProcessPlayer(p1,&EndGame);
			ProcessPlayer(p2,&EndGame);



  			//should also check for collisions with
  			//other player
  			//this isnt too great
  			if (distanceBetweenPoints(p1->xpos,p1->ypos,p2->xpos,p2->ypos)<(p1->offset+p2->offset)/2)
  			{
				ModifyCrew(p1,-2);
				ModifyCrew(p2,-2);
				Bounce(p1);
				Bounce(p2);
			}
  			//asteroids
  			//planet
  			CalcPlanet(p1);
  			CalcPlanet(p2);

			setScreen(p1,p2,bg0,bg1);

			MoveBullets(p1);

			MoveBullets(p2);

			MoveSprite(&sprites[0], p1->xscreen, p1->yscreen);

			MoveSprite(&sprites[13], p2->xscreen, p2->yscreen);


			turn++;

			if (turn==9)
			{
				turn=0;
				if (p1->crew>0)
					Regen(p1);
				if (p2->crew>0)
					Regen(p2);
			}


			DrawTrails();
			DrawPlanet();

			//UpdateStatus();  now we only update when changed
/*
			if (p1->crew>0)
				Regen(p1);
			if (p2->crew>0)
				Regen(p2);
*/


			RotateSprite(0, p1->angle, zoom, zoom);
			RotateSprite(13, p2->angle, zoom,zoom);

			WaitForVsync();					//waits for the screen to stop drawing
			UpdateBackground(bg0);
			UpdateBackground(bg1);
			CopyOAM();			//Copies sprite array into OAM.

		}//end while one or both ships are destroyed

	//should clear screen
	InitializeSprites();
	CopyOAM();
}




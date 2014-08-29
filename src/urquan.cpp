#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "gfx/urquan_out.h"  //urqan
#include "gfx/urquan_fire.h"
#include "gfx/urquan_fighters.h"
#include "gfx/urquan_fighters_fire.h"

#include "sound/urquan-fusion.h"
#include "sound/urquan-fighter.h"
#include "sound/urquan-recover.h"
#include "sound/urquan_launch.h"


extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

void LoadDreadnaught(s16 OAMStart, s16 SpriteStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = urquanData[loop-OAMStart];
       		OAMData[loop+512] = urquan_outData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512] = urquan_fireData[loop-OAMStart]; //loads some garb
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+512+128+512] = urquan_fightersData[loop-OAMStart]; //loads some garb
   	}

   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+512+128+32+512] = urquan_fighters_fireData[loop-OAMStart]; //loads some garb
   	}

}

int SpecialDreadnaught(pPlayer pl)
{
	//s16 n = nextSpecSprite(pl);
		s16 b;
		int res=0;
		for (int i =0;i<2;i++)
		{
		b= nextSpec(pl);



		if (b>=0&&pl->crew>1)//&&n>=0)
		{
		pl->weapon[b].type=UR_FIGHTERS;
		pl->weapon[b].life=1;//range
		pl->weapon[b].damage=4;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;
		//pl->weapon[b].sprite=n;
		pl->weapon[b].angle = pl->angle;
		pl->weapon[b].xpos = pl->xpos+((s32)(24 * (s32)SIN[pl->angle]));
		pl->weapon[b].ypos = pl->ypos-((s32)(24 * (s32)COS[pl->angle]));
		pl->weapon[b].size=8;
		pl->weapon[b].status=400;

		//this determines the middle of the ship which aint so good
		//pl->weapon[b].xpos = pl->xpos + 24;
		//pl->weapon[b].ypos = pl->ypos + 24;

		int e;
		e=(i==0?125:205);

		s16 a = pl->angle+e;
		if (a>360)
			a-=360;


		s32 x = ((pl->offset) * (s32)SIN[a])>>8;
		s32 y = ((pl->offset) * (s32)COS[a])>>8;

		pl->weapon[b].xpos=pl->xpos+(x);
		pl->weapon[b].ypos=pl->ypos-(y);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,8);

		s32 speed=0;
		pl->weapon[b].xspeed = 0;//(speed * (s32)SIN[pl->angle])>>8;
		pl->weapon[b].yspeed = 0;//(speed * (s32)COS[pl->angle])>>8;

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+72 | PRIORITY(1);
	    //sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart | PRIORITY(1);
	    ModifyCrew(pl,-1);
	    res+=1;
	    pl->fighters++;
		}


	}
	if (res>0)
		play_sfx(&UrQuan_Launch,pl->plr-1);
	return res;
}

int FightersFire(pWeapon f,s16 angle)
{
	//we know we are in range
	//so just draw gfx and damage
	pPlayer pl=(pPlayer)f->parent;
	pPlayer target=(pPlayer)f->target;

	s16 b= nextSpec(pl);

	if (b!=-1)
	{

		pl->weapon[b].type=UR_FIGHTERS_FIRE;
		pl->weapon[b].life=1;//range
		pl->weapon[b].angle = angle;
		pl->weapon[b].xpos = f->xpos;
		pl->weapon[b].ypos = f->ypos;
		pl->weapon[b].size=8;
		pl->weapon[b].xspeed = 0;
		pl->weapon[b].yspeed = 0;

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
				pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,8);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+74 | PRIORITY(0);

	    ModifyCrew(target,-1);
	    play_sfx(&UrQuan_Fighter,pl->plr-1);
		return 1;
	}
	return 0;
}

void SetDreadnaught(pPlayer pl)
{
	pl->crew=42;
	pl->maxcrew=42;
	pl->batt=42;
	pl->maxbatt=42;

	pl->maxspeed=30;

	pl->accel_inc=6;

	pl->firebatt=6;
	pl->specbatt=8;

	pl->offset=30;

	pl->batt_wait=4;
	pl->turn_wait=4;
	pl->thrust_wait=6;
	pl->weapon_wait=6;
	pl->special_wait=9;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=200;

	pl->fireangle=45;

}

int FireDreadnaught(pPlayer pl)
{

	s16 b = nextWeapon(pl);

	if (b>=0)
	{
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=20;//range
	pl->weapon[b].damage=-6;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=16;
	pl->weapon[b].angle = pl->angle;

	s32 speed=20;
	pl->weapon[b].xspeed = ((speed * (s32)SIN[pl->angle])>>8)/SPEED_REDUCT;
	pl->weapon[b].yspeed = ((speed * (s32)COS[pl->angle])>>8)/SPEED_REDUCT;

	pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle])>>8)/3;
	pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle])>>8)/3;

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);

	play_sfx(&UrQuan_Fusion,pl->plr-1);
    return 1;
	}
	return 0;
}

int aiSpecialDreadnaught(pPlayer ai)
{
	if (ai->crew>8&&ai->fighters<4)
		return 1;
	else
		return 0;
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
		MoveOffscreen(&sprites[ur->sprite]);
		parent->fighters--;

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
			parent->fighters--;
			//mothership crew++;
			ModifyCrew(parent,1);
			MoveOffscreen(&sprites[ur->sprite]);
			play_sfx(&UrQuan_Recover,parent->plr-1);
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

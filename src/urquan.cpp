#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "gfx/urquan_out.h"  //urqan
#include "gfx/urquan_fire.h"
#include "gfx/urquan_fighters.h"
#include "gfx/urquan_fighters_fire.h"

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

#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "gfx/pkunk_out.h"
#include "gfx/pkunk_fire.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

void SetFury(pPlayer pl)
{
	pl->crew=8;
	pl->maxcrew=8;
	pl->batt=12;
	pl->maxbatt=12;

	pl->maxspeed=64;

	pl->accel_inc=16;

	pl->firebatt=1;
	pl->specbatt=2;

	pl->offset=25;

	pl->batt_wait=0;
	pl->turn_wait=0;
	pl->thrust_wait=0;
	pl->weapon_wait=0;
	pl->special_wait=16;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=12+o;

	pl->range=120;

	pl->fireangle=100;

}

void LoadFury(s16 OAMStart, s16 SpriteStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
    {
       	OAMData[loop] = pkunkData[loop-OAMStart];
       	OAMData[loop+512] = pkunk_outData[loop-OAMStart];
	}
    for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+1024] = pkunk_fireData[loop-OAMStart]; //loads some garbv
    }

}


int FireFuryA(pPlayer pl,s32 angle)
{
	//s16 n = nextFireSprite(pl);
	s16 b = nextWeapon(pl);

	if (b>=0)//&&n>=0)
	{
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=5;//range
	pl->weapon[b].damage=-1;
	pl->weapon[b].angle;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;
	pl->weapon[b].size=8;
	//this determines the middle of the ship which aint so good

	pl->weapon[b].xpos = pl->xpos;//+(s32)(15 * SIN[angle])>>8;
	pl->weapon[b].ypos = pl->ypos;//-(s32)(15 * COS[angle])>>8 ;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	s32 speed=24;
	pl->weapon[b].xspeed = (s32)(speed * SIN[angle])>>8;
	pl->weapon[b].yspeed = (s32)(speed * COS[angle])>>8;

	//pl->weapon[b].xpos+=pl->weapon[b].xspeed;
	//pl->weapon[b].ypos-=pl->weapon[b].yspeed;

	//pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle])>>8)/3;
	//pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle])>>8)/3;


	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,8);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
    return 1;
	}
	return 0;
}

int FireFury(pPlayer pl)
{
	int r1=0;
	int r2=0;
	int r3=0;
	r1=FireFuryA( pl,pl->angle);

	s32 angle=pl->angle-90;
	if (angle<0)
		angle+=360;
	r2=FireFuryA( pl,angle);
	angle=pl->angle+90;
	if (angle>360)
		angle-=360;
	r3=FireFuryA( pl,angle);

	return (r1||r2||r3);
}

int SpecialFury(pPlayer pl)
{
	int r = ran(0,9);
	if (r<3)
	{
		ModifyCrew(pl,4);
	}
	return 1;
}

int DeathFury(pPlayer pl)
{
	int r=ran(0,20);
	if (r>15)
	{
		//SetFury(pl);
		SetShip(pl);
		SetNew(pl);
		UpdateCrew(pl);
		UpdateBatt(pl);
		pl->warp=0;
		pl->xspeed=0;
		pl->yspeed=0;
		pl->reinc=3;
		return 1;
	}
	return 0;
}


int aiSpecialFury(pPlayer ai)
{
	if (ai->crew<3)
		return 1;
	else
		return 0;
}
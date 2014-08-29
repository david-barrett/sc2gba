//fire.cpp
//contains all of the firing data
#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;

int FireDreadnaught(pPlayer pl);
int FireFury(pPlayer pl);

void Fire(pPlayer pl)
{
	print("fire");
	int ret=0;
	if (pl->weapon_turn==0&&pl->batt>pl->firebatt)
	{

		switch(pl->ship)
		{
		case DREADNAUGHT:
			ret=FireDreadnaught(pl);
			break;
		case FURY:
			ret=FireFury(pl);
			break;
		default:
			break;
		}
		if (ret==1)
		{
			pl->batt-=pl->firebatt;
			pl->weapon_turn=pl->weapon_wait;
		}
	}
}

int nextWeapon(pPlayer pl)
{
	for (int i=0;i<15;i++)
	{
		if (pl->weapon[i].life==-1)
			return i;
	}
	return -1;
}

int nextFireSprite(pPlayer pl)
{
	for (int i=pl->ffiresprite;i<=pl->lfiresprite;i++)
	{
		if (sprites[i].attribute2==0)
			return i;
	}
	return -1;
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
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(0);
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

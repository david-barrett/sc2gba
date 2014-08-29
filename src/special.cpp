//special.cpp
//contains all of the special data
#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"

extern s32 screenx,screeny;
extern OAMEntry sprites[128];
extern double scale;
extern int turn;

int SpecialDreadnaught(pPlayer pl);
int SpecialFury(pPlayer pl);

extern int turn;

void Special(pPlayer pl)
{
	int ret=0;
	if (pl->special_turn==0&&pl->batt>pl->specbatt)
	{

		switch(pl->ship)
		{
		case DREADNAUGHT:
			ret=SpecialDreadnaught(pl);
			break;
		case FURY:
			ret=SpecialFury(pl);
			break;
		default:
			break;
		}
		if (ret==1)
		{
			pl->batt-=pl->specbatt;
			pl->special_turn=pl->special_wait;
		}
	}
}

int nextSpec(pPlayer pl)
{
	for (int i=0;i<15;i++)
	{
		if (pl->weapon[i].life==-1)
			return i;
	}
	return -1;
}

int nextSpecSprite(pPlayer pl)
{
	for (int i=pl->ffiresprite;i<=pl->lfiresprite;i++)
	{
		if (sprites[i].attribute2==0)
			return i;
	}
	return -1;
}

int SpecialDreadnaught(pPlayer pl)
{
	//s16 n = nextSpecSprite(pl);
		s16 b = nextSpec(pl);

		if (b>=0&&pl->crew>1)//&&n>=0)
		{
		pl->weapon[b].type=UR_FIGHTERS;
		pl->weapon[b].life=1;//range
		pl->weapon[b].damage=4;
	//	pl->weapon[b].target=pl->opp;
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

		pl->weapon[b].xpos = pl->xpos + 32 + ((s32)(32 * (s32)SIN[pl->angle])) - 8;
		pl->weapon[b].ypos = pl->ypos + 32 + ((s32)(32 * (s32)COS[pl->angle])) - 8;

		pl->weapon[b].xpos = pl->xpos;
		pl->weapon[b].ypos = pl->ypos;

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,8);

		s32 speed=0;
		pl->weapon[b].xspeed = 0;//(speed * (s32)SIN[pl->angle])>>8;
		pl->weapon[b].yspeed = 0;//(speed * (s32)COS[pl->angle])>>8;

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+40 | PRIORITY(1);
	    pl->crew--;
	    return 1;
		}
	return 0;
}

int SpecialFury(pPlayer pl)
{
	//int r = ran(10);
	if (turn == 5)
	{
		pl->crew+=4;
		if (pl->crew>pl->maxcrew)
			pl->crew=pl->maxcrew;
	}
	return 1;
}




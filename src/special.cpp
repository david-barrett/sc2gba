//special.cpp
//contains all of the special data
#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;


extern unsigned long state;

int SpecialDreadnaught(pPlayer pl);
int SpecialFury(pPlayer pl);

extern int turn;

int Special(pPlayer pl)
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
		if (ret>0)
		{
			ModifyBatt(pl,-1*pl->specbatt);
			pl->special_turn=pl->special_wait;
		}
	}
	return ret;
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
	int r=ran(0,10);
	if (r<6)
	{
		//SetFury(pl);
		SetShip(pl);
		SetNew(pl);
		UpdateCrew(pl);
		UpdateBatt(pl);
		return 1;
	}
	return 0;
}




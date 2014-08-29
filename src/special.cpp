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


int Special(pPlayer pl)
{
	int ret=0;
	if (pl->special_turn==0&&pl->batt>pl->specbatt)
	{
		ret=pl->specfunc(pl);
		/*
		switch(pl->ship)
		{
		case DREADNAUGHT:
			ret=SpecialDreadnaught(pl);
			break;
		case FURY:
			ret=SpecialFury(pl);
			break;
		case TERMINATOR:
				ret=SpecialYehat(pl);
				break;
		default:
			break;
		}
		*/
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

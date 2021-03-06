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

void Fire(pPlayer pl)
{
	print("fire");
	int ret=0;
	if (pl->weapon_turn==0&&pl->batt>=pl->firebatt)
	{
		ret=pl->firefunc(pl);
		/*
		switch(pl->ship)
		{
		case DREADNAUGHT:
		//	ret=FireDreadnaught(pl);

			break;
		case FURY:
			ret=FireFury(pl);
			break;
		case TERMINATOR:
			ret=FireYehat(pl);
			break;
		default:
			break;
		}
		*/
		if (ret>0)
		{
			ModifyBatt(pl,-1*pl->firebatt);
			pl->weapon_turn=pl->weapon_wait;
		}
	}
}

int nextWeapon(pPlayer pl,int f,int l)
{
	//defaults f=1 & l=11
	for (int i=f;i<l+1;i++)
	{
		if (pl->weapon[i].life==-1)
			return i;
	}
	return -1;
}
/*
int nextFireSprite(pPlayer pl)
{
	for (int i=pl->ffiresprite;i<=pl->lfiresprite;i++)
	{
		if (sprites[i].attribute2==0)
			return i;
	}
	return -1;
}
*/



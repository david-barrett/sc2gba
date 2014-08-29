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

void Special(pPlayer pl)
{
	int ret=0;
	if (can(pl->specialrate)&&pl->batt>pl->specialbatt)
	{

		switch(pl->ship)
		{
		case DREADNAUGHT:
			ret=FireDreadnaught(pl);
			break;
		case FURY:
			ret=SpecialFury(pl);
			break;
		default:
			break;
		}
		if (ret==1)
		{
			pl->batt-=pl->specialbatt;
		}
	}
}
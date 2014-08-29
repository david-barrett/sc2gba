#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

extern pOAMEntry sprites;

void SetNew(pPlayer pl)
{
	//set random loc && facing

	//
	pl->batt_turn=0;
	pl->turn_turn=0;
	pl->thrust_turn=0;
	pl->weapon_turn=0;
	pl->special_turn=0;

	pl->xspeed=0;
	pl->yspeed=0;

	for (int i=0;i<12;i++)
	{
		pl->weapon[i].life=-1;
		MoveOffscreen(&sprites[pl->weapon[i].sprite]);
	}

	pl->warp=6;
	pl->xspeed = ((20) * (s32)SIN[pl->angle])>>8;
	pl->yspeed = ((20) * (s32)COS[pl->angle])>>8;

	pl->aiturn=0;
	pl->aispecial=0;

	pl->reinc=0;//pk
	pl->cloak=0;//ilwrath
	pl->shield=0;//yehat
	pl->scrambled=0;//

	pl->fighters=0;//urquan

	pl->spriteoffset=0;
	pl->angleoffset=0;

	GenerateStart(pl);
}




void SetShip(pPlayer pl)
{
	switch(pl->ship)
	{
		case DREADNAUGHT:
			SetDreadnaught(pl);
			break;
		case FURY:
			SetFury(pl);
			break;
	}
}

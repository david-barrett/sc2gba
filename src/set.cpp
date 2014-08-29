#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

extern pOAMEntry sprites;

void SetNew(pPlayer pl)
{
	//set random loc && facing
	print("in set new");
	//
	pl->batt_turn=0;
	pl->turn_turn=0;
	pl->thrust_turn=0;
	pl->weapon_turn=0;
	pl->special_turn=0;

	//pl->xspeed=0;
	//pl->yspeed=0;

	pl->thrustspeed=0;

	for (int i=0;i<12;i++)
	{
		pl->weapon[i].life=-1;
		MoveOffscreen(&sprites[pl->weapon[i].sprite]);
	}
	print("A");

	pl->aiturn=0;
	pl->aispecial=0;

	pl->reinc=0;//pk
	pl->cloak=0;//ilwrath
	pl->shield=0;//yehat
	pl->scrambled=0;//

	pl->fighters=0;//urquan

	pl->spriteoffset=0;
	pl->angleoffset=0;

	pl->ManeuverabilityIndex=0;

	print("gen start");
	GenerateStart(pl);
	pl->warp=6;
	pl->xspeed = ((20) * (s32)SIN[pl->angle])>>8;
	pl->yspeed = ((20) * (s32)COS[pl->angle])>>8;
	print("end set new");

	pl->EndGame=20;
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
		case TERMINATOR:
			SetYehat(pl);
			break;
		case AVENGER:
			SetIlwrath(pl);
			break;
	}
}

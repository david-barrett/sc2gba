#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

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

	for (int i=0;i<15;i++)
		pl->weapon[i].life=-1;

	pl->warp=6;
	pl->xspeed = ((20) * (s32)SIN[pl->angle])>>8;
	pl->yspeed = ((20) * (s32)COS[pl->angle])>>8;

	pl->aiturn=0;
	pl->aispecial=0;
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
}

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

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

	//pl->xspeed=0;
	//pl->yspeed=0;

	pl->thrustspeed=0;

	for (int i=0;i<12;i++)
	{
		pl->weapon[i].life=-1;
		MoveOffscreen(&sprites[pl->weapon[i].sprite]);
	}


	pl->aiturn=0;
	pl->aispecial=0;

	pl->reinc=0;//pk
	pl->cloak=0;//ilwrath
	pl->shield=0;//yehat
	pl->scrambled=0;//
	pl->destruct=0;



	pl->spriteoffset=0;
	pl->angleoffset=0;

	pl->limpets=0;
	pl->blaze=0;

	pl->ManeuverabilityIndex=0;



	GenerateStart(pl);
	pl->warp=6;
	//pl->xspeed = ((20) * (s32)SIN[pl->angle])>>8;
	//pl->yspeed = ((20) * (s32)COS[pl->angle])>>8;


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
		case INTRUDER:
			SetVux(pl);
			break;

		case GUARDIAN:
			SetAndrosynth(pl);
			break;
		case SKIFF :
			SetArilou(pl);
			break;
		case MARAUDER:
			SetBlackurq(pl);
			break;
		case BROODHOME:
			SetChenjesu(pl);
			break;
		case AVATAR:
		//	SetChmmr(pl);
			break;

		case BLADE:
		//	SetSupox(pl);
			break;
		case CRUISER:
			SetHuman(pl);
			break;
		case DRONE:
			SetUmgah(pl);
			break;
		case ELUDER:
			SetSpathi(pl);
			break;
		case JUGGER:
			SetUtwig(pl);
			break;

		case MAULER:
			SetDruuge(pl);
			break;
		case NEMESIS:
		//	SetOrz(pl);
			break;
		case PENETRATOR:
			SetSyreen(pl);
			break;
		case PODSHIP:
			SetMycon(pl);
			break;
		case PROBE:
			SetProbe(pl);
			break;

		case SCOUT:
			SetShofixti(pl);
			break;
		case STINGER:
			SetZoqFot(pl);
			break;
		case TORCH:
			SetThradd(pl);
			break;
		case TRADER:
			//SetMelnorme(pl);
			break;
		case TRANSFORMER:
			SetMmrnmhrm(pl);
			break;


	}
}

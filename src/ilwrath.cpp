#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "gfx/ilwrath_out.h"
//#include "gfx/Ilwrath_fire.h"
#include "gfx/ilwrath_cloak.h"

#include "ilwrath_sfx.h"

#include "ilwrathpilot.h"
#include "ilwrathpilotl.h"
#include "ilwrathpilotr.h"
#include "ilwrathpilott.h"
#include "ilwrathpilotf.h"
#include "ilwrathpilots.h"


extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireIlwrath(pPlayer pl);
int SpecialIlwrath(pPlayer pl);
int aiIlwrath(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetIlwrathPilot(pPlayer p);

void LoadIlwrath(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = ilwrathData[loop-OAMStart];
       		OAMData[loop+512] = ilwrath_outData[loop-OAMStart];
       		OAMData[loop+1024] = ilwrath_cloakData[loop-OAMStart];

   	}

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
   	{
		OAMData[loop+1024+512] = ilwrathpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+512] = ilwrathpilotlData[loop-OAMStart];
		OAMData[loop+2816] = ilwrathpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+2816+256] = ilwrathpilottData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+32;loop++)
		OAMData[loop+2816+256+128] = ilwrathpilotfData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
		OAMData[loop+2816+256+128+32] = ilwrathpilotsData[loop-OAMStart];

   	/*
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512+512] = ilwrath_fireData[loop-OAMStart]; //loads some garb
   	}
	*/
}

int SpecialIlwrath(pPlayer pl)
{
	if (pl->cloak==0)
	{
		pl->cloak=1;
		pl->spriteoffset=64;
		play_sfx(&ilwrath_cloak,pl->plr-1);
	}
	else
	{
		pl->cloak=0;
		pl->spriteoffset=0;
		play_sfx(&ilwrath_uncloak,pl->plr-1);
	}
	sprites[(pl->plr==1)?0:13].attribute2 = pl->SpriteStart+pl->spriteoffset | PRIORITY(1);

	return 1;
}

void SetIlwrath(pPlayer pl)
{
	pl->crew=22;
	pl->maxcrew=22;
	pl->batt=16;
	pl->maxbatt=16;

	pl->maxspeed=25;

	pl->accel_inc=5;

	pl->firebatt=1;
	pl->specbatt=3;

	pl->offset=25;

	pl->batt_wait=4;
	pl->turn_wait=2;
	pl->thrust_wait=0;
	pl->weapon_wait=0;
	pl->special_wait=13;
	pl->batt_regen=4;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=50;

	pl->fireangle=45;

	pl->firefunc=&FireIlwrath;
	pl->specfunc=&SpecialIlwrath;
	pl->aifunc=&aiIlwrath;
	pl->loadfunc=&LoadIlwrath;
	pl->loadpilots=&SetIlwrathPilot;

	pl->ditty=&ilwrath_ditty;

	pl->ship_flags = FIRES_FORE ;

	pl->pilot_sprite=(1024+512)/16;

	pl->pilots[0].x=4;
	pl->pilots[0].y=15;
	pl->pilots[1].x=4;
	pl->pilots[1].y=15;
	pl->pilots[2].x=6;
	pl->pilots[2].y=1;
	pl->pilots[3].x=25;
	pl->pilots[3].y=23;
	pl->pilots[4].x=27;
	pl->pilots[4].y=15;

}

int FireIlwrath(pPlayer pl)
{


	s16 b;
	int ret=0;
	pPlayer opp=(pPlayer)pl->opp;

	b= nextWeapon(pl);

	if (b>=0)
	{
		if (pl->cloak)
		{
			pl->cloak=0;
			pl->spriteoffset=0;
			sprites[(pl->plr==1)?0:13].attribute2 = pl->SpriteStart+pl->spriteoffset | PRIORITY(1);
			pl->angle=FindAngle(pl->xpos,pl->ypos,opp->xpos,opp->ypos);

		}


	pl->weapon[b].type=ILWRATHFIRE;
	pl->weapon[b].life=5;//range
	pl->weapon[b].damage=-1;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = pl->angle;

	s32 speed=25;
	pl->weapon[b].xspeed = ((speed * (s32)SIN[pl->angle])>>9);///SPEED_REDUCT;
	pl->weapon[b].yspeed = ((speed * (s32)COS[pl->angle])>>9);///SPEED_REDUCT;

	//pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle+(i==0?-30:+30)])>>8)/3;
	//pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle+(i==0?-30:+30)])>>8)/3;

	pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle])>>8)/3;
	pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle])>>8)/3;


	pl->weapon[b].size=8;
	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = FireSprite1 | PRIORITY(1);
    drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

    ret++;
    play_sfx(&ilwrath_flame,pl->plr-1);
    }

	return ret;

}

int aiIlwrath(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{

	pObject lpEvalDesc;
	//STARSHIPPTR StarShipPtr;

		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		lpEvalDesc->MoveState = PURSUE;
		if (lpEvalDesc->parent && lpEvalDesc->which_turn <= 10)
					/* don't want to dodge when you could be flaming */
			ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent = 0;

		ship_intelligence(ai,ObjectsOfConcern, ConcernCounter);

		//GetElementStarShip (ShipPtr, &StarShipPtr);
		if (lpEvalDesc->parent
				&& (lpEvalDesc->which_turn <= 6
				|| (lpEvalDesc->which_turn <= 10
				&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn <= 10)))
		{
			print("\nil fire now");
			print("which turn ship");
			print(lpEvalDesc->which_turn);
			print("which turn weapon");
			print(ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn);
			ai->ship_input_state &= ~SPECIAL;
			if (ai->cloak==1)
			{
				ai->ship_input_state &= ~LEFT | RIGHT;
				ai->ship_input_state |= THRUST;
			}
			ai->ship_input_state |= WEAPON;
		}
		else if (ai->special_turn == 0&& ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn >= 10)/*if (StarShipPtr->special_counter == 0
				&& (LOBYTE (GLOBAL (CurrentActivity)) != IN_ENCOUNTER
				|| !GET_GAME_STATE (PROBE_ILWRATH_ENCOUNTER)))*/
		{
			ai->ship_input_state &= ~SPECIAL;
			if (!ai->cloak==1
					&& !(ai->ship_input_state & WEAPON))
				ai->ship_input_state |= SPECIAL;
	}
}

void SetIlwrathPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64+16 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[45+off].attribute1 = SIZE_16 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64+16+16 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[46+off].attribute1 = SIZE_8 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64+32+8 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64+40+2 | PRIORITY(2);
}
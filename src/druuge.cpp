#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "druuge_out.h"
#include "druuge_fire.h"


#include "druuge_sfx.h"

#include "druugepilot.h"

#include "druugepilotl.h"
#include "druugepilotr.h"
#include "druugepilott.h"
#include "druugepilotf.h"
#include "druugepilots.h"


extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireDruuge(pPlayer pl);
int SpecialDruuge(pPlayer pl);
int aiDruuge(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetDruugePilot(pPlayer p);
void RestoreGFXDruuge(pPlayer p);
void MoveDogi(pWeapon ur);
void PostDruuge(pPlayer p);

extern s32 zoom;


void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#define MAX_CREW 14
#define MAX_ENERGY 32
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 4
#define SPECIAL_ENERGY_COST 0//16
#define ENERGY_WAIT 50
#define MAX_THRUST  SHIP_SPEED(20)
#define THRUST_INCREMENT  SHIP_SPEED(2)
#define TURN_WAIT 4
#define THRUST_WAIT 1
#define WEAPON_WAIT 10
#define SPECIAL_WAIT 30

#define SHIP_MASS 5
#define MISSILE_SPEED 30 //DISPLAY_TO_WORLD (30)
#define MISSILE_LIFE 20
#define MISSILE_RANGE (MISSILE_SPEED * MISSILE_LIFE)
#define MISSILE_DAMAGE 6

#define RECOIL_VELOCITY 6 //WORLD_TO_VELOCITY (DISPLAY_TO_WORLD (6))
#define MAX_RECOIL_VELOCITY (RECOIL_VELOCITY * 4)

void LoadDruuge(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = druugeData[loop-OAMStart];
       		OAMData[loop+512] = druuge_outData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+1024] = druuge_fireData[loop-OAMStart]; //loads some garb
   	}

   	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+32] = druugepilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+64;loop++)
	{
		OAMData[loop+(1024*2)+32] = druugepilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+32] = druugepilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)

		OAMData[loop+(1024*2)+128+32] = druugepilottData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
		OAMData[loop+(1024*2)+128+256+32] = druugepilotfData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
		OAMData[loop+(1024*2)+128+256+128+32] = druugepilotsData[loop-OAMStart];



}

int FireDruuge(pPlayer pl)
{
	s16 b= nextWeapon(pl);
	s8 ret=0;
	if (b>=0)
	{

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=MISSILE_LIFE;
	pl->weapon[b].damage=-1*MISSILE_DAMAGE; //guess
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = 0;
	pl->weapon[b].actualangle = 0;

	pl->weapon[b].xspeed=((s32)(MISSILE_SPEED * SIN[pl->angle])>>8);
	pl->weapon[b].yspeed=((s32)(MISSILE_SPEED * COS[pl->angle])>>8);;


	pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset * SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset * COS[pl->angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);

	play_sfx(&druuge_cannon,pl->plr-1);

	pl->xspeed-=((s32)(RECOIL_VELOCITY * SIN[pl->angle])>>8);
	pl->yspeed-=((s32)(RECOIL_VELOCITY * COS[pl->angle])>>8);;
	ret=1;
	}




	return ret;
}

void SetDruuge(pPlayer pl)
{
	pl->crew=MAX_CREW;
		pl->maxcrew=MAX_CREW;
		pl->batt=MAX_ENERGY;
		pl->maxbatt=MAX_ENERGY;

		pl->maxspeed=MAX_THRUST;

		pl->accel_inc=THRUST_INCREMENT;

		pl->firebatt=WEAPON_ENERGY_COST;
		pl->specbatt=SPECIAL_ENERGY_COST;

		pl->offset=14;

		pl->batt_wait=ENERGY_WAIT;
		pl->turn_wait=TURN_WAIT;
		pl->thrust_wait=THRUST_WAIT;
		pl->weapon_wait=WEAPON_WAIT;
		pl->special_wait=SPECIAL_WAIT;
		pl->batt_regen=ENERGY_REGENERATION;

		s16 o = (pl->plr-1)*13;

		pl->ffiresprite=1+o;
		pl->lfiresprite=4+o;

		pl->fspecsprite=5+o;
		pl->lspecsprite=12+o;

		pl->range=1440;

	pl->fireangle=45;

	pl->firefunc=&FireDruuge;
	pl->specfunc=&SpecialDruuge;
	pl->aifunc=&aiDruuge;
	pl->loadfunc=&LoadDruuge;
	pl->loadpilots=&SetDruugePilot;
	pl->postfunc=&PostDruuge;
	pl->restorefunc=&RestoreGFXDruuge;

	pl->ditty=&druuge_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+32)/16;
	pl->pilots[0].x=30;
	pl->pilots[0].y=16;
	pl->pilots[1].x=40;
	pl->pilots[1].y=15;
	pl->pilots[2].x=46;
	pl->pilots[2].y=1;
	pl->pilots[3].x=32;
	pl->pilots[3].y=1;
	pl->pilots[4].x=3;
	pl->pilots[4].y=0;


}

int SpecialDruuge(pPlayer pl)
{
	if (pl->crew>1&&pl->crew<MAX_CREW)
	{
		ModifyCrew(pl,-1);
		play_sfx(&druuge_furnace,pl->plr-1);
		//pl->batt+=2*SPECIAL_ENERGY_COST;
		ModifyBatt(pl,16);
	}



}

int aiDruuge(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	s8 ship_flags = 0;
		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		if ((ai->speed>=ai->maxspeed))
			lpEvalDesc->MoveState = ENTICE;
		else if (lpEvalDesc->parent
				&& lpEvalDesc->which_turn <= /*WORLD_TO_TURN*/ (MISSILE_RANGE * 3 / 4))
		{

			ship_flags = opp->ship_flags;
			opp->ship_flags &=
					~(FIRES_FORE | FIRES_RIGHT | FIRES_AFT | FIRES_LEFT);

			lpEvalDesc->MoveState = PURSUE;
			if (ai->thrust_turn == 0)
				++ai->thrust_turn;
		}
		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);
		if (ship_flags)
		{
			opp->ship_flags = ship_flags;
		}

		if (!((ai->speed>=ai->maxspeed))
				&& (lpEvalDesc->which_turn <= 12
				|| (
				ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent
				&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn <= 6
				)))
		{
			 ai->ship_input_state |= WEAPON;
			 if (ai->thrust_turn < WEAPON_WAIT + 1)
				ai->thrust_turn = WEAPON_WAIT + 1;
		}


		if ((ai->ship_input_state & WEAPON)
				&& ai->batt < WEAPON_ENERGY_COST
				&& ai->crew > 1)
			ai->ship_input_state |= SPECIAL;
		else
		ai->ship_input_state &= ~SPECIAL;

}

void SetDruugePilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[43+off].attribute1 = SIZE_8 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[44+off].attribute1 = SIZE_8 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+68 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+72 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |SQUARE  | 160;
	sprites[46+off].attribute1 = SIZE_16 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+88 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);
}

void RestoreGFXDruuge(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}
		}
	}
}

void PostDruuge(pPlayer p)
{
	s16 a=p->angle-45;
	if (a<0)
		a+=360;
	RotateSprite(p->plr==1?0:13, a, zoom, zoom);

	if (p->warp>1)
	{
		for (int i=0;i<12;i++)
		{
			if (p->weapon[i].type==TRAIL)
			{
				RotateSprite(p->weapon[i].sprite, a, zoom, zoom);
				p->weapon[i].angle= a;
			}
		}
	}

}

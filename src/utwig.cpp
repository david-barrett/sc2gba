#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "utwig_out.h"
#include "utwig_fire.h"
#include "utwig_shield.h"

#include "utwig_sfx.h"

#include "utwigpilot.h"
/*
#include "utwigpilotl.h"
#include "utwigpilotr.h"
#include "utwigpilott.h"
#include "utwigpilotf.h"
#include "utwigpilots.h"
*/

#define MAX_CREW 20
#define MAX_ENERGY 20
#define ENERGY_REGENERATION 0
#define WEAPON_ENERGY_COST 0
#define SPECIAL_ENERGY_COST 1
#define ENERGY_WAIT 255
#define MAX_THRUST SHIP_SPEED(36)
#define THRUST_INCREMENT SHIP_SPEED(6)
#define TURN_WAIT 1
#define THRUST_WAIT 6
#define WEAPON_WAIT 7
#define SPECIAL_WAIT 12

#define SHIP_MASS 8
#define UTWIG_OFFSET 9
#define MISSILE_SPEED 30 //DISPLAY_TO_WORLD (30)
#define MISSILE_LIFE 10

#define MISSILE_DAMAGE 1



extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireUtwig(pPlayer pl);
int SpecialUtwig(pPlayer pl);
int aiUtwig(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetUtwigPilot(pPlayer p);
void PostUtwig(pPlayer p);
void RestoreGFXUtwig(pPlayer p);

void LoadUtwig(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = utwigData[loop-OAMStart];
       		OAMData[loop+512] = utwig_outData[loop-OAMStart];
       		OAMData[loop+1024] = utwig_shieldData[loop-OAMStart];
       		OAMData[loop+1024+512] = utwig_fireData[loop-OAMStart];

   	}

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+2048+32] = utwigpilotData[loop-OAMStart];
	//		OAMData[loop+(1024*2)+512+32] = utwigpilotlData[loop-OAMStart];
	//		OAMData[loop+(1024*3)+512+32] = utwigpilotrData[loop-OAMStart];
	}
/*
	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
	//	OAMData[loop+(1024*4)+512+32] = utwigpilottData[loop-OAMStart];
	//	OAMData[loop+(1024*4)+512+32+128] = utwigpilotfData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+64;loop++)
	//	OAMData[loop+(1024*4)+512+32+256] = utwigpilotsData[loop-OAMStart];

*/
}

int SpecialUtwig(pPlayer pl)
{
	pl->shield=6;
	sprites[(pl->plr==1)?0:13].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
	play_sfx(&utwig_shield,pl->plr-1);
	return 1;
}

void SetUtwig(pPlayer pl)
{
	pl->crew=MAX_CREW;
		pl->maxcrew=MAX_CREW;
		pl->batt=MAX_ENERGY;
		pl->maxbatt=MAX_ENERGY;

		pl->maxspeed=MAX_THRUST;

		pl->accel_inc=THRUST_INCREMENT;

		pl->firebatt=WEAPON_ENERGY_COST;
		pl->specbatt=SPECIAL_ENERGY_COST;



		pl->batt_wait=ENERGY_WAIT;
		pl->turn_wait=TURN_WAIT;
		pl->thrust_wait=THRUST_WAIT;
		pl->weapon_wait=WEAPON_WAIT;
		pl->special_wait=SPECIAL_WAIT;
		pl->batt_regen=ENERGY_REGENERATION;

	pl->mass=SHIP_MASS;

	pl->offset=16;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=200;

	pl->fireangle=45;

	pl->firefunc=&FireUtwig;
	pl->specfunc=&SpecialUtwig;
	pl->aifunc=&aiUtwig;
	pl->loadfunc=&LoadUtwig;
	pl->loadpilots=&SetUtwigPilot;
	pl->postfunc=&PostUtwig;
	pl->restorefunc=&RestoreGFXUtwig;

	pl->ditty=&utwig_ditty;

	pl->ship_flags = FIRES_FORE | POINT_DEFENSE | SHIELD_DEFENSE;

	pl->pilot_sprite=(2048)/16;
	pl->pilots[0].x=240;
	pl->pilots[0].y=160;
	pl->pilots[1].x=240;
	pl->pilots[1].y=160;
	pl->pilots[2].x=240;
	pl->pilots[2].y=160;
	pl->pilots[3].x=240;
	pl->pilots[3].y=160;
	pl->pilots[4].x=240;
	pl->pilots[4].y=160;


}

int FireUtwig(pPlayer pl)
{

	s16 b;
	int ret=0;


	b= nextWeapon(pl);
	if (b>=0)
	{
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=MISSILE_LIFE;//range
	pl->weapon[b].damage=-1*MISSILE_DAMAGE;//6 suspect 6 was for both guns;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=32;
	pl->weapon[b].angle = pl->angle;

	s32 speed=20;
	pl->weapon[b].xspeed = ((MISSILE_SPEED * (s32)SIN[pl->angle])>>8);///SPEED_REDUCT;
	pl->weapon[b].yspeed = ((MISSILE_SPEED * (s32)COS[pl->angle])>>8);///SPEED_REDUCT;

	//pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle+(i==0?-30:+30)])>>8)/3;
	//pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle+(i==0?-30:+30)])>>8)/3;

	pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle])>>8)/3;
	pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle])>>8)/3;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_32| ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);

    ret=1;
	play_sfx(&utwig_fire,pl->plr-1);
	}
	return ret;
}

int aiUtwig(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	s16 ShieldStatus;

		pObject lpEvalDesc;

	pPlayer opp = (pPlayer)ai->opp;




		lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
		if (ai->batt == 0)
			ShieldStatus = 0;
		else
		{
			ShieldStatus = -1;
			if (lpEvalDesc->parent && lpEvalDesc->MoveState == ENTICE)
			{
				ShieldStatus = 0;
				//if (!(lpEvalDesc->ObjectPtr->state_flags & FINITE_LIFE))
				//	lpEvalDesc->MoveState = PURSUE;
				//else
				if
				//(lpEvalDesc->ObjectPtr->mass_points ||
						 (lpEvalDesc->type== CREW) //)
				{
					if ((lpEvalDesc->which_turn >>= 1) == 0)
						lpEvalDesc->which_turn = 1;

					//if (lpEvalDesc->ObjectPtr->mass_points)
					//	lpEvalDesc->ObjectPtr = 0;
				//	else
						lpEvalDesc->MoveState = PURSUE;
					ShieldStatus = 1;
				}
			}
		}

		if (ai->special_turn == 0)
		{
			ai->ship_input_state &= ~SPECIAL;
			if (ShieldStatus)
			{
				if ((ShieldStatus > 0 || lpEvalDesc->parent)
						&& lpEvalDesc->which_turn <= 2
						&& (ShieldStatus > 0
						|| (opp->ship_flags & IMMEDIATE_WEAPON) /* means IMMEDIATE WEAPON */
						|| PlotIntercept (ai,lpEvalDesc, 2, 0))
						&& (ran(0,3)))
				{
					ai->ship_input_state |= SPECIAL;
					ai->ship_input_state &= ~WEAPON;
				}

				lpEvalDesc->parent = 0;
			}
		}

		if (ai->batt
				&& (lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX])->parent)
		{


				if (!(opp->ship_flags	& IMMEDIATE_WEAPON))
					lpEvalDesc->MoveState = PURSUE;
		}
	ship_intelligence(ai,ObjectsOfConcern, ConcernCounter);

}

void SetUtwigPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 = SIZE_64 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[44+off].attribute1 = SIZE_64 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[45+off].attribute1 = SIZE_16 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+64 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[46+off].attribute1 = SIZE_16 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+64+8 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 = SIZE_8 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+64+16 | PRIORITY(2);
}

void PostUtwig(pPlayer pl)
{
	if (pl->shield>0)
	{
		pl->shield--;
		if (pl->shield==0)
		{
			sprites[(pl->plr==1)?0:13].attribute2 = pl->SpriteStart | PRIORITY(1);
		}
	}
}

void RestoreGFXUtwig(pPlayer p)
{
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_32 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+96 | PRIORITY(1);
			}
		}
	}
}

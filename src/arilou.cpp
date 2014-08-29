#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "arilou_out.h"
#include "arilou_laser.h"



#include "arilou_sfx.h"

#include "ariloupilot.h"
/*
#include "ariloupilotl.h"
#include "ariloupilotr.h"
#include "ariloupilott.h"
#include "ariloupilotf.h"
#include "ariloupilots.h"
*/

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireArilou(pPlayer pl);
int SpecialArilou(pPlayer pl);
int aiArilou(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetArilouPilot(pPlayer p);
void RestoreGFXArilou(pPlayer p);
void MoveDogi(pWeapon ur);
void PostArilou(pPlayer p);

#define MAX_CREW 6
#define MAX_ENERGY 20
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 2
#define SPECIAL_ENERGY_COST 3
#define ENERGY_WAIT 6
#define MAX_THRUST /* DISPLAY_TO_WORLD (10) */ SHIP_SPEED(40)
#define THRUST_INCREMENT MAX_THRUST
#define TURN_WAIT 0
#define THRUST_WAIT 0
#define WEAPON_WAIT 1
#define SPECIAL_WAIT 2

#define SHIP_MASS 1
#define ARILOU_OFFSET 9
#define LASER_RANGE 90// /*DISPLAY_TO_WORLD*/ (100 + ARILOU_OFFSET)



void LoadArilou(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = arilouData[loop-OAMStart];
       		OAMData[loop+512] = arilou_outData[loop-OAMStart];
       		OAMData[loop+1024] = arilou_laserData[loop-OAMStart];
   	}
	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+512] = ariloupilotData[loop-OAMStart];
	}
/*
	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+64] = ariloupilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512] = ariloupilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+64] = ariloupilottData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+256] = ariloupilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+512] = ariloupilotsData[loop-OAMStart];

	}
*/
}

int FireArilou(pPlayer pl)
{
	pPlayer opp=(pPlayer)pl->opp;
	play_sfx(&arilou_fire,pl->plr-1);

	for (int b=0;b<2;b++)
	{


			pl->weapon[b].type=LASER;
			pl->weapon[b].life=1;
			pl->weapon[b].damage=-2;//GUESS
			pl->weapon[b].target=pl->opp;
			pl->weapon[b].parent=pl;
			pl->weapon[b].damageparent=0;

			pl->weapon[b].size=32;//(b==3?8:32);
			pl->weapon[b].angle = FindAngle(pl->xpos,pl->ypos,opp->xpos,opp->ypos);

			s32 off=(b==0?13:45);

			pl->weapon[b].xspeed=0;
			pl->weapon[b].yspeed=0;


			pl->weapon[b].xpos = pl->xpos+((off * (s32)SIN[pl->weapon[b].angle])>>8);
			pl->weapon[b].ypos = pl->ypos-((off * (s32)COS[pl->weapon[b].angle])>>8);

			drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
				pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

			sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
			sprites[pl->weapon[b].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
			sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
		}
	return 1;
}



void SetArilou(pPlayer pl)
{
	pl->crew=MAX_CREW;
		pl->maxcrew=MAX_CREW;
		pl->batt=MAX_ENERGY;
		pl->maxbatt=MAX_ENERGY;

		pl->maxspeed=MAX_THRUST;

		pl->accel_inc=THRUST_INCREMENT;

		pl->firebatt=WEAPON_ENERGY_COST;
		pl->specbatt=SPECIAL_ENERGY_COST;

		pl->offset=9;

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

		pl->range=100;

	pl->fireangle=45;

	pl->firefunc=&FireArilou;
	pl->specfunc=&SpecialArilou;
	pl->aifunc=&aiArilou;
	pl->loadfunc=&LoadArilou;
	pl->loadpilots=&SetArilouPilot;
	pl->postfunc=&PostArilou;
	pl->restorefunc=&RestoreGFXArilou;

	pl->ditty=&arilou_ditty;

	pl->ship_flags = IMMEDIATE_WEAPON;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+512)/16;
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

	pl->charging=0;
	for (int i=8;i<12;i++)
		pl->weapon[i].life=-2;

}

int SpecialArilou(pPlayer pl)
{

	play_sfx(&arilou_warp,pl->plr-1);
	GenerateStart(pl);

	return 1;


}

int aiArilou(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	 ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState = ENTICE;
		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

		if (ai->special_turn == 0)
		{


			ai->ship_input_state &= ~SPECIAL;

			lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
			if (lpEvalDesc->parent && lpEvalDesc->which_turn <= 6)
			{
				s8 IsTrackingWeapon;



				/*if (((opp->ship_flags	& SEEKING_WEAPON)
						//&& lpEvalDesc->ObjectPtr->next.image.farray ==
						//EnemyStarShipPtr->RaceDescPtr->ship_data.weapon
						) ||
						((opp->ship_flags & SEEKING_SPECIAL)
						// &&	lpEvalDesc->ObjectPtr->next.image.farray ==
						//EnemyStarShipPtr->RaceDescPtr->ship_data.special))
						})*/
				if ((opp->ship_flags	& SEEKING_WEAPON)&&(opp->ship_flags & SEEKING_SPECIAL))
					IsTrackingWeapon = 1;
				else
					IsTrackingWeapon = 0;

				if ((//(opp->ship_flags & IMMEDIATE_WEAPON)|| /* means IMMEDIATE WEAPON */
						 (IsTrackingWeapon && (lpEvalDesc->which_turn == 1
						|| (lpEvalDesc->type != CREW))) /* FIGHTERS!!! */
						|| PlotIntercept (ai,lpEvalDesc,  3, 0))
						&& !(ran(0,3)))
				{
					ai->ship_input_state &= ~(LEFT | RIGHT | THRUST | WEAPON);
					ai->ship_input_state |= SPECIAL;
				}
			}
		}
		if (ai->batt <= SPECIAL_ENERGY_COST << 1)
			ai->ship_input_state &= ~WEAPON;
}

void SetArilouPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |WIDE  | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+16 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 = SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+32 | PRIORITY(2);
}

void RestoreGFXArilou(pPlayer p)
{


}

void PostArilou(pPlayer p)
{
	//update then remove inertia
	p->xpos+=p->xspeed;
	p->ypos-=p->yspeed;

	p->xspeed=0;
	p->yspeed=0;

}

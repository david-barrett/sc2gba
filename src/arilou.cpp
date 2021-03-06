#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "arilou_out.h"
#include "arilou_laser.h"



#include "arilou_sfx.h"

#include "ariloupilot.h"
#include "ariloupilotl.h"
#include "ariloupilotr.h"
#include "ariloupilott.h"
#include "ariloupilotf.h"
#include "ariloupilots.h"

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
#define LASER_RANGE 100// /*DISPLAY_TO_WORLD*/ (100 + ARILOU_OFFSET)



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

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+512] = ariloupilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+256+512] = ariloupilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*3)] = ariloupilottData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
	
		OAMData[loop+(1024*3)+128] = ariloupilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+128+256] = ariloupilotsData[loop-OAMStart];

	}

}

int FireArilou(pPlayer pl)
{
	pPlayer opp=(pPlayer)pl->opp;
	play_sfx(&arilou_fire,pl->plr-1);

	for (int b=0;b<4;b++)
	{


			pl->weapon[b].type=LASER;
			pl->weapon[b].object.life=2;
			pl->weapon[b].damage=-1;
			pl->weapon[b].target=pl->opp;
			pl->weapon[b].parent=pl;
			pl->weapon[b].damageparent=0;			
			pl->weapon[b].movefunc=0;			
			pl->weapon[b].hitfunc=0;
			pl->weapon[b].object.ignorecollision=0;

			pl->weapon[b].object.size=32;//(b==3?8:32);
			pl->weapon[b].object.angle = FindAngle(pl->object.xpos,pl->object.ypos,opp->object.xpos,opp->object.ypos);

			s32 off=(b==0?13:13+(b*32));

			pl->weapon[b].object.xspeed=0;
			pl->weapon[b].object.yspeed=0;


			pl->weapon[b].object.xpos = pl->object.xpos+((off * (s32)SIN[pl->weapon[b].object.angle])>>8);
			pl->weapon[b].object.ypos = pl->object.ypos-((off * (s32)COS[pl->weapon[b].object.angle])>>8);

			drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
				pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,pl->weapon[b].object.size);

			sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
			sprites[pl->weapon[b].sprite].attribute1 =SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
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

		pl->range=LASER_RANGE;

	pl->firefunc=&FireArilou;
	pl->specfunc=&SpecialArilou;
	pl->aifunc=&aiArilou;
	pl->loadfunc=&LoadArilou;
	pl->loadpilots=&SetArilouPilot;
	pl->postfunc=&PostArilou;
	pl->restorefunc=&RestoreGFXArilou;

	pl->ditty=&arilou_ditty;

	pl->ship_flags = IMMEDIATE_WEAPON;
	pl->object.mass_points=SHIP_MASS;

	pl->pilot_sprite=(1024+512)/16;
	pl->pilots[0].x=15;
	pl->pilots[0].y=14;
	pl->pilots[1].x=16;
	pl->pilots[1].y=16;
	pl->pilots[2].x=31;
	pl->pilots[2].y=7;
	pl->pilots[3].x=5;
	pl->pilots[3].y=7;
	pl->pilots[4].x=15;
	pl->pilots[4].y=0;

	pl->charging=0;
	for (int i=8;i<12;i++)
		pl->weapon[i].object.life=-2;

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
						//&& lpEvalDesc->tr->next.image.farray ==
						//EnemyStarShipPtr->RaceDescPtr->ship_data.weapon
						) ||
						((opp->ship_flags & SEEKING_SPECIAL)
						// &&	lpEvalDesc->tr->next.image.farray ==
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

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 =SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[44+off].attribute1 =SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[45+off].attribute1 =SIZE_16 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(1);

	sprites[46+off].attribute0 = COLOR_256 |TALL  | 160;
	sprites[46+off].attribute1 =SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+104 | PRIORITY(1);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 =SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+120 | PRIORITY(2);
}

void RestoreGFXArilou(pPlayer p)
{


}

void PostArilou(pPlayer p)
{
	//update then remove inertia
	p->object.xpos+=p->object.xspeed;
	p->object.ypos-=p->object.yspeed;

	p->object.xspeed=0;
	p->object.yspeed=0;

}

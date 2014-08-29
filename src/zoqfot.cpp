#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "zoqfot_out.h"
#include "zoqfot_fire.h"
#include "zoqfot_tongue1.h"
#include "zoqfot_tongue2.h"


#include "zoqfot_sfx.h"

#include "zoqfotpilot.h"
/*
#include "zoqfotpilotl.h"
#include "zoqfotpilotr.h"
#include "zoqfotpilott.h"
#include "zoqfotpilotf.h"
#include "zoqfotpilots.h"
*/

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireZoqfot(pPlayer pl);
int SpecialZoqfot(pPlayer pl);
int aiZoqfot(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetZoqfotPilot(pPlayer p);
void RestoreGFXZoqfot(pPlayer p);
void MoveDogi(pWeapon ur);
void PostZoqfot(pPlayer p);

extern s32 zoom;


void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#define MAX_CREW 10
#define MAX_ENERGY 10
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 1
#define SPECIAL_ENERGY_COST (MAX_ENERGY * 3 / 4)
#define ENERGY_WAIT 4
#define MAX_THRUST SHIP_SPEED(40)
#define THRUST_INCREMENT SHIP_SPEED(10)
#define TURN_WAIT 1
#define THRUST_WAIT 0
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 6

#define SHIP_MASS 5
#define MISSILE_SPEED 10//DISPLAY_TO_WORLD (10)
#define MISSILE_LIFE 10
#define MISSILE_RANGE (MISSILE_SPEED * MISSILE_LIFE)
#define MISSILE_DAMAGE 1

#define TONGUE_DAMAGE 12

void LoadZoqfot(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = zoqfotData[loop-OAMStart];
       		OAMData[loop+512] = zoqfot_outData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+256; loop++)               //load sprite image data
   	{
		OAMData[loop+1024] = zoqfot_tongue1Data[loop-OAMStart]; //loads some garb
		OAMData[loop+1024+256] = zoqfot_tongue2Data[loop-OAMStart]; //loads some garb
	}


   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+1024+512] = zoqfot_fireData[loop-OAMStart]; //loads some garb
   	}

   	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+512+32] = zoqfotpilotData[loop-OAMStart];
	}
/*
	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+64] = zoqfotpilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512] = zoqfotpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+64] = zoqfotpilottData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+256] = zoqfotpilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+512] = zoqfotpilotsData[loop-OAMStart];

	}
*/
}

int FireZoqfot(pPlayer pl)
{
	s16 b= nextWeapon(pl,1,11);
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
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);

	play_sfx(&zoqfot_bullet,pl->plr-1);

	ret=1;
	}




	return ret;
}

void SetZoqFot(pPlayer pl)
{
	pl->crew=MAX_CREW;
		pl->maxcrew=MAX_CREW;
		pl->batt=MAX_ENERGY;
		pl->maxbatt=MAX_ENERGY;

		pl->maxspeed=MAX_THRUST;

		pl->accel_inc=THRUST_INCREMENT;

		pl->firebatt=WEAPON_ENERGY_COST;
		pl->specbatt=SPECIAL_ENERGY_COST;

		pl->offset=12;

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

	pl->firefunc=&FireZoqfot;
	pl->specfunc=&SpecialZoqfot;
	pl->aifunc=&aiZoqfot;
	pl->loadfunc=&LoadZoqfot;
	pl->loadpilots=&SetZoqfotPilot;
	pl->postfunc=&PostZoqfot;
	pl->restorefunc=&RestoreGFXZoqfot;

	pl->ditty=&zoqfot_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+32+512)/16;
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
void DrawTongue(pPlayer pl)
{
	s8 b=0;
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=1;
	pl->weapon[b].damage=-1*TONGUE_DAMAGE;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

		pl->weapon[b].size=16;
		pl->weapon[b].angle = pl->angle;
		pl->weapon[b].actualangle = pl->actualangle;

		pl->weapon[b].xspeed=0;
		pl->weapon[b].yspeed=0;


		pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset * SIN[pl->angle])>>8);
		pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset * COS[pl->angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	if (pl->weapon[b].status==1||pl->weapon[b].status==6)
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
	else
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+80 | PRIORITY(1);


}

int SpecialZoqfot(pPlayer pl)
{

	play_sfx(&zoqfot_tonque,pl->plr-1);
	pl->weapon[0].status=6;
	DrawTongue(pl);
}

int aiZoqfot(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc,lpEnemyEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	s8 GiveTongueJob;


		GiveTongueJob =0;
		if (ai->special_turn == 0)
		{

			ai->ship_input_state &= ~SPECIAL;

			lpEnemyEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
			if (lpEnemyEvalDesc->parent
					&& lpEnemyEvalDesc->which_turn <= 4
				)
			{


				GiveTongueJob = 1;

				s8 angle =FindAngle(ai->xpos,ai->ypos,opp->xpos,opp->ypos);

				if (TurnAngle(angle,ai->angle,45)==0)
					ai->ship_input_state |= SPECIAL;
			}
		}

		++ai->weapon_turn;
		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);
		--ai->weapon_turn;

		if (ai->weapon_turn == 0)
		{
			ai->ship_input_state &= ~WEAPON;
			if (!GiveTongueJob)
			{
				ObjectsOfConcern += ConcernCounter;
				while (ConcernCounter--)
				{
					--ObjectsOfConcern;
					if (ObjectsOfConcern->parent
							&& (ConcernCounter == ENEMY_SHIP_INDEX
							|| (ConcernCounter == ENEMY_WEAPON_INDEX
							&& ObjectsOfConcern->MoveState != AVOID

							))

							//&& ship_weapons (ShipPtr,
							//ObjectsOfConcern->ObjectPtr, DISPLAY_TO_WORLD (20)))
							)
					{
						ai->ship_input_state |= WEAPON;
						break;
					}
				}
			}
	}

}

void SetZoqfotPilot(pPlayer pl)
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

void RestoreGFXZoqfot(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+96 | PRIORITY(1);
			}
		}
	}
}

void PostZoqfot(pPlayer p)
{
	//tongue
	if (p->weapon[0].status>0)
	{
		DrawTongue(p);
	}

	p->weapon[0].status--;
	if (p->weapon[0].status==0);
		p->weapon[0].life=0;


}

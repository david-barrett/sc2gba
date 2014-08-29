#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "probe_out.h"
#include "arilou_laser.h"



#include "probe_sfx.h"

#include "probepilot.h"
/*
#include "probepilotl.h"
#include "probepilotr.h"
#include "probepilott.h"
#include "probepilotf.h"
#include "probepilots.h"
*/

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;
extern pAsteroid asteroids;
extern s32 zoom;


void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

int FireProbe(pPlayer pl);
int SpecialProbe(pPlayer pl);
int aiProbe(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetProbePilot(pPlayer p);
void RestoreGFXProbe(pPlayer p);
void MoveDogi(pWeapon ur);
void PostProbe(pPlayer p);

#define MAX_CREW 12
#define MAX_ENERGY 20
#define ENERGY_REGENERATION 0
#define WEAPON_ENERGY_COST 2
#define SPECIAL_ENERGY_COST 0
#define ENERGY_WAIT 10
#define MAX_THRUST 60
#define THRUST_INCREMENT MAX_THRUST
#define TURN_WAIT 0
#define THRUST_WAIT 0
#define WEAPON_WAIT 17
#define SPECIAL_WAIT 20

#define SHIP_MASS 1
#define SLYLANDRO_OFFSET 9// /*DISPLAY_TO_WORLD*/ (100 + ARILOU_OFFSET)

//HARVEST_RANGE was originally (SPACE_HEIGHT * 3 / 8)
#define HARVEST_RANGE (208 * 3 / 8)



void LoadProbe(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = probeData[loop-OAMStart];
       		OAMData[loop+512] = probe_outData[loop-OAMStart];
       		OAMData[loop+1024] = arilou_laserData[loop-OAMStart];
   	}
	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+512] = probepilotData[loop-OAMStart];
	}
/*
	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+64] = probepilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512] = probepilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+64] = probepilottData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+256] = probepilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+512] = probepilotsData[loop-OAMStart];

	}
*/
}

int FireProbe(pPlayer pl)
{
	pPlayer opp=(pPlayer)pl->opp;
	//play_sfx(&probe_fire,pl->plr-1);

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



void SetProbe(pPlayer pl)
{
	pl->crew=MAX_CREW;
		pl->maxcrew=MAX_CREW;
		pl->batt=MAX_ENERGY;
		pl->maxbatt=MAX_ENERGY;

		pl->maxspeed=MAX_THRUST;

		pl->accel_inc=0;//THRUST_INCREMENT;

		pl->firebatt=WEAPON_ENERGY_COST;
		pl->specbatt=SPECIAL_ENERGY_COST;

		pl->batt_wait=ENERGY_WAIT;
		pl->turn_wait=TURN_WAIT;
		pl->thrust_wait=THRUST_WAIT;
		pl->weapon_wait=WEAPON_WAIT;
		pl->special_wait=SPECIAL_WAIT;
		pl->batt_regen=ENERGY_REGENERATION;
		pl->mass=SHIP_MASS;

			pl->offset=15;

		s16 o = (pl->plr-1)*13;

		pl->ffiresprite=1+o;
		pl->lfiresprite=4+o;

		pl->fspecsprite=5+o;
		pl->lspecsprite=12+o;

		pl->range=100;

	pl->fireangle=45;

	pl->firefunc=&FireProbe;
	pl->specfunc=&SpecialProbe;
	pl->aifunc=&aiProbe;
	pl->loadfunc=&LoadProbe;
	pl->loadpilots=&SetProbePilot;
	pl->postfunc=&PostProbe;
	pl->restorefunc=&RestoreGFXProbe;

	pl->ditty=&probe_ditty;

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
	pl->drawangle=0;

}

int SpecialProbe(pPlayer pl)
{
	extern pAsteroid asteroids;
	s32 d;

	for (int i=0;i<5;i++)
	{
		if (asteroids[i].life>0)
		{
			d=distanceBetweenPoints(pl->xpos,pl->ypos,asteroids[i].xpos,asteroids[i].ypos);
			if (d<HARVEST_RANGE)
			{
				asteroids[i].life=0;
				ModifyBatt(pl,MAX_ENERGY);
				play_sfx(&probe_energise,pl->plr-1);
			}
		}
	}

	return 1;


}

int aiProbe(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;


		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];

		if (ai->special_turn == 0
				&& ai->batt == 0
				&& ObjectsOfConcern[GRAVITY_MASS_INDEX].parent == 0)
			ConcernCounter = FIRST_EMPTY_INDEX + 1;
		if (lpEvalDesc->parent && lpEvalDesc->MoveState == PURSUE
				&& lpEvalDesc->which_turn <= 6)
			lpEvalDesc->MoveState = ENTICE;

		++ai->thrust_turn;
		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);
		--ai->thrust_wait;

		if (lpEvalDesc->parent && lpEvalDesc->which_turn <= 14)
			ai->ship_input_state |= WEAPON;
		else
			ai->ship_input_state &= ~WEAPON;

		ai->ship_input_state &= ~SPECIAL;
		if (ai->batt < ai->maxbatt)
		{
			lpEvalDesc = &ObjectsOfConcern[FIRST_EMPTY_INDEX];
			if (lpEvalDesc->parent && lpEvalDesc->which_turn <= 14)
				ai->ship_input_state |= SPECIAL;
	}

}

void SetProbePilot(pPlayer pl)
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

void RestoreGFXProbe(pPlayer p)
{


}

void PostProbe(pPlayer p)
{

	if ((p->ship_input_state & THRUST))//&&(p->thrust_turn<=p->thrust_wait))
	{
		p->actualangle+=8;
		if (p->actualangle>15)
			p->actualangle-=16;
		p->angle=(p->actualangle*45)>>1;
		//p->thrust_turn=p->thrust_wait*2;

	}

	p->xpos+=(s32)(p->maxspeed*SIN[p->angle])>>8;
	p->ypos-=(s32)(p->maxspeed*COS[p->angle])>>8;

	p->drawangle++;
	if (p->drawangle==16)
		p->drawangle=0;

	RotateSprite(p->plr==1?0:13, (p->drawangle*45)>>1, zoom, zoom);

}

#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "probe_out.h"
#include "probe_laser.h"



#include "probe_sfx.h"

#include "probepilot.h"
#include "probepilotl.h"
#include "probepilotr.h"
#include "probepilott.h"
#include "probepilotf.h"
#include "probepilots.h"

#include "probecrew1.h"
#include "probecrew2.h"
#include "probecrew3.h"
#include "probecrew4.h"
#include "probecrew5.h"
#include "probecrew6.h"
#include "probecrew7.h"
#include "probecrew8.h"

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
#define MAX_THRUST SHIP_SPEED(60)
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
       		OAMData[loop+1024] = probe_laserData[loop-OAMStart];
   	}
	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+512] = probepilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+64;loop++)
	{
		OAMData[loop+(1024*2)+512] = probepilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512] = probepilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)	
		OAMData[loop+(1024*2)+128+512] = probepilottData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
		OAMData[loop+(1024*2)+128+512+256] = probepilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+128+512+256] = probepilotsData[loop-OAMStart];

	}

	for (loop=OAMStart ;loop<OAMStart+32;loop++)
	{
		OAMData[loop+(1024*4)+128+512+256] = probecrew1Data[loop-OAMStart];
		OAMData[loop+(1024*4)+128+512+256+32] = probecrew2Data[loop-OAMStart];
		OAMData[loop+(1024*4)+128+512+256+64] = probecrew3Data[loop-OAMStart];
		OAMData[loop+(1024*4)+128+512+256+96] = probecrew4Data[loop-OAMStart];
		OAMData[loop+(1024*5)] = probecrew5Data[loop-OAMStart];
		OAMData[loop+(1024*5)+32] = probecrew6Data[loop-OAMStart];
		OAMData[loop+(1024*5)+64] = probecrew7Data[loop-OAMStart];
		OAMData[loop+(1024*5)+96] = probecrew8Data[loop-OAMStart];
	}
}

int FireProbe(pPlayer pl)
{
	pPlayer opp=(pPlayer)pl->opp;
	//play_sfx(&probe_fire,pl->plr-1);

	for (int b=0;b<3;b++)
	{


			pl->weapon[b].type=LASER;
			pl->weapon[b].object.life=2;
			pl->weapon[b].damage=-2;//GUESS
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
		pl->object.mass_points=SHIP_MASS;

			pl->offset=15;

		s16 o = (pl->plr-1)*13;

		pl->ffiresprite=1+o;
		pl->lfiresprite=4+o;

		pl->fspecsprite=5+o;
		pl->lspecsprite=12+o;

		pl->range=90;


	pl->firefunc=&FireProbe;
	pl->specfunc=&SpecialProbe;
	pl->aifunc=&aiProbe;
	pl->loadfunc=&LoadProbe;
	pl->loadpilots=&SetProbePilot;
	pl->postfunc=&PostProbe;
	pl->restorefunc=&RestoreGFXProbe;

	pl->ditty=&probe_ditty;

	pl->ship_flags = IMMEDIATE_WEAPON;
	pl->object.mass_points=SHIP_MASS;

	pl->pilot_sprite=(1024+512)/16;
	pl->pilots[0].x=47;
	pl->pilots[0].y=1;
	pl->pilots[1].x=9;
	pl->pilots[1].y=0;
	pl->pilots[2].x=15;
	pl->pilots[2].y=14;
	pl->pilots[3].x=4;
	pl->pilots[3].y=4;
	pl->pilots[4].x=511;
	pl->pilots[4].y=5;

	pl->charging=0;
	for (int i=8;i<12;i++)
		pl->weapon[i].object.life=-2;
	pl->drawangle=0;

}

int SpecialProbe(pPlayer pl)
{
	extern pAsteroid asteroids;
	s32 d;
	s8 found=0;

	for (int i=0;i<5;i++)
	{
		if (asteroids[i].object.life>0)
		{
			d=distanceBetweenPoints(pl->object.xpos,pl->object.ypos,asteroids[i].object.xpos,asteroids[i].object.ypos);
			if (d<HARVEST_RANGE)
			{
				asteroids[i].object.life=0;
				found=1;
			}
		}
	}
	if (found)
	{
		ModifyBatt(pl,MAX_ENERGY);
		play_sfx(&probe_energise,pl->plr-1);
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

	sprites[43+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[43+off].attribute1 =SIZE_8 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[44+off].attribute1 =SIZE_8 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+68 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[45+off].attribute1 =SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+72 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |TALL  | 160;
	sprites[46+off].attribute1 =SIZE_64 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+88 | PRIORITY(1);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 =SIZE_64 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+152 | PRIORITY(1);
}

void RestoreGFXProbe(pPlayer p)
{


}

void PostProbe(pPlayer p)
{

	if ((p->ship_input_state & THRUST))//&&(p->thrust_turn<=p->thrust_wait))
	{
		p->object.actualangle+=8;
		if (p->object.actualangle>15)
			p->object.actualangle-=16;
		p->object.angle=(p->object.actualangle*45)>>1;
		//p->thrust_turn=p->thrust_wait*2;

	}

	p->object.xpos+=(s32)(p->maxspeed*SIN[p->object.angle])>>8;
	p->object.ypos-=(s32)(p->maxspeed*COS[p->object.angle])>>8;

	p->drawangle++;
	if (p->drawangle==16)
		p->drawangle=0;

	RotateSprite(p->plr==1?0:13, (p->drawangle*45)>>1, zoom, zoom);

}

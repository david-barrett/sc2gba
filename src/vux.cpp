#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "vux_out.h"
#include "vux_laserbody.h"
#include "vux_laserend.h"
#include "vux_limpet.h"

#include "vux_sfx.h"

#include "vuxpilot.h"
#include "vuxpilotl.h"
#include "vuxpilotr.h"
#include "vuxpilott.h"
#include "vuxpilotf.h"
#include "vuxpilots.h"

#define MAX_CREW 20
#define MAX_ENERGY 40
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 1
#define SPECIAL_ENERGY_COST 2
#define ENERGY_WAIT 8
#define MAX_THRUST /* DISPLAY_TO_WORLD (5) */ SHIP_SPEED(21)
#define THRUST_INCREMENT /* DISPLAY_TO_WORLD (2) */ SHIP_SPEED(7)
#define TURN_WAIT 6
#define THRUST_WAIT 4
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 7

#define SHIP_MASS 6
#define WARP_OFFSET 46 /* How far outside of laser-range ship can warp in */
#define VUX_OFFSET 12
#define LASER_BASE 150
#define LASER_RANGE (LASER_BASE + VUX_OFFSET)//DISPLAY_TO_WORLD (LASER_BASE + VUX_OFFSET)


extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireVux(pPlayer pl);
int SpecialVux(pPlayer pl);
int aiVux(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetVuxPilot(pPlayer p);
void RestoreGFXVux(pPlayer p);

void LoadVux(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = vuxData[loop-OAMStart];
       		OAMData[loop+512] = vux_outData[loop-OAMStart];
       		OAMData[loop+1024] = vux_laserbodyData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512+512] = vux_laserendData[loop-OAMStart]; //loads some garb
		OAMData[loop+512+512+512+32] = vux_limpetData[loop-OAMStart]; //loads some garb
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+512+32+32] = vuxpilotData[loop-OAMStart];
		//	OAMData[loop+(1024*2)+512+32] = vuxpilotlData[loop-OAMStart];
		//	OAMData[loop+(1024*3)+512+32] = vuxpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+512+64] = vuxpilotlData[loop-OAMStart];
		OAMData[loop+(1024*3)+64] = vuxpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+64+512] = vuxpilottData[loop-OAMStart];

	}
	for (loop=OAMStart ;loop<OAMStart+64;loop++)
		OAMData[loop+(1024*3)+64+512+256] = vuxpilotfData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
		OAMData[loop+(1024*3)+64+512+256+64] = vuxpilotsData[loop-OAMStart];
}

int SpecialVux(pPlayer pl)
{
	s16 b= nextWeapon(pl,4,11);
	int ret=0;
	if (b>0)
	{
		pl->weapon[b].type=LIMPET;
		pl->weapon[b].life=1;
		pl->weapon[b].status=80;
		pl->weapon[b].damage=0;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=8;
		pl->weapon[b].angle = 0;

		pl->weapon[b].xspeed=0;
		pl->weapon[b].yspeed=0;


		pl->weapon[b].xpos = pl->xpos-((s32)(2 * SIN[pl->angle])>>8);
		pl->weapon[b].ypos = pl->ypos+((s32)(2 * COS[pl->angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+98 | PRIORITY(1);
		ret=1;
		play_sfx(&vux_limpet,pl->plr-1);
	}



	return ret;
}

void SetVux(pPlayer pl)
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

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=200;

	pl->offset=9;

	pl->fireangle=45;

	pl->firefunc=&FireVux;
	pl->specfunc=&SpecialVux;
	pl->aifunc=&aiVux;
	pl->loadfunc=&LoadVux;
	pl->loadpilots=&SetVuxPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXVux;

	pl->ditty=&vux_ditty;

	pl->ship_flags = FIRES_FORE | SEEKING_SPECIAL | IMMEDIATE_WEAPON;


	pl->pilot_sprite=(512+512+512+64)/16;
	pl->pilots[0].x=3;
	pl->pilots[0].y=0;
	pl->pilots[1].x=3;
	pl->pilots[1].y=0;
	pl->pilots[2].x=42;
	pl->pilots[2].y=0;
	pl->pilots[3].x=27;
	pl->pilots[3].y=11;
	pl->pilots[4].x=16;
	pl->pilots[4].y=1;

	//distance for vux start
	//override
	GenerateStart(pl,80);




}

int FireVux(pPlayer pl)
{


	for (int b=0;b<4;b++)
	{


	pl->weapon[b].type=LASER;
	pl->weapon[b].life=1;
	pl->weapon[b].damage=-2;//GUESS
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=(b==3?8:32);
	pl->weapon[b].angle = pl->angle;

	s32 off=(b==3?106:(b*32)+25);

	pl->weapon[b].xspeed=0;
	pl->weapon[b].yspeed=0;


	pl->weapon[b].xpos = pl->xpos+((off * (s32)SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((off * (s32)COS[pl->angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	if (b==3)
	{
    	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);
	}
    else
    {
    	sprites[pl->weapon[b].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
	}

	}
	play_sfx(&vux_laser,pl->plr-1);

	return 1;
}

int aiVux(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;


	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	lpEvalDesc->MoveState = PURSUE;
	if (ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent != 0
			&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].MoveState == ENTICE)
	{
		if (ObjectsOfConcern[ENEMY_WEAPON_INDEX].type!=CREW)
				ObjectsOfConcern[ENEMY_WEAPON_INDEX].MoveState = AVOID;
			else
				ObjectsOfConcern[ENEMY_WEAPON_INDEX].MoveState = PURSUE;
		}

		ship_intelligence (ai,
				ObjectsOfConcern, ConcernCounter);


		if (ai->special_turn== 0
				&& lpEvalDesc->which_turn <= 12
				&& (ai->ship_input_state & (LEFT | RIGHT))
				&& ai->batt >= 20)
			ai->ship_input_state |= SPECIAL;
		else
			ai->ship_input_state &= ~SPECIAL;

}

void SetVuxPilot(pPlayer pl)
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
	sprites[46+off].attribute1 = SIZE_8 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+16 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+20 | PRIORITY(2);
}

void MoveLimpet(pWeapon ur)
{
	s16 angle;
	pPlayer target=(pPlayer)ur->target;

	pPlayer parent=(pPlayer)ur->parent;
	ur->status--;

	if(ur->status==0)
	{
		ur->life=0;
		return;
	}

	if (DetectWeaponToShip(target,ur)==1)
	{
		//attach
		ur->life=0;
		AddLimpet(target);
		play_sfx(&vux_bite,parent->plr-1);
		if (target->turn_wait <9)
				target->turn_wait++;
		if (target->thrust_wait <9)
				target->thrust_wait++;


		if (target->accel_inc <= 2)
		{
				target->maxspeed = 2;

		}
		else
		{
			s16 num_thrusts;

			num_thrusts = target->maxspeed /target->accel_inc ;
			--target->accel_inc;
			target->maxspeed =	target->accel_inc * num_thrusts;
		}
		return;
	}


	if (target->crew>0)
	{

		angle = FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos);

		ur->xpos+=((2) * (s32)SIN[angle])>>8;
		ur->ypos-=((2) * (s32)COS[angle])>>8;
	}

}

void RestoreGFXVux(pPlayer p)
{
	//only restore limpets
	//im sure laser will be fine...
	for(int i=4;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==LIMPET)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+98 | PRIORITY(1);
			}
		}
	}
}

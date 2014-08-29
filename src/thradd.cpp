#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "thradd_out.h"
#include "thradd_bullet.h"
#include "thradd_flame.h"

#include "thradd_sfx.h"

#include "thraddpilot.h"
#include "thraddpilotl.h"
#include "thraddpilotr.h"
#include "thraddpilott.h"
#include "thraddpilotf.h"
#include "thraddpilots.h"


extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireThradd(pPlayer pl);
int SpecialThradd(pPlayer pl);
int aiThradd(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetThraddPilot(pPlayer p);
void RestoreGFXThradd(pPlayer p);
void MoveDogi(pWeapon ur);
void PostThradd(pPlayer p);

extern s32 zoom;


void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#define MAX_CREW 8
#define MAX_ENERGY 24
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 2
#define SPECIAL_ENERGY_COST 1
#define ENERGY_WAIT 6
#define MAX_THRUST SHIP_SPEED(28)
#define THRUST_INCREMENT SHIP_SPEED(7)
#define TURN_WAIT 1
#define THRUST_WAIT 0
#define WEAPON_WAIT 12
#define SPECIAL_WAIT 0

#define SHIP_MASS 7
#define THRADDASH_OFFSET 9
#define MISSILE_SPEED 30 //DISPLAY_TO_WORLD (30)
#define MISSILE_LIFE 15
#define MISSILE_RANGE  MISSILE_SPEED*MISSILE_LIFE
#define MISSILE_DAMAGE 1

#define NAPALM_DAMAGE 2
#define NAPALM_LIFE 40

#define SPECIAL_THRUST_INCREMENT SHIP_SPEED(12)
#define SPECIAL_MAX_THRUST SHIP_SPEED(72)

void LoadThradd(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)          
  	{
       		OAMData[loop] = thraddData[loop-OAMStart];
       		OAMData[loop+512] = thradd_outData[loop-OAMStart];
   	}

   	for(loop = OAMStart; loop < OAMStart+128; loop++)          
   	{
		OAMData[loop+1024] = thradd_bulletData[loop-OAMStart]; 
		OAMData[loop+1024+128] = thradd_flameData[loop-OAMStart]; 
   	}

   	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+256] = thraddpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+256] = thraddpilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+512] = thraddpilotrData[loop-OAMStart];			
		OAMData[loop+(1024*2)+512+256] = thraddpilottData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)	
		OAMData[loop+(1024*3)] = thraddpilotfData[loop-OAMStart];
	for (loop=OAMStart ;loop<OAMStart+128;loop++)
		OAMData[loop+(1024*3)+512] = thraddpilotsData[loop-OAMStart];

	

}

int FireThradd(pPlayer pl)
{
	s16 b =11;//= nextWeapon(pl,9,11);
	s8 ret=0;
	if (b>=0)
	{

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=MISSILE_LIFE;
	pl->weapon[b].damage=-1*MISSILE_DAMAGE; 
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=16;
	pl->weapon[b].angle = 0;
	pl->weapon[b].actualangle = 0;

	pl->weapon[b].xspeed=((s32)(MISSILE_SPEED * SIN[pl->angle])>>8);
	pl->weapon[b].yspeed=((s32)(MISSILE_SPEED * COS[pl->angle])>>8);;


	pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset * SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset * COS[pl->angle])>>8);

	#ifdef MISSILE_START
	pl->weapon[b].xpos-=pl->weapon[b].xspeed;
	pl->weapon[b].ypos+=pl->weapon[b].yspeed;
	#endif

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);

	play_sfx(&thradd_bullet,pl->plr-1);

	ret=1;
	}




	return ret;
}

void SetThradd(pPlayer pl)
{
	pl->crew=MAX_CREW;
		pl->maxcrew=MAX_CREW;
		pl->batt=MAX_ENERGY;
		pl->maxbatt=MAX_ENERGY;

		pl->maxspeed=MAX_THRUST;

		pl->accel_inc=THRUST_INCREMENT;

		pl->firebatt=WEAPON_ENERGY_COST;
		pl->specbatt=SPECIAL_ENERGY_COST;

		pl->offset=11;

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

		pl->range=MISSILE_RANGE;
	

	pl->firefunc=&FireThradd;
	pl->specfunc=&SpecialThradd;
	pl->aifunc=&aiThradd;
	pl->loadfunc=&LoadThradd;
	pl->loadpilots=&SetThraddPilot;
	pl->postfunc=&PostThradd;
	pl->restorefunc=&RestoreGFXThradd;

	pl->ditty=&thradd_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+256)/16;
	pl->pilots[0].x=43;
	pl->pilots[0].y=5;
	pl->pilots[1].x=43;
	pl->pilots[1].y=2;
	pl->pilots[2].x=27;
	pl->pilots[2].y=15;
	pl->pilots[3].x=3;
	pl->pilots[3].y=0;
	pl->pilots[4].x=23;
	pl->pilots[4].y=13;

	pl->currentweapon=0;

}


int SpecialThradd(pPlayer pl)
{
	pl->currentweapon++;
	if (pl->currentweapon==11)
		pl->currentweapon=0;
	s16 b=pl->currentweapon;



	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=NAPALM_LIFE;
	pl->weapon[b].damage=-1*NAPALM_DAMAGE;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=16;
	pl->weapon[b].angle = 0;
	pl->weapon[b].actualangle = 0;

	pl->weapon[b].xspeed=0;
	pl->weapon[b].yspeed=0;


	pl->weapon[b].xpos = pl->xpos;//-((s32)(pl->offset * SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos;//+((s32)(pl->offset * COS[pl->angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+72 | PRIORITY(1);

	pl->maxspeed=SPECIAL_MAX_THRUST;
	pl->accel_inc=SPECIAL_THRUST_INCREMENT;
	s16 turn=pl->thrust_turn;

	Thrust(pl);

	pl->maxspeed=MAX_THRUST;
	pl->accel_inc=THRUST_INCREMENT;
	pl->thrust_turn=turn;

	return 1;

}

int aiThradd(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc,lpEnemyEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	if (lpEvalDesc->parent)
	{
#define STATIONARY_SPEED 4
		
		if (lpEvalDesc->which_turn > 8
				|| distanceBetweenPoints(0,0,ai->xspeed,ai->yspeed) <=
				(long)STATIONARY_SPEED * STATIONARY_SPEED)
			lpEvalDesc->MoveState = PURSUE;
		else
			lpEvalDesc->MoveState = ENTICE;
	}
	ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

	if (ai->special_turn == 0)
	{
		ai->ship_input_state &= ~SPECIAL;
		if (ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent
				&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].MoveState == ENTICE)
		{
			if ((ai->ship_input_state & THRUST)
					|| (ai->turn_turn == 0
					&& !(ai->ship_input_state & (LEFT | RIGHT)))
					|| TurnAngle(FindAngle(0,0,ObjectsOfConcern[ENEMY_WEAPON_INDEX].xspeed,ObjectsOfConcern[ENEMY_WEAPON_INDEX].yspeed),
						ai->angle,90)==0)
				ai->ship_input_state |= SPECIAL;
		}
		else if (lpEvalDesc->parent)
		{
			if (lpEvalDesc->MoveState == PURSUE)
			{
				if (ai->batt >= WEAPON_ENERGY_COST
						+ SPECIAL_ENERGY_COST
						&& ai->turn_turn == 0
						&& !(ai->ship_input_state & (LEFT | RIGHT))
						&& (!(ai->ship_input_state & SPECIAL)
						|| (distanceBetweenPoints(0,0,ai->xspeed,ai->yspeed)<ai->maxspeed)))
					ai->ship_input_state |= SPECIAL;
			}
			else if (lpEvalDesc->MoveState == ENTICE)
			{
				s16 direction_angle=FindAngle(0,0,ai->xspeed,ai->yspeed);

				if ((lpEvalDesc->which_turn > 24
						&& !(ai->ship_input_state & (LEFT | RIGHT)))
						|| (lpEvalDesc->which_turn <= 16
						&& TurnAngle(direction_angle,ai->angle,180)==0
						&& (lpEvalDesc->which_turn < 12
						|| TurnAngle(direction_angle,
							FindAngle(0,0,lpEvalDesc->xpos,lpEvalDesc->ypos),180==0))))
					ai->ship_input_state |= SPECIAL;
			}
		}

		if ((ai->ship_input_state & SPECIAL)
				&& ai->batt >=
				SPECIAL_ENERGY_COST)
			ai->ship_input_state &= ~THRUST;
	}


}

void SetThraddPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | WIDE | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(1);

	sprites[46+off].attribute0 = COLOR_256 |SQUARE  | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+112 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[47+off].attribute1 = SIZE_16 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+144 | PRIORITY(2);
}

void RestoreGFXThradd(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = FireSprite1+4 | PRIORITY(1);
			}

		}
	}
}

void PostThradd(pPlayer p)
{

}


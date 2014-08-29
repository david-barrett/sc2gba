#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "supox_out.h"
#include "supox_fire.h"

#include "supox_sfx.h"

#include "supoxpilot.h"
#include "supoxpilotl.h"
#include "supoxpilotr.h"
#include "supoxpilott.h"
#include "supoxpilotf.h"
#include "supoxpilots.h"

#define MAX_CREW 12
#define MAX_ENERGY 16
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 1
#define SPECIAL_ENERGY_COST 1
#define ENERGY_WAIT 4
#define MAX_THRUST SHIP_SPEED(40)
#define THRUST_INCREMENT SHIP_SPEED(8)
#define TURN_WAIT 1
#define THRUST_WAIT 0
#define WEAPON_WAIT 2
#define SPECIAL_WAIT 0

#define SHIP_MASS 4
#define MISSILE_SPEED DISPLAY_TO_WORLD (30)
#define MISSILE_LIFE 10
#define MISSILE_DAMAGE 1

#define ZIP_SPEED 30//Guess


extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireSupox(pPlayer pl);
int SpecialSupox(pPlayer pl);
int aiSupox(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetSupoxPilot(pPlayer p);
//void PostSupox(pPlayer p);
void RestoreGFXSupox(pPlayer p);


void LoadSupox(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)            
  	{
       		OAMData[loop] = supoxData[loop-OAMStart];
       		OAMData[loop+512] = supox_outData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)              
   	{
		OAMData[loop+512+512] = supox_fireData[loop-OAMStart]; 
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+32] = supoxpilotData[loop-OAMStart];

	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+32] = supoxpilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+256+32] = supoxpilotrData[loop-OAMStart];
		OAMData[loop+(1024*2)+512+32] = supoxpilottData[loop-OAMStart];
		OAMData[loop+(1024*2)+512+256+32] = supoxpilotfData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
		OAMData[loop+(1024*3)+32] = supoxpilotsData[loop-OAMStart];


}

int SpecialSupox(pPlayer pl)
{
	s16 angle;
	if (pl->ship_input_state & THRUST)
	{
		//backwards
		angle=ModifyAngle(pl->angle,180);
	}
	else if (pl->ship_input_state & LEFT)
	{
		//left
		angle=ModifyAngle(pl->angle,270);
	}
	else if (pl->ship_input_state & RIGHT)
	{
		//right
		angle=ModifyAngle(pl->angle,90);
	}
	else 
		return 0;
	pl->xpos+=((s32)(ZIP_SPEED*SIN[angle])>>8);
	pl->ypos-=((s32)(ZIP_SPEED*COS[angle])>>8);
		

	pl->ship_input_state &= ~THRUST;
	pl->ship_input_state &= ~LEFT;
	pl->ship_input_state &= ~RIGHT;

	//pl->ship_input_state=0;

	return 1;
}

void SetSupox(pPlayer pl)
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
		pl->offset=15;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=200;

	pl->fireangle=45;

	pl->firefunc=&FireSupox;
	pl->specfunc=&SpecialSupox;
	pl->aifunc=&aiSupox;
	pl->loadfunc=&LoadSupox;
	pl->loadpilots=&SetSupoxPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXSupox;

	pl->ditty=&supox_ditty;

	pl->ship_flags = FIRES_FORE ;

	pl->pilot_sprite=(512+512+32)/16;
	pl->pilots[0].x=28;
	pl->pilots[0].y=15;
	pl->pilots[1].x=4;
	pl->pilots[1].y=15;
	pl->pilots[2].x=4;
	pl->pilots[2].y=1;
	pl->pilots[3].x=23;
	pl->pilots[3].y=5;
	pl->pilots[4].x=27;
	pl->pilots[4].y=1;
}

int FireSupox(pPlayer pl)
{
	s16 b= nextWeapon(pl);//,0,4);

	if (b>=0)
	{

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=10;
	pl->weapon[b].damage=-1*MISSILE_DAMAGE;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = pl->angle;

	s32 speed=20;//20;
	pl->weapon[b].xspeed = ((speed * (s32)SIN[pl->angle])>>8);///SPEED_REDUCT;
	pl->weapon[b].yspeed = ((speed * (s32)COS[pl->angle])>>8);///SPEED_REDUCT;

	pl->weapon[b].xpos = pl->xpos+((40 * (s32)SIN[pl->angle])>>8)/3;
	pl->weapon[b].ypos = pl->ypos-((40 * (s32)COS[pl->angle])>>8)/3;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
  	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);

	play_sfx(&supox_bullet,pl->plr-1);
	return 1;
	}
	return 0;
}

int aiSupox(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;
	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	if (ai->special_turn || lpEvalDesc->parent == 0)
		ai->ship_input_state &= ~SPECIAL;
	else
	{
		s8 LinedUp;
		s16 direction_angle;		

		

		LinedUp = (TurnAngle(ai->angle,FindAngle(0,0,ai->xspeed,ai->yspeed),180)==0);

		if (!LinedUp
				|| lpEvalDesc->which_turn > 20
				|| TurnAngle(ai->angle,lpEvalDesc->angle,180)==0)
			ai->ship_input_state &= ~SPECIAL;
		else if (LinedUp && lpEvalDesc->which_turn <= 12)
			ai->ship_input_state |= SPECIAL;

		if (ai->ship_input_state & SPECIAL)
			lpEvalDesc->MoveState = PURSUE;
	}

	ship_intelligence (ai,
			ObjectsOfConcern, ConcernCounter);

	if (ai->ship_input_state & SPECIAL)
		ai->ship_input_state |= THRUST | WEAPON;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
	if (ai->special_turn == 0
			&& lpEvalDesc->parent
			&& lpEvalDesc->MoveState == AVOID
			&& ai->turn_turn == 0)
	{
		ai->ship_input_state &= ~THRUST;
		ai->ship_input_state |= SPECIAL;
		if (!(ai->ship_input_state & (LEFT | RIGHT)))
			ai->ship_input_state |= 1 << (ran(0,1));
		else
			ai->ship_input_state |=
					ai->ship_input_state & (LEFT | RIGHT);
	}

}

void SetSupoxPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | WIDE | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(1);

	sprites[46+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+112 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[47+off].attribute1 = SIZE_32| 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);
}



void RestoreGFXSupox(pPlayer p)
{
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);

		}
	}
}


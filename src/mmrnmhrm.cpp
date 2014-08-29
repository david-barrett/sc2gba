#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "mmrnmhrm_out1.h"
#include "mmrnmhrm1.h"
#include "mmrnmhrm_fire1a.h"
#include "mmrnmhrm_fire1b1.h"
#include "mmrnmhrm_fire1b2.h"
#include "mmrnmhrm_fire1c.h"
#include "mmrnmhrm_fire1d.h"
#include "mmrnmhrm_fire1e.h"
#include "mmrnmhrm_fire1f.h"
#include "mmrnmhrm_fire1g.h"

#include "mmrnmhrm_out2.h"
#include "mmrnmhrm2.h"
#include "mmrnmhrm_fire2.h"

#include "mmrnmhrm_sfx.h"

#include "mmrnmhrmpilot.h"
#include "mmrnmhrmpilotl.h"
//#include "mmrnmhrmpilotr.h"
#include "mmrnmhrmpilott.h"
#include "mmrnmhrmpilotf.h"
#include "mmrnmhrmpilots.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireMmrnmhrm(pPlayer pl);
int SpecialMmrnmhrm(pPlayer pl);
int aiMmrnmhrm(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetMmrnmhrmPilot(pPlayer p);
void RestoreGFXMmrnmhrm(pPlayer p);
void MoveMmrnmhrmMissile(pWeapon ur);

#define MAX_CREW 20
#define MAX_ENERGY 10
#define ENERGY_REGENERATION 2
#define WEAPON_ENERGY_COST 1
#define SPECIAL_ENERGY_COST MAX_ENERGY
#define ENERGY_WAIT 6
#define MAX_THRUST SHIP_SPEED(20)
#define THRUST_INCREMENT SHIP_SPEED (5)
#define TURN_WAIT 2
#define THRUST_WAIT 1
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 0

#define YWING_ENERGY_REGENERATION 1
#define YWING_WEAPON_ENERGY_COST 1
#define YWING_SPECIAL_ENERGY_COST MAX_ENERGY
#define YWING_ENERGY_WAIT 6
#define YWING_MAX_THRUST SHIP_SPEED(50)
#define YWING_THRUST_INCREMENT SHIP_SPEED(10)
#define YWING_TURN_WAIT 14
#define YWING_THRUST_WAIT 0
#define YWING_WEAPON_WAIT 20
#define YWING_SPECIAL_WAIT 0

#define SHIP_MASS 3
#define MMRNMHRM_OFFSET 16
#define LASER_RANGE  (125 + MMRNMHRM_OFFSET)

#define MISSILE_SPEED 20
#define TRACK_WAIT 5
#define MISSILE_DAMAGE 1
#define MISSILE_LIFE 40
#define MISSILE_RANGE MISSILE_SPEED*MISSILE_LIFE


void LoadMmrnmhrm(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = mmrnmhrm1Data[loop-OAMStart];
       		OAMData[loop+512] = mmrnmhrm_out1Data[loop-OAMStart];
       		OAMData[loop+1024] = mmrnmhrm_fire1aData[loop-OAMStart];
       		//OAMData[loop+1024+512] = mmrnmhrm_fire1bData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+512+1024] = mmrnmhrm_fire2Data[loop-OAMStart]; //loads some garb
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+32+512] = mmrnmhrmpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+32+512] = mmrnmhrmpilotlData[loop-OAMStart];
		//OAMData[loop+(1024*2)+32+512] = mmrnmhrmpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+32] = mmrnmhrmpilottData[loop-OAMStart];
		OAMData[loop+(1024*3)+32+256] = mmrnmhrmpilotfData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
		OAMData[loop+(1024*3)+32+512] = mmrnmhrmpilotsData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		  		OAMData[loop+(1024*4)+512+32] = mmrnmhrm_fire1b1Data[loop-OAMStart];
				OAMData[loop+(1024*4)+512+32+128] = mmrnmhrm_fire1b2Data[loop-OAMStart];
				OAMData[loop+(1024*4)+512+32+256] = mmrnmhrm_fire1cData[loop-OAMStart];
				OAMData[loop+(1024*4)+512+32+256+128] = mmrnmhrm_fire1dData[loop-OAMStart];
				OAMData[loop+(1024*5)+32] = mmrnmhrm_fire1eData[loop-OAMStart];
				OAMData[loop+(1024*5)+32+128] = mmrnmhrm_fire1fData[loop-OAMStart];
				OAMData[loop+(1024*5)+32+128] = mmrnmhrm_fire1gData[loop-OAMStart];
	}

	

}

int FireMmrnmhrm(pPlayer pl)
{

	s16 b;//= nextWeapon(pl);
	int ret=0;

	if (pl->form==0)
	{
		//probably need to increse in size!!
		//laser
		play_sfx(&mmrnmhrm_laser,pl->plr-1);

		for (int b=0;b<7;b++)
		{


		pl->weapon[b].type=LASER;
		pl->weapon[b].life=1;
		pl->weapon[b].damage=-2;//GUESS
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=(b==0?32:16);
		pl->weapon[b].angle = pl->angle;

		s32 off=(b==0?13:22+(b*14));
		s16 sprite=pl->SpriteStart+64;//b==0;
		if (b>0)
			sprite=pl->SpriteStart+282+(b*8);
		

		pl->weapon[b].xspeed=0;
		pl->weapon[b].yspeed=0;


		pl->weapon[b].xpos = pl->xpos+((off * (s32)SIN[pl->angle])>>8);
		pl->weapon[b].ypos = pl->ypos-((off * (s32)COS[pl->angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		if (b==0)
			sprites[pl->weapon[b].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
		else
			sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;		
		sprites[pl->weapon[b].sprite].attribute2 = sprite | PRIORITY(1);
		}

	}
	else
	{
		play_sfx(&mmrnmhrm_missile,pl->plr-1);
		for (int i=0;i<2;i++)
		{
		b= nextWeapon(pl);
		//missile

	if (b>=0)
	{
		pl->weapon[b].type=MISSILE;
		pl->weapon[b].movefunc=&MoveMmrnmhrmMissile;
		pl->weapon[b].life=1;
		pl->weapon[b].status=MISSILE_LIFE;
		pl->weapon[b].damage=-1*MISSILE_DAMAGE;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=8;
		//pl->weapon[b].angle = ModifyAngle(pl->angle,(i==0?-30:+30));
		if (i==0)
		{
			pl->weapon[b].actualangle = pl->actualangle-1;
			if (pl->weapon[b].actualangle==-1)
				pl->weapon[b].actualangle=15;
		}
		else
		{
			pl->weapon[b].actualangle = pl->actualangle+1;
					if (pl->weapon[b].actualangle==16)
			pl->weapon[b].actualangle=0;
		}

		pl->weapon[b].angle=(pl->weapon[b].actualangle*45)>>1;

		pl->weapon[b].xspeed = ((MISSILE_SPEED) * (s32)SIN[pl->weapon[b].angle])>>8;
		pl->weapon[b].yspeed = ((MISSILE_SPEED) * (s32)COS[pl->weapon[b].angle])>>8;


		pl->weapon[b].turn_wait=0;


		pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[ModifyAngle(pl->angle,(i==0?-30:+30))])>>8)/3;
		pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[ModifyAngle(pl->angle,(i==0?-30:+30))])>>8)/3;

		#ifdef MISSILE_START
		pl->weapon[b].xpos-=pl->weapon[b].xspeed;
		pl->weapon[b].ypos+=pl->weapon[b].yspeed;
		#endif

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);
		ret=1;

		}
		}

	}

	return 1;
}

void SetMmrnmhrm(pPlayer pl)
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

		pl->range=LASER_RANGE;

	

	pl->firefunc=&FireMmrnmhrm;
	pl->specfunc=&SpecialMmrnmhrm;
	pl->aifunc=&aiMmrnmhrm;
	pl->loadfunc=&LoadMmrnmhrm;
	pl->loadpilots=&SetMmrnmhrmPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXMmrnmhrm;

	pl->ditty=&mmrnmhrm_ditty;

	pl->form=0;

	pl->ship_flags = FIRES_FORE | IMMEDIATE_WEAPON;
	pl->mass=5;

	pl->pilot_sprite=(1024+32+512)/16;
	pl->pilots[0].x=41;
	pl->pilots[0].y=256;
	pl->pilots[1].x=5;
	pl->pilots[1].y=256;
	pl->pilots[2].x=16;
	pl->pilots[2].y=15;
	pl->pilots[3].x=15;
	pl->pilots[3].y=0;
	pl->pilots[4].x=0;
	pl->pilots[4].y=0;


}

int SpecialMmrnmhrm(pPlayer pl)
{
	s16 OAMStart=16*pl->SpriteStart;
	s16 loop;
	play_sfx(&mmrnmhrm_trans,pl->plr-1);

	if (pl->form==0)
	{
		pl->form=1;
		for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
	 	{
	   		OAMData[loop] = mmrnmhrm2Data[loop-OAMStart];
       		OAMData[loop+512] = mmrnmhrm_out2Data[loop-OAMStart];

			pl->maxspeed=YWING_MAX_THRUST;
			pl->accel_inc=YWING_THRUST_INCREMENT;
			pl->firebatt=YWING_WEAPON_ENERGY_COST;
			pl->specbatt=YWING_SPECIAL_ENERGY_COST;
			pl->batt_wait=YWING_ENERGY_WAIT;
			pl->turn_wait=YWING_TURN_WAIT;
			pl->thrust_wait=YWING_THRUST_WAIT;
			pl->weapon_wait=YWING_WEAPON_WAIT;
			pl->special_wait=YWING_SPECIAL_WAIT;
			pl->batt_regen=YWING_ENERGY_REGENERATION;
			pl->range=LASER_RANGE;
		}
	}
	else
	{
		pl->form=0;
		for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
	 	{
	   		OAMData[loop] = mmrnmhrm1Data[loop-OAMStart];
	   		OAMData[loop+512] = mmrnmhrm_out1Data[loop-OAMStart];
		}

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
		pl->range=MISSILE_RANGE;
	}


	return 1;

}

int aiMmrnmhrm(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	s8 CanTransform;

	CanTransform = (ai->special_turn == 0	&& ai->batt >=
			ai->specbatt);

	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];

	ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

	ai->ship_input_state &= ~SPECIAL;
	if (CanTransform
			&& lpEvalDesc->parent
			&& !(ai->ship_input_state & WEAPON))
	{


		s16 direction_angle=ai->angle;
		s16 travel_angle=FindAngle(0,0,ai->xspeed,ai->yspeed);
		pPlayer opp=(pPlayer)ai->opp;

		if (ai->form==0)//in slow form
		{
			if (lpEvalDesc->which_turn > 8)
			{
				if (opp->ManeuverabilityIndex <= SLOW_SHIP
						|| TurnAngle(direction_angle,travel_angle,90)==0)
					ai->ship_input_state |= SPECIAL;
			}
		}
		else
		{

			if (lpEvalDesc->which_turn < 16)
			{
				if (lpEvalDesc->which_turn <= 8
						|| TurnAngle(direction_angle,travel_angle,90)!=0)
					ai->ship_input_state |= SPECIAL;
			}
			else if (lpEvalDesc->which_turn > 32
					&&	TurnAngle(direction_angle,travel_angle,180)!=0)
				ai->ship_input_state |= SPECIAL;
		}
	}

	if (ai->form==1)//fast
	{
		if (!(ai->ship_input_state & SPECIAL)
				&& lpEvalDesc->parent)
			ai->ship_input_state |= WEAPON;
		else
			ai->ship_input_state &= ~WEAPON;
	}

}

void SetMmrnmhrmPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |TALL  | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+112 | PRIORITY(1);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_64 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);
}

void MoveMmrnmhrmMissile(pWeapon ur)
{
	pPlayer target=(pPlayer)ur->target;

	if (DetectWeaponToShip(target,ur)==1)
			{
				ModifyCrew(target,ur->damage);
	//			ur->life==0;
				CreateExplosion(ur);
		}

	else if (ur->turn_wait > 0)
		--ur->turn_wait;
	else
	{
		s16 angle;

		ur->status--;

		if(ur->status==0)
		{
			ur->life=0;
			return;
		}


			angle = FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos);

			int ret=TurnAngle(angle,ur->angle,15);
			if (ret==0)
			{


				s32 x = ((MISSILE_SPEED) * (s32)SIN[ur->angle])>>8;
				s32 y = ((MISSILE_SPEED) * (s32)COS[ur->angle])>>8;


				ur->xspeed = x;//(ur->xspeed + x)/2;
				ur->yspeed = y;//(ur->yspeed + y)/2;
			}
			else if (ret<0)
			{
				ur->actualangle--;
				if (ur->actualangle==-1)
					ur->actualangle=15;
			}
			else if (ret>0)
			{
				ur->actualangle++;
				if (ur->actualangle==16)
					ur->actualangle=0;
			}
			ur->angle=(ur->actualangle*45)>>1;


		ur->turn_wait = TRACK_WAIT;
	}
	ur->xpos+=ur->xspeed;
	ur->ypos-=ur->yspeed;


}

void RestoreGFXMmrnmhrm(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==MISSILE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+128 | PRIORITY(1);
			}



		}
	}
}

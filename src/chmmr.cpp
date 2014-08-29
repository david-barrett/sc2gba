#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "chmmr_out.h"

#include "chmmr_lasers.h"
#include "chmmr_laserm.h"
#include "chmmr_laserd.h"

#include "chmmr_sat1.h"
#include "chmmr_sat2.h"
#include "chmmr_sat3.h"
#include "chmmr_satl.h"


#include "chmmr_sfx.h"

#include "chmmrpilot.h"
#include "chmmrpilotl.h"
#include "chmmrpilotr.h"
#include "chmmrpilott.h"
#include "chmmrpilotf.h"
#include "chmmrpilots.h"

#define MAX_CREW 42
#define MAX_ENERGY 42
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 2
#define SPECIAL_ENERGY_COST 1
#define ENERGY_WAIT 1
#define MAX_THRUST SHIP_SPEED(35)
#define THRUST_INCREMENT SHIP_SPEED(7)
#define TURN_WAIT 3
#define THRUST_WAIT 5
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 0

#define SHIP_MASS 10
#define LASER_RANGE 150 //DISPLAY_TO_WORLD (150)

#define SAT_DIST 50

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireChmmr(pPlayer pl);
int SpecialChmmr(pPlayer pl);
int aiChmmr(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetChmmrPilot(pPlayer p);
void RestoreGFXChmmr(pPlayer p);
void MoveSat(pWeapon ur);
void PostChmmr(pPlayer p);

int CreateActualOutline(s32 xpos,s32 ypos,s16 angle,pWeapon w,s32 SpriteStart,s8 life);

#define ps (1024*2)+512+32//2048 //2048//(1024*3)+512+128

void LoadChmmr(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = chmmrData[loop-OAMStart];
       		OAMData[loop+512] = chmmr_outData[loop-OAMStart];

			OAMData[loop+1024] = chmmr_lasermData[loop-OAMStart];       					
			OAMData[loop+1024+512] = chmmr_laserdData[loop-OAMStart];
   	}

	for(loop = OAMStart; loop < OAMStart+32; loop++)  
		OAMData[loop+2048] = chmmr_lasersData[loop-OAMStart];

   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+2048+32] = chmmr_sat1Data[loop-OAMStart];
		OAMData[loop+(1024*2)+128+32] = chmmr_sat2Data[loop-OAMStart];
		OAMData[loop+(1024*2)+256+32] = chmmr_sat3Data[loop-OAMStart];
		OAMData[loop+(1024*2)+256+128+32] = chmmr_satlData[loop-OAMStart];
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+ps] = chmmrpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+1024+ps] = chmmrpilotlData[loop-OAMStart];
		OAMData[loop+1024+ps+512] = chmmrpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+ps] = chmmrpilottData[loop-OAMStart];	
		OAMData[loop+(1024*2)+ps+256] = chmmrpilotfData[loop-OAMStart];	
		OAMData[loop+(1024*2)+ps+512] = chmmrpilotsData[loop-OAMStart];
	}
	
}

int SpecialChmmr(pPlayer pl)
{
	pPlayer opp=(pPlayer)pl->opp;
	s16 a = FindAngle(opp->xpos,opp->ypos,pl->xpos,pl->ypos);

	s16 magnitude = 12 / opp->mass;
	if (magnitude<3)
		magnitude=3;

	opp->xspeed+=(s32)(magnitude * (s32)SIN[a])>>8;
	opp->yspeed+=(s32)(magnitude * (s32)COS[a])>>8;

	play_sfx(&chmmr_tractor,pl->plr-1);
/*
	print("opp x");
	print(opp->xpos);
	print("opp y");
	print(opp->ypos);
*/
	

	s32 x,y,m;
	for (s16 b=5;b<8;b++)
	{
		if (b==5)
			m=magnitude*5;//(magnitude*3)>>2;
		else if (b==6)
			m=magnitude*10;//>>1;
		else
			m=magnitude*15;//>>2;
		x=opp->xpos+((s32)(m * (s32)SIN[a])>>8);
		y=opp->ypos-((s32)(m * (s32)COS[a])>>8);
		/*
		print("m x");
	print(x);
	print("m y");
	print(y);
*/
		CreateActualOutline(x,y,opp->angle,&pl->weapon[b],opp->SpriteStart,1);
	}


	return 1;
}

void SetChmmr(pPlayer pl)
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

	pl->offset=16;

	pl->fireangle=45;

	pl->firefunc=&FireChmmr;
	pl->specfunc=&SpecialChmmr;
	pl->aifunc=&aiChmmr;
	pl->loadfunc=&LoadChmmr;
	pl->loadpilots=&SetChmmrPilot;
	pl->postfunc=&PostChmmr;
	pl->restorefunc=&RestoreGFXChmmr;

	pl->ditty=&chmmr_ditty;

	pl->ship_flags = FIRES_FORE | IMMEDIATE_WEAPON | SEEKING_SPECIAL | POINT_DEFENSE;


	pl->pilot_sprite=(ps)/16;
	pl->pilots[0].x=3;
	pl->pilots[0].y=0;
	pl->pilots[1].x=0;
	pl->pilots[1].y=0;
	pl->pilots[2].x=30;
	pl->pilots[2].y=0;
	pl->pilots[3].x=14;
	pl->pilots[3].y=0;
	pl->pilots[4].x=40;
	pl->pilots[4].y=0;

//init sats
	for (int i=0;i<3;i++)
	{
		sprites[pl->weapon[i+9].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;
		sprites[pl->weapon[i+9].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[i+9].sprite) | 240;
		sprites[pl->weapon[i+9].sprite].attribute2 = pl->SpriteStart+130 | PRIORITY(1);
		pl->weapon[i+9].turn_wait=i;
		pl->weapon[i+9].life=5;
		pl->weapon[i+9].size=16;
		pl->weapon[i+9].type=SAT;
		pl->weapon[i+9].movefunc=&MoveSat;
		pl->weapon[i+9].damage=0;
		pl->weapon[i+9].target=pl->opp;
		pl->weapon[i+9].parent=pl;
		pl->weapon[i+9].damageparent=0;		
		pl->weapon[i+9].angle =0;
		pl->weapon[i+9].actualangle =(i*5);
		pl->weapon[i+9].fire_wait=0;

	}

}

int FireChmmr(pPlayer pl)
{


	for (int b=0;b<4;b++)
	{


	pl->weapon[b].type=LASER;
	pl->weapon[b].life=1;
	pl->weapon[b].damage=-2;//GUESS
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=32;
	pl->weapon[b].angle = pl->angle;

	s32 off=(b*31)+25;

	pl->weapon[b].xspeed=0;
	pl->weapon[b].yspeed=0;


	pl->weapon[b].xpos = pl->xpos+((off * (s32)SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((off * (s32)COS[pl->angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
	

	}
	play_sfx(&chmmr_laser,pl->plr-1);

	return 1;
}

int aiChmmr(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

	pPlayer target=(pPlayer)ai->opp;

	
	ai->ship_input_state &= ~SPECIAL;
	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	if (ai->special_turn == 0
			&& lpEvalDesc->parent
			&& !(ai->ship_input_state & WEAPON)
			&& lpEvalDesc->which_turn > 12
			&& (TurnAngle(FindAngle(0,0,ai->xspeed,ai->yspeed),
				FindAngle(0,0,target->xspeed,target->yspeed),90)!=0))
			ai->ship_input_state |= SPECIAL;



	

}

void SetChmmrPilot(pPlayer pl)
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

void MoveSat(pWeapon ur)
{
	s16 angle=(ur->actualangle*45)>>1;

	pPlayer target=(pPlayer)ur->target;

	pPlayer parent=(pPlayer)ur->parent;
	ur->xpos=parent->xpos+((s32)(SAT_DIST * SIN[angle])>>8);
	ur->ypos=parent->ypos-((s32)(SAT_DIST * COS[angle])>>8);
	ur->angle=0;

	ur->turn_wait++;
	if (ur->turn_wait==9)
		ur->turn_wait=0;

	s16 sprite;

	if (ur->turn_wait<3)
		sprite=parent->SpriteStart+130;
	else if (ur->turn_wait<6)
		sprite=parent->SpriteStart+138;
	else
		sprite=parent->SpriteStart+146;

	sprites[ur->sprite].attribute2 = sprite | PRIORITY(0);
	sprites[ur->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
	sprites[ur->sprite].attribute1 = SIZE_16 | ROTDATA(ur->sprite) | 240;

	if (ur->fire_wait==0)
	{
		s16 d = distanceBetweenPoints(target->xpos,target->ypos,ur->xpos,ur->ypos);
		if (d<target->offset+20)
		{
			s16 b=nextWeapon(parent);
		
			if (b>=0)
			{
				ur->fire_wait=5;
				parent->weapon[b].type=LASER;
				parent->weapon[b].life=1;
				parent->weapon[b].damage=0;//GUESS
				parent->weapon[b].target=parent->opp;
				parent->weapon[b].parent=parent;
				parent->weapon[b].damageparent=0;

				parent->weapon[b].size=16;
				parent->weapon[b].angle = FindAngle(target->xpos,target->ypos,ur->xpos,ur->ypos);

				

				parent->weapon[b].xspeed=0;
				parent->weapon[b].yspeed=0;


				parent->weapon[b].xpos = (target->xpos+ur->xpos)>>1;
				parent->weapon[b].ypos = (target->ypos+ur->ypos)>>1;

				drawOnScreen(&parent->weapon[b].xscreen,&parent->weapon[b].yscreen,
					parent->weapon[b].xpos,parent->weapon[b].ypos,screenx,screeny,parent->weapon[b].size);

				sprites[parent->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | parent->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
				sprites[parent->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(parent->weapon[b].sprite) | parent->weapon[b].xscreen;
				sprites[parent->weapon[b].sprite].attribute2 = parent->SpriteStart+154 | PRIORITY(1);

				ModifyCrew(target,-1);
			}

		}
	}
	else
		ur->fire_wait--;



	/*
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
		play_sfx(&chmmr_bite,parent->plr-1);
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
*/
}

void RestoreGFXChmmr(pPlayer p)
{
	
	for(int i=8;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SAT)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+130 | PRIORITY(1);
			}
		}
	}
}

void PostChmmr(pPlayer p)
{
	for (int i=9;i<12;i++)
	{
		p->weapon[i].actualangle++;
		if (p->weapon[i].actualangle==16)
			p->weapon[i].actualangle=0;
	}
}

#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "mycon_out.h"
#include "mycon_fire1.h"
#include "mycon_fire2.h"
#include "mycon_fire3.h"


#include "mycon_sfx.h"

#include "myconpilot.h"
#include "myconpilotl.h"
#include "myconpilotr.h"
#include "myconpilott.h"
#include "myconpilotf.h"
#include "myconpilots.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;
extern pAsteroid asteroids;

int FireMycon(pPlayer pl);
int SpecialMycon(pPlayer pl);
int aiMycon(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetMyconPilot(pPlayer p);
void RestoreGFXMycon(pPlayer p);
void MovePlasma(pWeapon ur);
void Postmycon(pPlayer pl);

#define MAX_CREW 20
#define MAX_ENERGY 40
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 20
#define SPECIAL_ENERGY_COST MAX_ENERGY
#define ENERGY_WAIT 4
#define MAX_THRUST /* DISPLAY_TO_WORLD (7) */ SHIP_SPEED(27)
#define THRUST_INCREMENT /* DISPLAY_TO_WORLD (2) */ SHIP_SPEED(9)
#define TURN_WAIT 6
#define THRUST_WAIT 6
#define WEAPON_WAIT 5
#define SPECIAL_WAIT 0

#define SHIP_MASS 7

#define NUM_PLASMAS 11
#define NUM_GLOBALLS 8
#define PLASMA_DURATION 13
#define MISSILE_LIFE (NUM_PLASMAS * PLASMA_DURATION)
#define MISSILE_SPEED 8
#define MISSILE_DAMAGE 10
#define MISSILE_RANGE  MISSILE_SPEED*MISSILE_LIFE

#define TRACK_WAIT 1

#define REGENERATION_AMOUNT 4

void LoadMycon(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = myconData[loop-OAMStart];
       		OAMData[loop+512] = mycon_outData[loop-OAMStart];
	}
	for(loop = OAMStart; loop < OAMStart+128; loop++)
       		OAMData[loop+512+512] = mycon_fire1Data[loop-OAMStart];
    for(loop = OAMStart; loop < OAMStart+512; loop++)
    {
       		OAMData[loop+512+512+128] = mycon_fire2Data[loop-OAMStart];
       		OAMData[loop+512+512+512+128] = mycon_fire3Data[loop-OAMStart];
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+2048+128] = myconpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+128] = myconpilotlData[loop-OAMStart];
		OAMData[loop+(1024*3)+256+128] = myconpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*3)+512+128] = myconpilottData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
		OAMData[loop+(1024*4)+128] = myconpilotfData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*5)+128] = myconpilotsData[loop-OAMStart];
	}


}

int FireMycon(pPlayer pl)
{

	s16 b= nextWeapon(pl);
	int ret=0;

	if (b>=0)
	{
		pl->weapon[b].type=MISSILE;
		pl->weapon[b].movefunc=&MovePlasma;
		pl->weapon[b].object.life=3;
		pl->weapon[b].status=MISSILE_LIFE;
		pl->weapon[b].damage=MISSILE_DAMAGE*-1;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=1;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=0;

		pl->weapon[b].object.size=16;
		pl->weapon[b].object.actualangle = pl->object.actualangle;
		pl->weapon[b].object.angle=(pl->weapon[b].object.actualangle*45)>>1;

		pl->weapon[b].object.xspeed = (MISSILE_SPEED * (s32)SIN[pl->object.angle])>>8;
		pl->weapon[b].object.yspeed = (MISSILE_SPEED * (s32)COS[pl->object.angle])>>8;


		pl->weapon[b].turn_wait=0;


		pl->weapon[b].object.xpos = pl->object.xpos+((s32)(pl->offset*2 * SIN[pl->weapon[b].object.angle])>>8);
		pl->weapon[b].object.ypos = pl->object.ypos-((s32)(pl->offset*2 * COS[pl->weapon[b].object.angle])>>8);

		drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
			pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,pl->weapon[b].object.size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 =SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
		ret=1;
		play_sfx(&mycon_plasmoid,pl->plr-1);
	}



	return ret;
}

void SetMycon(pPlayer pl)
{
	pl->crew=MAX_CREW;
	pl->maxcrew=MAX_CREW;
	pl->batt=MAX_ENERGY;
	pl->maxbatt=MAX_ENERGY;

	pl->maxspeed=MAX_THRUST;

	pl->accel_inc=THRUST_INCREMENT;

	pl->firebatt=WEAPON_ENERGY_COST;
	pl->specbatt=SPECIAL_ENERGY_COST;

	pl->offset=14;

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

	pl->firefunc=&FireMycon;
	pl->specfunc=&SpecialMycon;
	pl->aifunc=&aiMycon;
	pl->loadfunc=&LoadMycon;
	pl->loadpilots=&SetMyconPilot;
	pl->postfunc=&Postmycon;
	pl->restorefunc=&RestoreGFXMycon;

	pl->ditty=&mycon_ditty;

	pl->ship_flags = FIRES_FORE | SEEKING_WEAPON ;
	pl->object.mass_points=SHIP_MASS;

	pl->pilot_sprite=(2048+128)/16;
	pl->pilots[0].x=511;
	pl->pilots[0].y=15;
	pl->pilots[1].x=31;
	pl->pilots[1].y=18;
	pl->pilots[2].x=18;
	pl->pilots[2].y=1;
	pl->pilots[3].x=0;
	pl->pilots[3].y=0;
	pl->pilots[4].x=23 ; //temp remove these -
	pl->pilots[4].y=5;  //run out of gfx!!


}



int SpecialMycon(pPlayer pl)
{
	ModifyCrew(pl,REGENERATION_AMOUNT);

	play_sfx(&mycon_regrow,pl->plr-1);
	return 1;

}

int aiMycon(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
		if (lpEvalDesc->parent && lpEvalDesc->MoveState == ENTICE)
		{
			if (lpEvalDesc->type!=CREW)
				lpEvalDesc->MoveState = AVOID;
			else
				lpEvalDesc->MoveState = PURSUE;
		}

		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);


		if (ObjectsOfConcern[ENEMY_WEAPON_INDEX].MoveState == PURSUE)
			ai->ship_input_state &= ~THRUST; /* don't pursue seekers */

		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		if (ai->weapon_turn == 0
				&& lpEvalDesc->parent
				&& (lpEvalDesc->which_turn <= 16
				|| ai->crew == ai->maxcrew))
		{


			if (((TurnAngle(FindAngle(0,0,ai->object.xspeed,ai->object.yspeed),ai->object.angle,90)==0)
					&& (ai->speed<ai->maxspeed))
					|| TurnAngle(FindAngle(0,0,ai->object.xspeed,ai->object.yspeed),ai->object.angle,180)!=0)
				ai->ship_input_state |= WEAPON;
		}

		if (ai->special_turn == 0)
		{
			ai->ship_input_state &= ~SPECIAL;
			ai->range = 800;
			if (ai->crew < ai->maxcrew)
			{
				ai->range = MISSILE_SPEED * MISSILE_LIFE;
				if (ai->batt >= SPECIAL_ENERGY_COST
						&& !(ai->ship_input_state & WEAPON))
					ai->ship_input_state |= SPECIAL;
			}
	}



}

void SetMyconPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 =SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | TALL | 160;
	sprites[44+off].attribute1 =SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[45+off].attribute1 =SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |TALL  | 160;
	sprites[46+off].attribute1 =SIZE_64 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(1);

	sprites[47+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[47+off].attribute1 =SIZE_16 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+192 | PRIORITY(1);
}

void  MovePlasma(pWeapon ur)
{
	pPlayer target=(pPlayer)ur->target;
	pPlayer pl =(pPlayer)ur->parent;

	if (ur->object.life==1&&ur->status>33)
	{
		ur->status==33;
	}
	else
	if (ur->object.life==2&&ur->status>111)
	{
			ur->status==111;
	}



	if (ur->status==33)
	{
		sprites[ur->sprite].attribute1 =SIZE_32 | ROTDATA(ur->sprite) | ur->object.xscreen;
		sprites[ur->sprite].attribute2 = pl->SpriteStart+104 | PRIORITY(1);
		ur->object.life=1;
		ur->object.size=32;
	}
	else if (ur->status==111)
	{
		ur->object.size=32;
		sprites[ur->sprite].attribute1 =SIZE_32 | ROTDATA(ur->sprite) | ur->object.xscreen;
		sprites[ur->sprite].attribute2 = pl->SpriteStart+72 | PRIORITY(1);
		ur->object.life=2;

	}

	ur->damage = (-1*(ur->status *	MISSILE_DAMAGE + (MISSILE_LIFE - 1)) / MISSILE_LIFE);



		if (DetectWeaponToShip(target,ur)==1)
				{
					ModifyCrew(target,ur->damage);
		//			ur->object.life==0;
					CreateExplosion(ur);
			}

		if (DetectWeaponToShip(pl,ur)==1)
			{
							ModifyCrew(pl,ur->damage);
				//			ur->object.life==0;
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
				ur->object.life=0;
				return;
			}


			angle = FindAngle(ur->object.xpos,ur->object.ypos,target->object.xpos,target->object.ypos);

				int ret=TurnAngle(angle,ur->object.angle,15);
				if (ret==0)
				{


					s32 x = ((MISSILE_SPEED) * (s32)SIN[ur->object.angle])>>8;
					s32 y = ((MISSILE_SPEED) * (s32)COS[ur->object.angle])>>8;


					ur->object.xspeed = x;//(ur->object.xspeed + x)/2;
					ur->object.yspeed = y;//(ur->object.yspeed + y)/2;
				}
				else if (ret<0)
								{
									ur->object.actualangle--;
									if (ur->object.actualangle==-1)
										ur->object.actualangle=15;
								}
								else if (ret>0)
								{
									ur->object.actualangle++;
									if (ur->object.actualangle==16)
										ur->object.actualangle=0;
								}
				ur->object.angle=(ur->object.actualangle*45)>>1;


			ur->turn_wait = TRACK_WAIT;
		}
	
}

void RestoreGFXMycon(pPlayer p)
{
	//only restore limpets
	//im sure laser will be fine...
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].object.life>0)
		{
			if(p->weapon[i].type==MISSILE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 =SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}



		}
	}
}
void Postmycon(pPlayer pl)
{

}

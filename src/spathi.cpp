#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "spathi_out.h"
#include "spathi_butt.h"
#include "spathi_fire.h"


#include "spathi_sfx.h"

#include "spathipilot.h"
#include "spathipilotl.h"
#include "spathipilotr.h"
#include "spathipilott.h"
#include "spathipilotf.h"
#include "spathipilots.h"

#define MAX_CREW 30
#define MAX_ENERGY 10
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 2
#define SPECIAL_ENERGY_COST 3
#define ENERGY_WAIT 10
#define MAX_THRUST SHIP_SPEED(48)
#define THRUST_INCREMENT SHIP_SPEED(12)
#define TURN_WAIT 1
#define THRUST_WAIT 1
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 7

#define SHIP_MASS 5
#define MISSILE_SPEED /*DISPLAY_TO_WORLD*/ (30)
#define MISSILE_LIFE 10
#define MISSILE_RANGE (MISSILE_SPEED * MISSILE_LIFE)

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireSpathi(pPlayer pl);
int SpecialSpathi(pPlayer pl);
int aiSpathi(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetSpathiPilot(pPlayer p);
void RestoreGFXSpathi(pPlayer p);
void MoveButt(pWeapon ur);

#define DISCRIMINATOR_SPEED 8
#define TRACK_WAIT 1


void LoadSpathi(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = spathiData[loop-OAMStart];
       		OAMData[loop+512] = spathi_outData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512] = spathi_fireData[loop-OAMStart]; //loads some garb
		OAMData[loop+512+512+32] = spathi_buttData[loop-OAMStart]; //loads some garb
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+64] = spathipilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+64] = spathipilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512] = spathipilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+64] = spathipilottData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+256] = spathipilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+512] = spathipilotsData[loop-OAMStart];

	}

}

int SpecialSpathi(pPlayer pl)
{

	s16 b= nextWeapon(pl);
	int ret=0;

	if (b>=0)
	{
		pl->weapon[b].type=BUTT;
		pl->weapon[b].object.life=1;
		pl->weapon[b].status=30;
		pl->weapon[b].damage=-2;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;
		pl->weapon[b].movefunc=&MoveButt;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=0;

		pl->weapon[b].object.size=8;
		pl->weapon[b].object.angle = ModifyAngle(pl->object.angle,180);
		pl->weapon[b].object.actualangle = pl->object.actualangle+8;
		if (pl->weapon[b].object.actualangle>15)
			pl->weapon[b].object.actualangle-=16;

		pl->weapon[b].object.angle=(pl->weapon[b].object.actualangle*45)>>1;

		pl->weapon[b].object.xspeed = ((DISCRIMINATOR_SPEED) * (s32)SIN[pl->weapon[b].object.angle])>>8;
		pl->weapon[b].object.yspeed = ((DISCRIMINATOR_SPEED) * (s32)COS[pl->weapon[b].object.angle])>>8;


		pl->weapon[b].turn_wait=0;


		pl->weapon[b].object.xpos = pl->object.xpos+((s32)(pl->offset*2 * SIN[pl->weapon[b].object.angle])>>8);
		pl->weapon[b].object.ypos = pl->object.ypos-((s32)(pl->offset*2 * COS[pl->weapon[b].object.angle])>>8);

		#ifdef MISSILE_START
		pl->weapon[b].object.xpos-=pl->weapon[b].object.xspeed;
		pl->weapon[b].object.ypos+=pl->weapon[b].object.yspeed;
		#endif

		drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
			pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,pl->weapon[b].object.size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 =SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+66 | PRIORITY(1);
		ret=1;
		play_sfx(&spathi_butt,pl->plr-1);
	}



	return ret;
}

void SetSpathi(pPlayer pl)
{
	pl->crew=30;
	pl->maxcrew=30;
	pl->batt=10;
	pl->maxbatt=10;

	pl->maxspeed=48;

	pl->accel_inc=12;

	pl->firebatt=2;
	pl->specbatt=3;

	pl->offset=14;

	pl->batt_wait=10;
	pl->turn_wait=1;
	pl->thrust_wait=1;
	pl->weapon_wait=0;
	pl->special_wait=7;
	pl->batt_regen=1;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=MISSILE_RANGE;

	pl->firefunc=&FireSpathi;
	pl->specfunc=&SpecialSpathi;
	pl->aifunc=&aiSpathi;
	pl->loadfunc=&LoadSpathi;
	pl->loadpilots=&SetSpathiPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXSpathi;

	pl->ditty=&spathi_ditty;

	pl->ship_flags = FIRES_FORE | FIRES_AFT | SEEKING_SPECIAL | DONT_CHASE;
	pl->object.mass_points=5;

	pl->pilot_sprite=(1024+64)/16;
	pl->pilots[0].x=27;
	pl->pilots[0].y=0;
	pl->pilots[1].x=28;
	pl->pilots[1].y=0;
	pl->pilots[2].x=22;
	pl->pilots[2].y=0;
	pl->pilots[3].x=3;
	pl->pilots[3].y=0;
	pl->pilots[4].x=13;
	pl->pilots[4].y=1;


}

int FireSpathi(pPlayer pl)
{


	s16 b= nextWeapon(pl);//,0,4);

		if (b>=0)
		{

		pl->weapon[b].type=SIMPLE;
		pl->weapon[b].object.life=10;
		pl->weapon[b].damage=-2;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;
		pl->weapon[b].movefunc=0;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=0;
		
		pl->weapon[b].object.size=8;
		pl->weapon[b].object.angle = pl->object.angle;

		s32 speed=30;//20;
		pl->weapon[b].object.xspeed = ((speed * (s32)SIN[pl->object.angle])>>9);///SPEED_REDUCT;
		pl->weapon[b].object.yspeed = ((speed * (s32)COS[pl->object.angle])>>9);///SPEED_REDUCT;

		pl->weapon[b].object.xpos = pl->object.xpos+((40 * (s32)SIN[pl->object.angle])>>8)/3;
		pl->weapon[b].object.ypos = pl->object.ypos-((40 * (s32)COS[pl->object.angle])>>8)/3;

		#ifdef MISSILE_START
		pl->weapon[b].object.xpos-=pl->weapon[b].object.xspeed;
		pl->weapon[b].object.ypos+=pl->weapon[b].object.yspeed;
		#endif

		drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
			pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,pl->weapon[b].object.size);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
	  	sprites[pl->weapon[b].sprite].attribute1 =SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);

		play_sfx(&spathi_bullet,pl->plr-1);
		return 1;
		}
	return 0;
}

int aiSpathi(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;


	ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

	ai->ship_input_state &= ~SPECIAL;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	if (ai->special_turn == 0
			&& lpEvalDesc->parent
			&& lpEvalDesc->which_turn <= 24)
	{
		/*
		COUNT travel_facing, direction_facing;
	.object.size delta_x, delta_y;

		travel_facing = FindAngle(0,0,ai->object.xspeed,ai->object.yspeed);

		delta_x = lpEvalDesc->tr->current.location.x
				- ShipPtr->current.location.x;
		delta_y = lpEvalDesc->tr->current.location.y
				- ShipPtr->current.location.y;
		direction_facing = NORMALIZE_FACING (
			.object.angle_TO_FACING (ARCTAN (delta_x, delta_y))
				);

		if (NORMALIZE_FACING (direction_facing
				- (StarShipPtr->ShipFacing +.object.angle_TO_FACING (HALF_CIRCLE))
				+.object.angle_TO_FACING (QUADRANT))
				<=.object.angle_TO_FACING (HALF_CIRCLE)
				&& (lpEvalDesc->which_turn <= 8
				|| NORMALIZE_FACING (direction_facing
				+.object.angle_TO_FACING (HALF_CIRCLE)
				->object.angle_TO_FACING (GetVelocityTrave.object.angle (
						&lpEvalDesc->tr->velocity
						))
				+.object.angle_TO_FACING (QUADRANT))
				<=.object.angle_TO_FACING (HALF_CIRCLE))
				&& (!(StarShipPtr->cur_status_flags &
				(SHIP_BEYOND_MAX_SPEED | SHIP_IN_GRAVITY_WELL))
				|| NORMALIZE_FACING (direction_facing
				- travel_facing +.object.angle_TO_FACING (QUADRANT))
				<=.object.angle_TO_FACING (HALF_CIRCLE)))
			StarShipPtr->ship_input_state |= SPECIAL;
		*/

		/* MY way if enemy behind fire butt */
		pPlayer opp=(pPlayer)ai->opp;
		s16 angle=FindAngle(ai->object.xpos,ai->object.ypos,opp->object.xpos,opp->object.ypos);
		if (!(TurnAngle(angle,ai->object.angle,180)))
			ai->ship_input_state |= SPECIAL;
	}

}

void SetSpathiPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[43+off].attribute1 =SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 =SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[45+off].attribute1 =SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |WIDE  | 160;
	sprites[46+off].attribute1 =SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+16 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 =SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+32 | PRIORITY(2);
}

void MoveButt(pWeapon ur)
{
	pPlayer target=(pPlayer)ur->target;

	if (ur->turn_wait > 0)
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


				s32 x = ((DISCRIMINATOR_SPEED) * (s32)SIN[ur->object.angle])>>8;
				s32 y = ((DISCRIMINATOR_SPEED) * (s32)COS[ur->object.angle])>>8;


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
	ur->object.xpos+=ur->object.xspeed;
	ur->object.ypos-=ur->object.yspeed;


}

void RestoreGFXSpathi(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].object.life>0)
		{
			if(p->weapon[i].type==BUTT)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 =SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+66 | PRIORITY(1);
			}
			else if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 =SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}


		}
	}
}

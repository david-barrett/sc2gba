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
		pl->weapon[b].life=1;
		pl->weapon[b].status=30;
		pl->weapon[b].damage=-2;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=8;
		pl->weapon[b].angle = ModifyAngle(pl->angle,180);

		pl->weapon[b].xspeed = ((DISCRIMINATOR_SPEED) * (s32)SIN[pl->weapon[b].angle])>>8;
		pl->weapon[b].yspeed = ((DISCRIMINATOR_SPEED) * (s32)COS[pl->weapon[b].angle])>>8;


		pl->weapon[b].turn_wait=0;


		pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset*2 * SIN[pl->weapon[b].angle])>>8);
		pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset*2 * COS[pl->weapon[b].angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
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

	pl->range=300;

	pl->fireangle=45;

	pl->firefunc=&FireSpathi;
	pl->specfunc=&SpecialSpathi;
	pl->aifunc=&aiSpathi;
	pl->loadfunc=&LoadSpathi;
	pl->loadpilots=&SetSpathiPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXSpathi;

	pl->ditty=&spathi_ditty;

	pl->ship_flags = FIRES_FORE | FIRES_AFT | SEEKING_SPECIAL | DONT_CHASE;
	pl->mass=5;

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
		pl->weapon[b].life=10;
		pl->weapon[b].damage=-2;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=8;
		pl->weapon[b].angle = pl->angle;

		s32 speed=30;//20;
		pl->weapon[b].xspeed = ((speed * (s32)SIN[pl->angle])>>9);///SPEED_REDUCT;
		pl->weapon[b].yspeed = ((speed * (s32)COS[pl->angle])>>9);///SPEED_REDUCT;

		pl->weapon[b].xpos = pl->xpos+((40 * (s32)SIN[pl->angle])>>8)/3;
		pl->weapon[b].ypos = pl->ypos-((40 * (s32)COS[pl->angle])>>8)/3;

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	  	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
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
		SIZE delta_x, delta_y;

		travel_facing = FindAngle(0,0,ai->xspeed,ai->yspeed);

		delta_x = lpEvalDesc->ObjectPtr->current.location.x
				- ShipPtr->current.location.x;
		delta_y = lpEvalDesc->ObjectPtr->current.location.y
				- ShipPtr->current.location.y;
		direction_facing = NORMALIZE_FACING (
				ANGLE_TO_FACING (ARCTAN (delta_x, delta_y))
				);

		if (NORMALIZE_FACING (direction_facing
				- (StarShipPtr->ShipFacing + ANGLE_TO_FACING (HALF_CIRCLE))
				+ ANGLE_TO_FACING (QUADRANT))
				<= ANGLE_TO_FACING (HALF_CIRCLE)
				&& (lpEvalDesc->which_turn <= 8
				|| NORMALIZE_FACING (direction_facing
				+ ANGLE_TO_FACING (HALF_CIRCLE)
				- ANGLE_TO_FACING (GetVelocityTravelAngle (
						&lpEvalDesc->ObjectPtr->velocity
						))
				+ ANGLE_TO_FACING (QUADRANT))
				<= ANGLE_TO_FACING (HALF_CIRCLE))
				&& (!(StarShipPtr->cur_status_flags &
				(SHIP_BEYOND_MAX_SPEED | SHIP_IN_GRAVITY_WELL))
				|| NORMALIZE_FACING (direction_facing
				- travel_facing + ANGLE_TO_FACING (QUADRANT))
				<= ANGLE_TO_FACING (HALF_CIRCLE)))
			StarShipPtr->ship_input_state |= SPECIAL;
		*/

		/* MY way if enemy behind fire butt */
		pPlayer opp=(pPlayer)ai->opp;
		s16 angle=FindAngle(ai->xpos,ai->ypos,opp->xpos,opp->ypos);
		if (!(TurnAngle(angle,ai->angle,180)))
			ai->ship_input_state |= SPECIAL;
	}

}

void SetSpathiPilot(pPlayer pl)
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

void MoveButt(pWeapon ur)
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


				s32 x = ((DISCRIMINATOR_SPEED) * (s32)SIN[ur->angle])>>8;
				s32 y = ((DISCRIMINATOR_SPEED) * (s32)COS[ur->angle])>>8;


				ur->xspeed = x;//(ur->xspeed + x)/2;
				ur->yspeed = y;//(ur->yspeed + y)/2;
			}
			else if (ret<0)
			{
				ur->angle-=15;
				if (ur->angle<0)
					ur->angle+=360;
			}
			else if (ret>0)
			{
				ur->angle+=15;
				if (ur->angle>360)
					ur->angle-=360;
			}


		ur->turn_wait = TRACK_WAIT;
	}
	ur->xpos+=ur->xspeed;
	ur->ypos-=ur->yspeed;


}

void RestoreGFXSpathi(pPlayer p)
{
	//only restore limpets
	//im sure laser will be fine...
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==BUTT)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+66 | PRIORITY(1);
			}
			else if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}


		}
	}
}

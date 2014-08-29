#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "androsyn_out.h"
#include "andro_fireball.h"
#include "andro_bubbles.h"
#include "andro_bubblespop.h"


#include "androsynth_sfx.h"

#include "andropilot.h"
#include "andropilotl.h"
#include "andropilotr.h"
#include "andropilott.h"
#include "andropilotf.h"
#include "andropilots.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireAndrosynth(pPlayer pl);
int SpecialAndrosynth(pPlayer pl);
int aiAndrosynth(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetAndrosynthPilot(pPlayer p);
void RestoreGFXAndrosynth(pPlayer p);
void MoveBubbles(pWeapon ur);
void PostAndro(pPlayer pl);

#define MAX_CREW 20
#define MAX_ENERGY 24
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 3
#define SPECIAL_ENERGY_COST 2
#define ENERGY_WAIT 8
#define MAX_THRUST SHIP_SPEED (24)
#define THRUST_INCREMENT SHIP_SPEED(3)
#define TURN_WAIT 4
#define THRUST_WAIT 0
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 0

#define SHIP_MASS 6

#define MISSILE_SPEED 8

#define TRACK_WAIT 2
#define MISSILE_DAMAGE 2
#define MISSILE_LIFE 200
#define BLAZER_THRUST SHIP_SPEED(60)
#define BLAZER_DAMAGE 3
#define BLAZER_MASS 1
#define BLAZER_TURN_WAIT 0

void LoadAndrosynth(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = androsynData[loop-OAMStart];
       		OAMData[loop+512] = androsyn_outData[loop-OAMStart];
       		OAMData[loop+512+512] = andro_fireballData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512+512] = andro_bubblesData[loop-OAMStart]; //loads some garb
		OAMData[loop+512+512+32+512] = andro_bubblespopData[loop-OAMStart]; //loads some garb
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+64+512] = andropilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*2)+64+512] = andropilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512+128] = andropilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+64;loop++)
	{
		OAMData[loop+(1024*2)+64+512+256] = andropilottData[loop-OAMStart];
		OAMData[loop+(1024*2)+128+512+256] = andropilotfData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512+256+128] = andropilotsData[loop-OAMStart];

	}

}

int FireAndrosynth(pPlayer pl)
{

	s16 b= nextWeapon(pl);
	int ret=0;

	if (b>=0)
	{
		pl->weapon[b].type=BUBBLE;
		pl->weapon[b].movefunc=&MoveBubbles;
		pl->weapon[b].life=3;
		pl->weapon[b].status=MISSILE_LIFE;
		pl->weapon[b].damage=MISSILE_DAMAGE*-1;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=8;
		pl->weapon[b].angle = 0;

		pl->weapon[b].xspeed = (MISSILE_SPEED * (s32)SIN[pl->angle])>>8;
		pl->weapon[b].yspeed = (MISSILE_SPEED * (s32)COS[pl->angle])>>8;


		pl->weapon[b].turn_wait=0;


		pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset*2 * SIN[pl->angle])>>8);
		pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset*2 * COS[pl->angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);
		ret=1;
		play_sfx(&Androsynth_Bubble,pl->plr-1);
	}



	return ret;
}

void SetAndrosynth(pPlayer pl)
{
	pl->crew=MAX_CREW;
	pl->maxcrew=MAX_CREW;
	pl->batt=MAX_ENERGY;
	pl->maxbatt=MAX_ENERGY;

	pl->maxspeed=MAX_THRUST;

	pl->accel_inc=THRUST_INCREMENT;

	pl->firebatt=WEAPON_ENERGY_COST;
	pl->specbatt=SPECIAL_ENERGY_COST;

	pl->offset=10;

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

	pl->range=MISSILE_SPEED*MISSILE_LIFE;

	pl->firefunc=&FireAndrosynth;
	pl->specfunc=&SpecialAndrosynth;
	pl->aifunc=&aiAndrosynth;
	pl->loadfunc=&LoadAndrosynth;
	pl->loadpilots=&SetAndrosynthPilot;
	pl->postfunc=&PostAndro;
	pl->restorefunc=&RestoreGFXAndrosynth;

	pl->ditty=&androsynth_ditty;

	pl->ship_flags = FIRES_FORE | SEEKING_WEAPON;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+64+512)/16;
	pl->pilots[0].x=16;
	pl->pilots[0].y=16;
	pl->pilots[1].x=15;
	pl->pilots[1].y=15;
	pl->pilots[2].x=10;
	pl->pilots[2].y=0;
	pl->pilots[3].x=36;
	pl->pilots[3].y=4;
	pl->pilots[4].x=51;
	pl->pilots[4].y=6;


}

int SpecialAndrosynth(pPlayer pl)
{
	pl->turn_wait=BLAZER_TURN_WAIT;

	sprites[(pl->plr==1)?0:13].attribute2 = pl->SpriteStart+64 | PRIORITY(1);

	pl->blaze=1;
	pl->mass=BLAZER_MASS;
	pl->batt_turn=ENERGY_WAIT;

	play_sfx(&Androsynth_Blaze,pl->plr-1);
	return 1;

}

int aiAndrosynth(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;



	lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
				/* in blazer form */
	if (ai->blaze==1)
	{
		ObjectsOfConcern[CREW_OBJECT_INDEX].parent = 0;
		if (lpEvalDesc->parent && lpEvalDesc->MoveState == ENTICE)
		{
			if (lpEvalDesc->type!=CREW)
				lpEvalDesc->MoveState = AVOID;
			else
				lpEvalDesc->parent = 0;
		}

		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);
	}
	else
	{
		pPlayer pEnemyStarShip=(pPlayer)ai->opp;

		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		if (lpEvalDesc->parent)
		{
				if (lpEvalDesc->which_turn <= 16
					&& (ai->special_turn > 0
					|| ai->batt < MAX_ENERGY / 3
					|| ((pEnemyStarShip->range) <= CLOSE_RANGE_WEAPON
					&& pEnemyStarShip->crew > BLAZER_DAMAGE)
					|| (pEnemyStarShip->crew > (BLAZER_DAMAGE * 3)
					&& pEnemyStarShip->ManeuverabilityIndex  > SLOW_SHIP)))
				lpEvalDesc->MoveState = ENTICE;
		}

		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

		if (ai->special_turn == 0)
		{
			ai->ship_input_state &= ~SPECIAL;
			if ((ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent
					&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn <= 4)
					|| (lpEvalDesc->parent
					&& ai->batt >= MAX_ENERGY / 3
					&& (pEnemyStarShip->range >= ai->range << 1
					|| (lpEvalDesc->which_turn < 16
					&& (pEnemyStarShip->range > CLOSE_RANGE_WEAPON
					|| pEnemyStarShip->crew 	<= BLAZER_DAMAGE)
					&& (pEnemyStarShip->crew   <= (s16)(BLAZER_DAMAGE * 3)
					|| pEnemyStarShip->ManeuverabilityIndex <=
					SLOW_SHIP)))))
				ai->ship_input_state |= SPECIAL;
		}

		if (!(ai->ship_input_state & SPECIAL)
				&& ai->weapon_turn == 0
				&& lpEvalDesc->parent)
		{
			if (lpEvalDesc->which_turn <= 4)
				ai->ship_input_state |= WEAPON;
			else if (lpEvalDesc->MoveState != PURSUE
					&& lpEvalDesc->which_turn <= 12)
			{
				/*
				COUNT travel_facing, direction_facing;
				SIZE delta_x, delta_y,
							ship_delta_x, ship_delta_y,
							other_delta_x, other_delta_y;

				GetCurrentVelocityComponents (&ShipPtr->velocity,
						&ship_delta_x, &ship_delta_y);
				GetCurrentVelocityComponents (&lpEvalDesc->ObjectPtr->velocity,
						&other_delta_x, &other_delta_y);
				delta_x = ship_delta_x - other_delta_x;
				delta_y = ship_delta_y - other_delta_y;
				travel_facing = ARCTAN (delta_x, delta_y);

				delta_x =
						lpEvalDesc->ObjectPtr->next.location.x -
						ShipPtr->next.location.x;
				delta_y =
						lpEvalDesc->ObjectPtr->next.location.y -
						ShipPtr->next.location.y;
				direction_facing = ARCTAN (delta_x, delta_y);

				if (NORMALIZE_ANGLE (travel_facing
						- direction_facing + OCTANT) <= QUADRANT)*/
				s16 travel_angle = FindAngle(0,0,ai->xspeed,ai->yspeed);
				if (TurnAngle(travel_angle,ai->angle,90)==0)
					ai->ship_input_state |= WEAPON;
			}
		}
	}
}

void SetAndrosynthPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[43+off].attribute1 = SIZE_16 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_16 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+72 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | WIDE | 160;
	sprites[45+off].attribute1 = SIZE_8 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |WIDE  | 160;
	sprites[46+off].attribute1 = SIZE_8 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+84 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 = SIZE_8 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+88 | PRIORITY(2);
}

void MoveBubbles(pWeapon ur)
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

				s32 x = ((MISSILE_SPEED) * (s32)SIN[angle])>>8;
				s32 y = ((MISSILE_SPEED) * (s32)COS[angle])>>8;


				ur->xspeed = x;//(ur->xspeed + x)/2;
				ur->yspeed = y;//(ur->yspeed + y)/2;

		ur->turn_wait = TRACK_WAIT;
	}
	ur->xpos+=ur->xspeed;
	ur->ypos-=ur->yspeed;


}

void RestoreGFXAndrosynth(pPlayer p)
{
	//only restore limpets
	//im sure laser will be fine...
	for(int i=4;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==BUBBLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+96 | PRIORITY(1);
			}



		}
	}
}
void PostAndro(pPlayer pl)
{
	for(int i=0;i<12;i++)
	{
		if (pl->weapon[i].life==0)
		{
			if(pl->weapon[i].type==BUBBLE)
			{
   			sprites[pl->weapon[i].sprite].attribute2 = pl->SpriteStart+98 | PRIORITY(1);
			}
		}
	}
	//blazer
	if (pl->blaze==1)
	{
		if (pl->batt==0)
		{
			//stop blazing
			pl->mass=SHIP_MASS;
			pl->turn_wait=TURN_WAIT;
			sprites[(pl->plr==1)?0:13].attribute2 = pl->SpriteStart | PRIORITY(1);
			pl->blaze=0;
			pl->xspeed=0;
			pl->yspeed=0;
		}
		else
		{
			pl->batt_wait=ENERGY_WAIT;
			pl->thrust_turn=THRUST_WAIT;
			pl->special_turn=SPECIAL_WAIT;
			pl->weapon_turn=WEAPON_WAIT;

			pl->xpos+=((BLAZER_THRUST) * (s32)SIN[pl->angle])>>8;
			pl->ypos-=((BLAZER_THRUST) * (s32)COS[pl->angle])>>8;

			if (pl->batt_turn==ENERGY_WAIT>>1)
			{
				ModifyBatt(pl,-1);
				pl->batt_turn=ENERGY_WAIT;
			}


		}
	}
}

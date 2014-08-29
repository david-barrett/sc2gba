#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "chenjesu_out.h"
#include "chenjesu_dogi.h"
#include "chenjesu_splinter.h"
#include "chenjesu_bullet.h"


#include "chenjesu_sfx.h"

#include "chenjesupilot.h"
/*
#include "chenjesupilotl.h"
#include "chenjesupilotr.h"
#include "chenjesupilott.h"
#include "chenjesupilotf.h"
#include "chenjesupilots.h"
*/

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireChenjesu(pPlayer pl);
int SpecialChenjesu(pPlayer pl);
int aiChenjesu(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetChenjesuPilot(pPlayer p);
void RestoreGFXChenjesu(pPlayer p);
void MoveDogi(pWeapon ur);
void PostChenjesu(pPlayer p);

#define MAX_CREW 36
#define MAX_ENERGY 30
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 5
#define SPECIAL_ENERGY_COST MAX_ENERGY
#define ENERGY_WAIT 4
#define MAX_THRUST  /* DISPLAY_TO_WORLD (7) */ 27
#define THRUST_INCREMENT /* DISPLAY_TO_WORLD (2) */ 3
#define TURN_WAIT 6
#define THRUST_WAIT 4
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 0

#define MAX_DOGGIES 4
#define ENERGY_DRAIN 10
#define DOGGY_SPEED 8

#define SHIP_MASS 10
#define MISSILE_SPEED 16
#define MISSILE_LIFE 90 /* actually, it's as long as you
										 * hold the button down.
										 */
#define FRAGMENT_LIFE 10
#define FRAGMENT_SPEED MISSILE_SPEED
#define FRAGMENT_RANGE (FRAGMENT_LIFE * FRAGMENT_SPEED)


#define FRAGMENT_HITS 1
#define FRAGMENT_DAMAGE 2


void LoadChenjesu(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = chenjesuData[loop-OAMStart];
       		OAMData[loop+512] = chenjesu_outData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+1024] = chenjesu_dogiData[loop-OAMStart]; //loads some garb
		OAMData[loop+1024+128] = chenjesu_bulletData[loop-OAMStart]; //loads some garb
   	}

   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
		OAMData[loop+1024+256] = chenjesu_splinterData[loop-OAMStart]; //loads some garb



	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+256+32] = chenjesupilotData[loop-OAMStart];
	}
/*
	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+64] = chenjesupilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+64+512] = chenjesupilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*3)+64] = chenjesupilottData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+256] = chenjesupilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+64+512] = chenjesupilotsData[loop-OAMStart];

	}
*/
}

int FireChenjesu(pPlayer pl)
{
	if (pl->charging>0)
		return 0;

	s16 b= 0;

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=MISSILE_LIFE;
	pl->weapon[b].damage=-8; //guess
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=16;
	pl->weapon[b].angle = pl->angle;

	pl->weapon[b].xspeed=((s32)(MISSILE_SPEED * SIN[pl->angle])>>8);
	pl->weapon[b].yspeed=((s32)(MISSILE_SPEED * COS[pl->angle])>>8);;


	pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset * SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset * COS[pl->angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+72 | PRIORITY(1);

	play_sfx(&chenjesu_fire,pl->plr-1);

	pl->charging=1;

	return 1;
}

int CreateSplinter(pPlayer pl,s16 x,s16 y,s16 a,s8 b)
{

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=FRAGMENT_LIFE;
	pl->weapon[b].damage=FRAGMENT_DAMAGE*-1;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = 0;

	pl->weapon[b].xspeed=((s32)(FRAGMENT_SPEED * SIN[a])>>8);
	pl->weapon[b].yspeed=((s32)(FRAGMENT_SPEED * COS[a])>>8);;


	pl->weapon[b].xpos = x;
	pl->weapon[b].ypos = y;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+80 | PRIORITY(1);
	return 1;

}

void SetChenjesu(pPlayer pl)
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

		pl->range=1440;

	pl->fireangle=45;

	pl->firefunc=&FireChenjesu;
	pl->specfunc=&SpecialChenjesu;
	pl->aifunc=&aiChenjesu;
	pl->loadfunc=&LoadChenjesu;
	pl->loadpilots=&SetChenjesuPilot;
	pl->postfunc=&PostChenjesu;
	pl->restorefunc=&RestoreGFXChenjesu;

	pl->ditty=&chenjesu_ditty;

	pl->ship_flags = FIRES_FORE | SEEKING_SPECIAL | SEEKING_WEAPON;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+256+32)/16;
	pl->pilots[0].x=240;
	pl->pilots[0].y=160;
	pl->pilots[1].x=240;
	pl->pilots[1].y=160;
	pl->pilots[2].x=240;
	pl->pilots[2].y=160;
	pl->pilots[3].x=240;
	pl->pilots[3].y=160;
	pl->pilots[4].x=240;
	pl->pilots[4].y=160;

	pl->charging=0;
	for (int i=8;i<12;i++)
		pl->weapon[i].life=-2;

}

int SpecialChenjesu(pPlayer pl)
{
	s16 b= nextWeapon(pl,8,11);
		int ret=0;
		if (b>0)
		{
			pl->weapon[b].type=DOGI;
			pl->weapon[b].life=1;
			pl->weapon[b].status=80;
			pl->weapon[b].movefunc=&MoveDogi;
			pl->weapon[b].damage=0;
			pl->weapon[b].target=pl->opp;
			pl->weapon[b].parent=pl;
			pl->weapon[b].damageparent=0;

			pl->weapon[b].size=16;
			pl->weapon[b].angle = 0;

			pl->weapon[b].xspeed=0;
			pl->weapon[b].yspeed=0;


			pl->weapon[b].xpos = pl->xpos-((s32)(pl->offset * SIN[pl->angle])>>8);
			pl->weapon[b].ypos = pl->ypos+((s32)(pl->offset * COS[pl->angle])>>8);

			drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
				pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

		 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
			sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
			sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
			ret=1;
			play_sfx(&chenjesu_dogi,pl->plr-1);
		}



	return ret;


}

int aiChenjesu(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		if (lpEvalDesc->parent)
		{



			if ((lpEvalDesc->which_turn <= 16
					&& opp->ManeuverabilityIndex >= MEDIUM_SHIP)
					|| (opp->ManeuverabilityIndex <= SLOW_SHIP
					&& opp->range >= (LONG_RANGE_WEAPON * 3 >>2)
					&& (ai->ship_flags & SEEKING_WEAPON)))
				lpEvalDesc->MoveState = PURSUE;
		}

		if (ai->special_turn == 1
				&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent
				&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].MoveState == ENTICE
				&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn <= 8)
		{
			lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
		}

		ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);
/*
		if (lpEvalDesc->parent)
		{
			HELEMENT h, hNext;
			ELEMENTPTR CrystalPtr;

			h = (StarShipPtr->old_status_flags & WEAPON) ?
					GetTailElement () : (HELEMENT)0;
			for (; h; h = hNext)
			{
				LockElement (h, &CrystalPtr);
				hNext = GetPredElement (CrystalPtr);
				if (!(CrystalPtr->state_flags & NONSOLID)
						&& CrystalPtr->next.image.farray == StarShipPtr->RaceDescPtr->ship_data.weapon
						&& CrystalPtr->preprocess_func
						&& CrystalPtr->life_span > 0
						&& (CrystalPtr->state_flags & (GOOD_GUY | BAD_GUY)) ==
						(ShipPtr->state_flags & (GOOD_GUY | BAD_GUY)))
				{
					if (ObjectsOfConcern[ENEMY_SHIP_INDEX].ObjectPtr)
					{
						COUNT which_turn;

						if ((which_turn = PlotIntercept (CrystalPtr,
								ObjectsOfConcern[ENEMY_SHIP_INDEX].ObjectPtr,
								CrystalPtr->life_span,
								FRAGMENT_RANGE / 2)) == 0
								|| (which_turn == 1
								&& PlotIntercept (CrystalPtr,
								ObjectsOfConcern[ENEMY_SHIP_INDEX].ObjectPtr,
								CrystalPtr->life_span, 0) == 0))
							StarShipPtr->ship_input_state &= ~WEAPON;
						else if (StarShipPtr->weapon_counter == 0)
						{
							StarShipPtr->ship_input_state |= WEAPON;
							lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
						}

						UnlockElement (h);
						break;
					}
					hNext = 0;
				}
				UnlockElement (h);

			}

			if (h == 0)
			{
				if (StarShipPtr->old_status_flags & WEAPON)
				{
					StarShipPtr->ship_input_state &= ~WEAPON;
					if (lpEvalDesc == &ObjectsOfConcern[ENEMY_WEAPON_INDEX])
						ai->weapon_turn = 3;
				}
				else if (StarShipPtr->weapon_counter == 0
						&& ship_weapons (ShipPtr, lpEvalDesc->ObjectPtr, FRAGMENT_RANGE / 2))
					ai->ship_input_state |= WEAPON;
			}
	}*/

		ai->ship_input_state &= ~WEAPON;

		if (ai->charging>0)//we have fired
		{
			s16 angle=FindAngle(ai->weapon[0].xpos,ai->weapon[0].ypos,opp->xpos,opp->ypos);
			s16 d=distanceBetweenPoints(ai->weapon[0].xpos,ai->weapon[0].ypos,opp->xpos,opp->ypos);

			//if we are close
			if (d<CLOSE_RANGE_WEAPON)
			{
				//if we are close and on target continue
				//else fragment
				if (TurnAngle(angle,ai->weapon[0].angle,45)==0)
					ai->ship_input_state |= WEAPON;
				else
					ai->ship_input_state &= ~WEAPON;
			}
			//if we are near or totally out fragment
			else
			{
				if (TurnAngle(angle,ai->weapon[0].angle,90)==0)
					ai->ship_input_state |= WEAPON;
				else
					ai->ship_input_state &= ~WEAPON;

			}
		}
		else //should we fire?
		{
			s16 angle=FindAngle(ai->xpos,ai->ypos,opp->xpos,opp->ypos);

			if (TurnAngle(angle,ai->angle,15)==0)
				ai->ship_input_state |= WEAPON;
			else
				ai->ship_input_state &= ~WEAPON;

		}


		//if (StarShipPtr->special_counter < MAX_DOGGIES)
		{
			if (lpEvalDesc->parent
					&& ai->batt <= SPECIAL_ENERGY_COST
					&& !(ai->ship_input_state & WEAPON))
				ai->ship_input_state |= SPECIAL;
	}


}

void SetChenjesuPilot(pPlayer pl)
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

void MoveDogi(pWeapon ur)
{
	s16 angle;
		pPlayer target=(pPlayer)ur->target;
		angle = FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos);


		if (DetectWeaponToShip(target,ur)==1)
		{
			//bounce somehow
			ur->xpos-=((DOGGY_SPEED) * (s32)SIN[angle])>>7;
			ur->ypos+=((DOGGY_SPEED) * (s32)COS[angle])>>7;

			ModifyBatt(target,-1*ENERGY_DRAIN);
		}


		else if (target->crew>0)
		{



			ur->xpos+=((DOGGY_SPEED) * (s32)SIN[angle])>>8;
			ur->ypos-=((DOGGY_SPEED) * (s32)COS[angle])>>8;
		}


}

void RestoreGFXChenjesu(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==DOGI)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}
		}
	}
}

void PostChenjesu(pPlayer p)
{
	if (p->weapon[0].life>0&&p->charging>0&&(!(p->ship_input_state & WEAPON))&&p->weapon[0].type!=EXP)
	{

		p->charging=0;
		//splinter
		s16 a;
		s16 x=p->weapon[0].xpos;
		s16 y=p->weapon[0].ypos;
		play_sfx(&chenjesu_fragment,p->plr-1);

		for (int i=0;i<8;i++)
		{
			a=(i*90)>>1;
			CreateSplinter(p,x,y,a,i);
		}

	}

	for (int i=8;i<12;i++)
	if (p->weapon[i].life==-1)
	{	play_sfx(&chenjesu_dogidie,p->plr-1);
		p->weapon[i].life=-2;
	}
}

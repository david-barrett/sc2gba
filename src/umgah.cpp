#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "umgah_out.h"
#include "umgah_coneend.h"
#include "umgah_conestart.h"

#include "umgah_sfx.h"

#include "umgahpilot.h"
#include "umgahpilotl.h"
#include "umgahpilotr.h"
#include "umgahpilott.h"
#include "umgahpilotf.h"
#include "umgahpilots.h"

#define MAX_CREW 10
#define MAX_ENERGY 30
#define ENERGY_REGENERATION MAX_ENERGY
#define WEAPON_ENERGY_COST 0
#define SPECIAL_ENERGY_COST 1
#define ENERGY_WAIT 150
#define MAX_THRUST /* DISPLAY_TO_WORLD (5) */ SHIP_SPEED(18)
#define THRUST_INCREMENT /* DISPLAY_TO_WORLD (2) */ SHIP_SPEED(6)
#define TURN_WAIT 4
#define THRUST_WAIT 3
#define WEAPON_WAIT 0
#define SPECIAL_WAIT 2

#define SHIP_MASS 1



#define JUMP_DIST 40

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireUmgah(pPlayer pl);
int SpecialUmgah(pPlayer pl);
int aiUmgah(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetUmgahPilot(pPlayer p);
void PostUmgah(pPlayer p);
void RestoreGFXUmgah(pPlayer p);

void LoadUmgah(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = umgahData[loop-OAMStart];
       		OAMData[loop+512] = umgah_outData[loop-OAMStart];
       		OAMData[loop+512+512] = umgah_coneendData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512+512] = umgah_conestartData[loop-OAMStart]; //loads some garb
   	}

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1664] = umgahpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
		OAMData[loop+1664+1024] = umgahpilotlData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
			OAMData[loop+1664+1024+256] = umgahpilotrData[loop-OAMStart];
			OAMData[loop+1664+1024+256+512] = umgahpilottData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+1664+1024+256+512+512] = umgahpilotfData[loop-OAMStart];
		OAMData[loop+1664+1024+256+512+512+256] = umgahpilotsData[loop-OAMStart];

	}



}

int SpecialUmgah(pPlayer pl)
{

	pl->xpos-=((JUMP_DIST  * (s32)SIN[pl->angle])>>8);
	pl->ypos+=((JUMP_DIST * (s32)COS[pl->angle])>>8);
	play_sfx(&umgah_backzip,pl->plr-1);
	pl->xspeed=0;
	pl->yspeed=0;
	return 1;
}

void SetUmgah(pPlayer pl)
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
		pl->offset=7;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=45;

	pl->firefunc=&FireUmgah;
	pl->specfunc=&SpecialUmgah;
	pl->aifunc=&aiUmgah;
	pl->loadfunc=&LoadUmgah;
	pl->loadpilots=&SetUmgahPilot;
	pl->postfunc=&PostUmgah;
	pl->restorefunc=&RestoreGFXUmgah;

	pl->ditty=&umgah_ditty;

	pl->ship_flags = FIRES_FORE | IMMEDIATE_WEAPON;

	pl->pilot_sprite=(1664)/16;
	pl->pilots[0].x=7;
	pl->pilots[0].y=1;
	pl->pilots[1].x=4;
	pl->pilots[1].y=0;
	pl->pilots[2].x=509;
	pl->pilots[2].y=1;
	pl->pilots[3].x=31;
	pl->pilots[3].y=8;
	pl->pilots[4].x=25;
	pl->pilots[4].y=0;


}

int FireUmgah(pPlayer pl)
{

	s16 b;
	int ret=0;


	b= 0;//nextWeapon(pl);

	for (b=0;b<2;b++)
	{
	pl->weapon[b].type=LASER;
	pl->weapon[b].life=1;//range
	pl->weapon[b].damage=-1;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=(b==0)?16:32;
	pl->weapon[b].angle = pl->angle;

	pl->weapon[b].xspeed = 0;
	pl->weapon[b].yspeed = 0;

	//pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle+(i==0?-30:+30)])>>8)/3;
	//pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle+(i==0?-30:+30)])>>8)/3;

	s16 o = (b==0)?10:34;

	pl->weapon[b].xpos = pl->xpos+((o * (s32)SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((o * (s32)COS[pl->angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	if (b==0)
	{
	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(3);
	}
	else
	{
		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[pl->weapon[b].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(3);
	}


    ret++;
	play_sfx(&umgah_cone,pl->plr-1);
	pl->batt_wait=150;
	}
	return ret;
}

int aiUmgah(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer EnemyStarShipPtr=(pPlayer)ai->opp;


		lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
		if (lpEvalDesc->parent && lpEvalDesc->MoveState == ENTICE)
		{
			if (lpEvalDesc->which_turn > 3
					)//|| (StarShipPtr->old_status_flags & SPECIAL))
				lpEvalDesc->parent = 0;
			else if //((lpEvalDesc->ObjectPtr->state_flags & FINITE_LIFE) &&
					(lpEvalDesc->type != CREW)//)
				lpEvalDesc->MoveState = AVOID;
			else
				lpEvalDesc->MoveState = PURSUE;
		}

		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		if (ai->special_turn==0
				|| ObjectsOfConcern[GRAVITY_MASS_INDEX].parent
				|| lpEvalDesc->parent == 0)
		{
			ai->range = CLOSE_RANGE_WEAPON;
			ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

			if (lpEvalDesc->which_turn < 16)
				ai->ship_input_state |= WEAPON;
			ai->ship_input_state &= ~SPECIAL;
		}
		else
		{
			s8 this_turn;
			s8 EnemyBehind, EnoughJuice;

			if (lpEvalDesc->which_turn >= 0xFF + 1)
				this_turn = 0xFF;
			else
				this_turn = (s8)lpEvalDesc->which_turn;

			EnoughJuice = (s8)((JUMP_DIST * ai->batt) > this_turn);
			/*
			delta_x = lpEvalDesc->ObjectPtr->next.location.x -
					ShipPtr->next.location.x;
			delta_y = lpEvalDesc->ObjectPtr->next.location.y -
					ShipPtr->next.location.y;
					*/
			s16 angle=FindAngle(ai->xpos,ai->ypos,EnemyStarShipPtr->xpos,EnemyStarShipPtr->ypos);
			EnemyBehind = (!(TurnAngle(angle,ai->angle,180)));

/*
			if (EnoughJuice
					&& (//(StarShipPtr->old_status_flags & SPECIAL) ||
					 EnemyBehind
					|| (this_turn > 6
					&& EnemyStarShipPtr->ManeuverabilityIndex
					) <= SLOW_SHIP)
					|| (this_turn >= 16 && this_turn <= 24))
					*/
			if (EnoughJuice && (EnemyBehind
				|| (this_turn > 6 && EnemyStarShipPtr->ManeuverabilityIndex) <= SLOW_SHIP)
				|| (this_turn >= 16 && this_turn <= 24))
				ai->range = (LONG_RANGE_WEAPON << 3);
			else
				ai->range = CLOSE_RANGE_WEAPON;

			ship_intelligence(ai,ObjectsOfConcern, ConcernCounter);

			if (ai->range == CLOSE_RANGE_WEAPON)
				ai->ship_input_state &= ~SPECIAL;
			else
			{
				s8 LinedUp;

				ai->ship_input_state &= ~THRUST;
				LinedUp = (s8)(ai->turn_turn == 0);
						//&& !(StarShipPtr->old_status_flags & (LEFT | RIGHT)));
				/*if (((StarShipPtr->old_status_flags & SPECIAL)
						&& this_turn <= ai->special_turn)
						|| (!(StarShipPtr->old_status_flags & SPECIAL)
						&& EnemyBehind && (LinedUp || this_turn < 16)))*/
				if (((this_turn <= ai->special_turn) || (EnemyBehind && (LinedUp || this_turn < 16))))
				{
					ai->ship_input_state |= SPECIAL;
					ai->special_turn = this_turn;

					/* don't want him backing straight into ship */
					if (this_turn <= 8 && LinedUp)
					{
						if (ran(0,1))
							ai->ship_input_state |= LEFT;
						else
							ai->ship_input_state |= RIGHT;
					}
				}
				/*
				else if (StarShipPtr->old_status_flags & SPECIAL)
				{
					StarShipPtr->ship_input_state &= ~(SPECIAL | LEFT | RIGHT);
					StarShipPtr->ship_input_state |= THRUST;
				}
				*/
			}

			if (this_turn < 16 && !EnemyBehind)
				ai->ship_input_state |= WEAPON;
		}

		if (!(ai->ship_input_state & SPECIAL))
		ai->special_turn = 0xFF;


}

void SetUmgahPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+112 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 | TALL | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+144 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+160 | PRIORITY(2);
}

void PostUmgah(pPlayer pl)
{
	//bit kludgy stops cone turning into explosion
	/*
	if (pl->ship_input_state & WEAPON)
		FireUmgah(pl);
	*/
}

void RestoreGFXUmgah(pPlayer p)
{
	/*
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+96 | PRIORITY(1);
			}
		}
	}
	*/
}

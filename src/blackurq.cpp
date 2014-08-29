#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "blackurq_out.h"
#include "blackurq_blade.h"
#include "blackurq_fire.h"
#include "thradd_flame.h"


#include "blackurq_sfx.h"

#include "blackurqpilot.h"
#include "blackurqpilotl.h"
#include "blackurqpilotr.h"
#include "blackurqpilott.h"
#include "blackurqpilotf.h"
#include "blackurqpilots.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireBlackurq(pPlayer pl);
int SpecialBlackurq(pPlayer pl);
int aiBlackurq(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetBlackurqPilot(pPlayer p);
void RestoreGFXBlackurq(pPlayer p);
void MoveDogi(pWeapon ur);
void PostBlackurq(pPlayer p);

extern s32 zoom;
extern s8 v3do;

void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#define MAX_CREW 42
#define MAX_ENERGY 42
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 6
#define SPECIAL_ENERGY_COST (42 / 2)
#define ENERGY_WAIT 4
#define MAX_THRUST SHIP_SPEED(30)
#define THRUST_INCREMENT SHIP_SPEED(6)
#define TURN_WAIT 4
#define THRUST_WAIT 6
#define WEAPON_WAIT 6
#define SPECIAL_WAIT 9

#define SHIP_MASS 10
#define MISSILE_SPEED 64
#define MISSILE_LIFE 64 /* actually, it's as long as you
										 * hold the button down.
										 */
#define MISSILE_DAMAGE 4

#define BLADE_SPEED 2
#define BLADE_WAIT 2

#define GAS_DAMAGE 3
#define GAS_SPEED 16
#define GAS_LIFE 10 //guess

void LoadBlackurq(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)           
  	{
       		OAMData[loop] = blackurqData[loop-OAMStart];
       		OAMData[loop+512] = blackurq_outData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)           
   	{
		OAMData[loop+1024] = blackurq_bladeData[loop-OAMStart]; 
		OAMData[loop+1024+128] = blackurq_fireData[loop-OAMStart];
		OAMData[loop+1024+256] = thradd_flameData[loop-OAMStart];
   	}

   	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+256] = blackurqpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+(1024*2)+256] = blackurqpilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+256+512] = blackurqpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*3)+256] = blackurqpilottData[loop-OAMStart];
		OAMData[loop+(1024*3)+128+256] = blackurqpilotfData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
		OAMData[loop+(1024*3)+512] = blackurqpilotsData[loop-OAMStart];

	}
}

int FireBlackurq(pPlayer pl)
{
	if (pl->charging>0)
		return 0;

	pl->currentweapon++;
	if (pl->currentweapon==4)
		pl->currentweapon=0;

	s16 b= pl->currentweapon;

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=MISSILE_LIFE;
	pl->weapon[b].damage=-1*MISSILE_DAMAGE; //guess
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

	play_sfx(&kohrah_blade,pl->plr-1);

	pl->charging=1;



	return 1;
}
/*
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
*/
void SetBlackurq(pPlayer pl)
{
	pl->crew=MAX_CREW;
		pl->maxcrew=MAX_CREW;
		pl->batt=MAX_ENERGY;
		pl->maxbatt=MAX_ENERGY;

		pl->maxspeed=MAX_THRUST;

		pl->accel_inc=THRUST_INCREMENT;

		pl->firebatt=WEAPON_ENERGY_COST;
		pl->specbatt=SPECIAL_ENERGY_COST;

		pl->offset=16;

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

	pl->firefunc=&FireBlackurq;
	pl->specfunc=&SpecialBlackurq;
	pl->aifunc=&aiBlackurq;
	pl->loadfunc=&LoadBlackurq;
	pl->loadpilots=&SetBlackurqPilot;
	pl->postfunc=&PostBlackurq;
	pl->restorefunc=&RestoreGFXBlackurq;

	pl->ditty=&kohrah_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+256)/16;
	pl->pilots[0].x=28;
	pl->pilots[0].y=0;
	pl->pilots[1].x=3;
	pl->pilots[1].y=0;
	pl->pilots[2].x=25;
	pl->pilots[2].y=9;
	pl->pilots[3].x=17;
	pl->pilots[3].y=23;
	pl->pilots[4].x=0;
	pl->pilots[4].y=0;

	pl->charging=0;
	pl->currentweapon=0;

}

int SpecialBlackurq(pPlayer pl)
{

	for (int b=4;b<12;b++)
		{
			pl->weapon[b].type=SIMPLE;
			pl->weapon[b].life=GAS_LIFE;
			pl->weapon[b].damage=-1*GAS_DAMAGE;
			pl->weapon[b].target=pl->opp;
			pl->weapon[b].parent=pl;
			pl->weapon[b].damageparent=0;

			pl->weapon[b].size=16;
			pl->weapon[b].angle = (b-4)*45;

			pl->weapon[b].xspeed=((s32)(GAS_SPEED * SIN[pl->weapon[b].angle])>>8);
			pl->weapon[b].yspeed=((s32)(GAS_SPEED * COS[pl->weapon[b].angle])>>8);


			pl->weapon[b].xpos = pl->xpos-((s32)(pl->offset * SIN[pl->angle])>>8);
			pl->weapon[b].ypos = pl->ypos+((s32)(pl->offset * COS[pl->angle])>>8);

			#ifdef MISSILE_START
			pl->weapon[b].xpos-=pl->weapon[b].xspeed;
			pl->weapon[b].ypos+=pl->weapon[b].yspeed;
			#endif

			drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
				pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

		 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
			sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
			if (v3do)
				sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+72 | PRIORITY(1);
			else
				sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+80 | PRIORITY(1);
			//tmp use exp
			//sprites[pl->weapon[b].sprite].attribute2 = FireSprite1+4 | PRIORITY(1);//4or12or16


		}
		play_sfx(&kohrah_fried,pl->plr-1);


	return 1;


}

int aiBlackurq(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
		if (lpEvalDesc->parent
				&& lpEvalDesc->MoveState == ENTICE
				&& (lpEvalDesc->type== CREW)
				&& lpEvalDesc->which_turn <= 8)
			lpEvalDesc->MoveState = PURSUE;

		ship_intelligence (ai,	ObjectsOfConcern, ConcernCounter);


		ai->ship_input_state &= ~SPECIAL;

		if (ai->special_turn == 0
				&& ai->batt >= SPECIAL_ENERGY_COST
				&& lpEvalDesc->parent
				&& lpEvalDesc->which_turn <= 8)
			ai->ship_input_state |= SPECIAL;

		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		/*
		if (lpEvalDesc->parent)
		{
	#define FRAGMENT_LIFE 10
	#define FRAGMENT_SPEED MISSILE_SPEED
	#define FRAGMENT_RANGE (FRAGMENT_LIFE * FRAGMENT_SPEED)
			HELEMENT h, hNext;
			ELEMENTPTR BuzzSawPtr;

			h = (StarShipPtr->old_status_flags & WEAPON) ?
					GetSuccElement (ShipPtr) : (HELEMENT)0;
			for (; h; h = hNext)
			{
				LockElement (h, &BuzzSawPtr);
				hNext = GetSuccElement (BuzzSawPtr);
				if (!(BuzzSawPtr->state_flags & NONSOLID)
						&& BuzzSawPtr->next.image.farray == StarShipPtr->RaceDescPtr->ship_data.weapon
						&& BuzzSawPtr->life_span > MISSILE_LIFE * 3 / 4
						&& (BuzzSawPtr->state_flags & (GOOD_GUY | BAD_GUY)) ==
						(ShipPtr->state_flags & (GOOD_GUY | BAD_GUY)))
				{
					{
						//COUNT which_turn;

						if (!PlotIntercept (BuzzSawPtr,
								lpEvalDesc->ObjectPtr, BuzzSawPtr->life_span,
								FRAGMENT_RANGE / 2))
							StarShipPtr->ship_input_state &= ~WEAPON;
						else if (StarShipPtr->weapon_counter == 0)
							StarShipPtr->ship_input_state |= WEAPON;

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
					StarShipPtr->ship_input_state &= ~WEAPON;
				else if (StarShipPtr->weapon_counter == 0
						&& ship_weapons (ShipPtr, lpEvalDesc->ObjectPtr, FRAGMENT_RANGE / 2))
					StarShipPtr->ship_input_state |= WEAPON;

				if (StarShipPtr->special_counter == 0
						&& !(StarShipPtr->ship_input_state & WEAPON)
						&& lpEvalDesc->which_turn <= 8
						&& (StarShipPtr->ship_input_state & (LEFT | RIGHT))
						&& StarShipPtr->RaceDescPtr->ship_info.energy_level >=
						SPECIAL_ENERGY_COST)
					StarShipPtr->ship_input_state |= SPECIAL;
			}
	}
	*/
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

}

void SetBlackurqPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[45+off].attribute1 = SIZE_16 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |TALL  | 160;
	sprites[46+off].attribute1 = SIZE_16 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+8 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_64 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+16 | PRIORITY(2);
}

void RestoreGFXBlackurq(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}
		}
	}
}

void MoveBlades(pWeapon w)
{
	pPlayer target = (pPlayer)w->target;
	s16 d;
	if (w->turn_wait==0)
	{
		d=distanceBetweenPoints(target->xpos,target->ypos,w->xpos,w->ypos);
	if (target->crew>0&&d<24)
	{
		w->turn_wait=BLADE_WAIT;
		s16 angle = FindAngle(w->xpos,w->ypos,target->xpos,target->ypos);

		w->xpos+=((BLADE_SPEED) * (s32)SIN[angle])>>8;
		w->ypos-=((BLADE_SPEED) * (s32)COS[angle])>>8;
	}
	}
	else
		w->turn_wait--;

}

void PostBlackurq(pPlayer p)
{
	s16 a=p->angle-45;
	if (a<0)
		a+=360;
	RotateSprite(p->plr==1?0:13, a, zoom, zoom);

	if (p->warp>1)
	{/*
		for (int i=0;i<12;i++)
		{
			if (p->weapon[i].type==TRAIL)
			{
				RotateSprite(p->weapon[i].sprite, a, zoom, zoom);
				p->weapon[i].angle= a;
			}
		}*/
	}
	else
	{

	for (int i=0;i<4;i++)
	{
		if (p->weapon[i].type==SIMPLE||p->weapon[i].type==BLADES)
		{
		p->weapon[i].actualangle+=1;
		if (p->weapon[i].actualangle==16)
			p->weapon[i].actualangle=0;
		p->weapon[i].angle=(p->weapon[i].actualangle*45)>>1;
		if (p->weapon[i].life==MISSILE_LIFE-1)
			p->weapon[i].life++;
		}
	}

	}
	if (p->charging>0&&(!(p->ship_input_state & WEAPON)))

	{
		p->weapon[p->currentweapon].xspeed=0;
		p->weapon[p->currentweapon].yspeed=0;
		p->charging=0;
		if (v3do)
		{
			p->weapon[p->currentweapon].movefunc=&MoveBlades;
			p->weapon[p->currentweapon].type=BLADES;
			p->weapon[p->currentweapon].turn_wait=BLADE_WAIT;
		}
	}
}

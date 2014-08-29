#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "gfx/yehat_out.h"
#include "gfx/yehat_fire.h"
#include "gfx/yehat_shield.h"

#include "yehat_sfx.h"

#include "yehatpilot.h"
#include "yehatpilotl.h"
#include "yehatpilotr.h"
#include "yehatpilott.h"
#include "yehatpilotf.h"
#include "yehatpilots.h"



extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireYehat(pPlayer pl);
int SpecialYehat(pPlayer pl);
int aiYehat(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetYehatPilot(pPlayer p);

void LoadYehat(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = yehatData[loop-OAMStart];
       		OAMData[loop+512] = yehat_outData[loop-OAMStart];
       		OAMData[loop+1024] = yehat_shieldData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512+512] = yehat_fireData[loop-OAMStart]; //loads some garb
   	}

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+512+32] = yehatpilotData[loop-OAMStart];
			OAMData[loop+(1024*2)+512+32] = yehatpilotlData[loop-OAMStart];
			OAMData[loop+(1024*3)+512+32] = yehatpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*4)+512+32] = yehatpilottData[loop-OAMStart];
		OAMData[loop+(1024*4)+512+32+128] = yehatpilotfData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+64;loop++)
		OAMData[loop+(1024*4)+512+32+256] = yehatpilotsData[loop-OAMStart];


}

int SpecialYehat(pPlayer pl)
{
	pl->shield=6;
	pl->spriteoffset=64;
	sprites[(pl->plr==1)?0:13].attribute2 = pl->SpriteStart+pl->spriteoffset | PRIORITY(1);
	play_sfx(&yehat_shield,pl->plr-1);
	return 1;
}

void SetYehat(pPlayer pl)
{
	pl->crew=20;
	pl->maxcrew=20;
	pl->batt=10;
	pl->maxbatt=10;

	pl->maxspeed=30;

	pl->accel_inc=2;

	pl->firebatt=1;
	pl->specbatt=3;

	pl->offset=20;

	pl->batt_wait=6;
	pl->turn_wait=2;
	pl->thrust_wait=2;
	pl->weapon_wait=0;
	pl->special_wait=2;
	pl->batt_regen=2;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=200;

	pl->fireangle=45;

	pl->firefunc=&FireYehat;
	pl->specfunc=&SpecialYehat;
	pl->aifunc=&aiYehat;
	pl->loadfunc=&LoadYehat;
	pl->loadpilots=&SetYehatPilot;

	pl->ditty=&yehat_ditty;

	pl->ship_flags = FIRES_FORE | SHIELD_DEFENSE;

	pl->pilot_sprite=(1024+512+32)/16;
	pl->pilots[0].x=0;
	pl->pilots[0].y=0;
	pl->pilots[1].x=0;
	pl->pilots[1].y=0;
	pl->pilots[2].x=3;
	pl->pilots[2].y=1;
	pl->pilots[3].x=43;
	pl->pilots[3].y=1;
	pl->pilots[4].x=28;
	pl->pilots[4].y=1;


}

int FireYehat(pPlayer pl)
{

	s16 b;
	int ret=0;

	for (int i=0;i<2;i++)
	{
	b= nextWeapon(pl);

	if (b>=0)
	{
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=10;//range
	pl->weapon[b].damage=-3;//6 suspect 6 was for both guns;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = pl->angle;

	s32 speed=20;
	pl->weapon[b].xspeed = ((speed * (s32)SIN[pl->angle])>>9);///SPEED_REDUCT;
	pl->weapon[b].yspeed = ((speed * (s32)COS[pl->angle])>>9);///SPEED_REDUCT;

	//pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle+(i==0?-30:+30)])>>8)/3;
	//pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle+(i==0?-30:+30)])>>8)/3;

	pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[ModifyAngle(pl->angle,(i==0?-30:+30))])>>8)/3;
	pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[ModifyAngle(pl->angle,(i==0?-30:+30))])>>8)/3;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);

    ret++;
	}
	play_sfx(&yehat_fire,pl->plr-1);
	}
	return ret;
}

int aiYehat(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	s16 ShieldStatus;
	//	STARSHIPPTR StarShipPtr;
		pObject lpEvalDesc;

		pPlayer opp = (pPlayer)ai->opp;

		ShieldStatus = 1;
		lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
		/*
		if (lpEvalDesc->ObjectPtr && lpEvalDesc->MoveState == ENTICE)
		{
			ShieldStatus = 0;
		//	if (!(lpEvalDesc->	->state_flags & (FINITE_LIFE | CREW_OBJECT)))
			//	lpEvalDesc->MoveState = PURSUE;
			//else
			if (lpEvalDesc.type==CREW))
			{
				if (!(lpEvalDesc->ObjectPtr->state_flags & FINITE_LIFE))
					lpEvalDesc->which_turn <<= 1;
				else
				{
					if ((lpEvalDesc->which_turn >>= 1) == 0)
						lpEvalDesc->which_turn = 1;

					if (lpEvalDesc->ObjectPtr->mass_points)
						lpEvalDesc->ObjectPtr = 0;
					else
						lpEvalDesc->MoveState = PURSUE;
				}
				ShieldStatus = 1;
			}
		}
		*/

		if (ai->special_turn == 0)
		{
			ai->ship_input_state &= ~SPECIAL;
			if (ShieldStatus)
			{
				/*
				if (ai->crew <= 10
						&& (ShieldStatus > 0 || lpEvalDesc->parent)
						&& lpEvalDesc->which_turn <= 2
						&& (ShieldStatus > 0
						|| (lpEvalDesc->type==PLAYER)
						|| PlotIntercept (ai,lpEvalDesc, 2, 0))
						&& (ran(0,10) < ai->ai-2))
					ai->ship_input_state |= SPECIAL;
					*/

				if (ai->crew <= 10
					&& (lpEvalDesc->type!=CREW)
					&& lpEvalDesc->which_turn <= 2
					&& (ran(0,10) < ai->ai-2))
						ai->ship_input_state |= SPECIAL;


	/*
				if (lpEvalDesc->ObjectPtr
						&& !(lpEvalDesc->ObjectPtr->state_flags & CREW_OBJECT))
					lpEvalDesc->ObjectPtr = 0;*/
			}
		}

		if ((lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX]))
		{
		//	STARSHIPPTR EnemyStarShipPtr;

		//	GetElementStarShip (lpEvalDesc->ObjectPtr, &EnemyStarShipPtr);
			if (!(opp->ship_flags
					& IMMEDIATE_WEAPON))
				lpEvalDesc->MoveState = PURSUE;
	}
	ship_intelligence(ai,ObjectsOfConcern, ConcernCounter);

}

void SetYehatPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 = SIZE_64 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[44+off].attribute1 = SIZE_64 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[45+off].attribute1 = SIZE_16 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+64 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[46+off].attribute1 = SIZE_16 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+64+8 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 = SIZE_8 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128+64+16 | PRIORITY(2);
}
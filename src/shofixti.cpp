#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "shofixti_out.h"
#include "shofixti_bullet.h"
#include "shofixti_exp1.h"
#include "shofixti_exp2.h"


#include "shofixti_sfx.h"

#include "shofixtipilot.h"
/*
#include "shofixtipilotl.h"
#include "shofixtipilotr.h"
#include "shofixtipilott.h"
#include "shofixtipilotf.h"
#include "shofixtipilots.h"
*/

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;
extern pAsteroid asteroids;
extern s32 planetx,planety;
extern s8 pilot;
extern s32 zoom;

int FireShofixti(pPlayer pl);
int SpecialShofixti(pPlayer pl);
int aiShofixti(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetShofixtiPilot(pPlayer p);
void RestoreGFXShofixti(pPlayer p);
void MoveShofixtiMissile(pWeapon ur);
void Postshofixti(pPlayer pl);
void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#define MAX_CREW 6
#define MAX_ENERGY 4
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 1
#define SPECIAL_ENERGY_COST 0
#define ENERGY_WAIT 9
#define MAX_THRUST 35
#define THRUST_INCREMENT 5
#define TURN_WAIT 1
#define THRUST_WAIT 0
#define WEAPON_WAIT 3
#define SPECIAL_WAIT 0

#define SHIP_MASS 1
#define MISSILE_SPEED 24
#define MISSILE_LIFE 10
#define MISSILE_DAMAGE 1

#define DESTRUCT_RANGE 180
#define MAX_DESTRUCTION (DESTRUCT_RANGE / 10)


void LoadShofixti(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = shofixtiData[loop-OAMStart];
       		OAMData[loop+512] = shofixti_outData[loop-OAMStart];
       		OAMData[loop+512+512] = shofixti_exp1Data[loop-OAMStart];
       		OAMData[loop+512+512+512] = shofixti_exp2Data[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+2048] = shofixti_bulletData[loop-OAMStart]; //loads some garb
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+2048+32] = shofixtipilotData[loop-OAMStart];
	}
/*
	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*2)+128+512] = shofixtipilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+256+512] = shofixtipilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+32;loop++)
	{
		OAMData[loop+(1024*2)+256+512+128] = shofixtipilottData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+256+512+128+32] = shofixtipilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+128+32] = shofixtipilotsData[loop-OAMStart];

	}
	*/

}

int FireShofixti(pPlayer pl)
{

	s16 b= nextWeapon(pl);
	int ret=0;

	if (b>=0)
	{
		pl->weapon[b].type=SIMPLE;
		pl->weapon[b].movefunc=0;
		pl->weapon[b].life=MISSILE_LIFE;
		pl->weapon[b].status=0;
		pl->weapon[b].damage=MISSILE_DAMAGE*-1;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=8;
		pl->weapon[b].angle = pl->angle;

		pl->weapon[b].xspeed = (MISSILE_SPEED * (s32)SIN[pl->angle])>>8;
		pl->weapon[b].yspeed = (MISSILE_SPEED * (s32)COS[pl->angle])>>8;


		pl->weapon[b].turn_wait=0;


		pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset*2 * SIN[pl->weapon[b].angle])>>8);
		pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset*2 * COS[pl->weapon[b].angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+128 | PRIORITY(1);
		ret=1;
		play_sfx(&shofixti_fire,pl->plr-1);
	}



	return ret;
}

void SetShofixti(pPlayer pl)
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

	pl->range=300;

	pl->fireangle=45;

	pl->firefunc=&FireShofixti;
	pl->specfunc=&SpecialShofixti;
	pl->aifunc=&aiShofixti;
	pl->loadfunc=&LoadShofixti;
	pl->loadpilots=&SetShofixtiPilot;
	pl->postfunc=&Postshofixti;
	pl->restorefunc=&RestoreGFXShofixti;

	pl->ditty=&shofixti_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(2048+32)/16;
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

	pl->destruct=0;


}

int SpecialShofixti(pPlayer pl)
{

	pl->destruct++;
	if (pl->destruct==1)
	{
		if (pilot)
		{
		//change status
		}
	}
	else if (pl->destruct==2)
	{
			if (pilot)
			{
			//change status
			}
	}
	else if (pl->destruct==3)
	{
		//detonate
		ModifyCrew(pl,MAX_CREW*-1);
		ModifyBatt(pl,MAX_ENERGY*-1);
		play_sfx(&shofixti_kyaiee,pl->plr-1);

		sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+64 | PRIORITY(0);

		//check if anything caught in exp


		s16 destruction;

		pPlayer opp = (pPlayer)pl->opp;
		s32 d = distanceBetweenPoints(pl->xpos,pl->ypos,opp->xpos,opp->ypos);
		int i;

		if (d<=DESTRUCT_RANGE+opp->offset)
		{
			destruction = ((MAX_DESTRUCTION * (DESTRUCT_RANGE - d)
						/ DESTRUCT_RANGE) + 1);
			ModifyCrew(opp,-1*destruction);
		}

		for (i=0;i<12;i++)
		{
			if (opp->weapon[i].life>0)
			{
				d = distanceBetweenPoints(pl->xpos,pl->ypos,
					opp->weapon[i].xpos,opp->weapon[i].ypos);
				if (d<=DESTRUCT_RANGE+(opp->weapon[i].size>>1))
					opp->weapon[i].life=0;

			}
		}

		for (i=0;i<5;i++)
		{
			if (asteroids[i].life>0)
			{
			d = distanceBetweenPoints(pl->xpos,pl->ypos,
			asteroids[i].xpos,asteroids[i].ypos);
			if (d<=DESTRUCT_RANGE+16)
				asteroids[i].life=0;

			}
		}




	}







	return 1;

}

int aiShofixti(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	ship_intelligence (ai,
				ObjectsOfConcern, ConcernCounter);


		if (ai->special_turn == 0)
		{
			if (ai->ship_input_state & SPECIAL)
				ai->ship_input_state &= ~SPECIAL;
			else
			{
				pObject lpWeaponEvalDesc, lpShipEvalDesc;

				lpWeaponEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
				lpShipEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
				if (ai->destruct<2
						|| (lpShipEvalDesc->which_turn <= 4
						&& ai->crew == 1)
						|| (PlotIntercept (ai,lpWeaponEvalDesc,2, 0)
						&& ai->crew==1)
						&& (ran(0,1)))
					ai->ship_input_state |= SPECIAL;
			}
	}





}

void SetShofixtiPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[43+off].attribute1 = SIZE_16 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_16 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+72 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[45+off].attribute1 = SIZE_8 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |WIDE  | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+82 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 = SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+98 | PRIORITY(2);
}



void RestoreGFXShofixti(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+128| PRIORITY(1);
			}



		}
	}
}
void Postshofixti(pPlayer pl)
{
	if (pl->destruct>2)
	{
		//RotateSprite(pl->plr==1?0:13, pl->angle, (s32)(double(zoom)/2.5), (s32)(double(zoom)/2.5));
		RotateSprite(pl->plr==1?0:13, pl->angle, zoom>>1, zoom>>1);
		pl->destruct++;
		if (pl->destruct==8)
			sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+96 | PRIORITY(0);
	}

}


#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "shofixti_out.h"
#include "shofixti_fire.h"
#include "shofixti_exp3.h"
#include "shofixti_exp2b.h"


#include "shofixti_sfx.h"

#include "shofixtipilot.h"
#include "shofixtipilotl.h"
#include "shofixtipilotr.h"
#include "shofixtipilott.h"
#include "shofixtipilotf.h"
#include "shofixtipilots1.h"
#include "shofixtipilots2.h"
#include "shofixtipilots3.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;
extern pAsteroid asteroids;
//extern s32 planet.xpos,planet.ypos;
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
#define MAX_THRUST SHIP_SPEED(35)
#define THRUST_INCREMENT SHIP_SPEED(5)
#define TURN_WAIT 1
#define THRUST_WAIT 0
#define WEAPON_WAIT 3
#define SPECIAL_WAIT 0

#define SHIP_MASS 1
#define MISSILE_SPEED 24
#define MISSILE_LIFE 10
#define MISSILE_DAMAGE 1
#define MISSILE_RANGE  MISSILE_SPEED*MISSILE_LIFE

#define DESTRUCT_RANGE 180
#define MAX_DESTRUCTION (DESTRUCT_RANGE / 10)

#define ps 2080


void LoadShofixti(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       	OAMData[loop] = shofixtiData[loop-OAMStart];
       	OAMData[loop+512] = shofixti_outData[loop-OAMStart];
       	OAMData[loop+512+512] = shofixti_exp3Data[loop-OAMStart];           //load sprite image data
       	OAMData[loop+512+512+512] = shofixti_exp2bData[loop-OAMStart];
	}
   	
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+2048] = shofixti_fireData[loop-OAMStart]; //loads some garb
   	}
	

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+ps] = shofixtipilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+ps+1024] = shofixtipilotlData[loop-OAMStart];
		OAMData[loop+ps+1024+128] = shofixtipilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+512;loop++)
	{
		OAMData[loop+ps+1024+256] = shofixtipilottData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+1024+256+512+ps] = shofixtipilotfData[loop-OAMStart];		

	}


}

int FireShofixti(pPlayer pl)
{

	s16 b= nextWeapon(pl);
	int ret=0;

	if (b>=0)
	{
		pl->weapon[b].type=SIMPLE;
		pl->weapon[b].movefunc=0;
		pl->weapon[b].object.life=MISSILE_LIFE;
		pl->weapon[b].status=0;
		pl->weapon[b].damage=MISSILE_DAMAGE*-1;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;
		pl->weapon[b].movefunc=0;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=0;

		pl->weapon[b].object.size=8;
		pl->weapon[b].object.angle = pl->object.angle;

		pl->weapon[b].object.xspeed = (MISSILE_SPEED * (s32)SIN[pl->object.angle])>>8;
		pl->weapon[b].object.yspeed = (MISSILE_SPEED * (s32)COS[pl->object.angle])>>8;


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

	pl->range=MISSILE_RANGE;

	pl->firefunc=&FireShofixti;
	pl->specfunc=&SpecialShofixti;
	pl->aifunc=&aiShofixti;
	pl->loadfunc=&LoadShofixti;
	pl->loadpilots=&SetShofixtiPilot;
	pl->postfunc=&Postshofixti;
	pl->restorefunc=&RestoreGFXShofixti;

	pl->ditty=&shofixti_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->object.mass_points=SHIP_MASS;

	pl->pilot_sprite=ps/16;
	pl->pilots[0].x=7;
	pl->pilots[0].y=8;
	pl->pilots[1].x=3;
	pl->pilots[1].y=8;
	pl->pilots[2].x=14;
	pl->pilots[2].y=1;
	pl->pilots[3].x=43;
	pl->pilots[3].y=10;
	pl->pilots[4].x=240;
	pl->pilots[4].y=160;

	pl->destruct=0;


}

int SpecialShofixti(pPlayer pl)
{
	s16 loop;
	s32 OAMStart=pl->SpriteStart*16;
	pl->destruct++;
	//bit ineffient load the pilot this way but...
	if (pl->destruct==1)
	{
		if (pilot)
		{
		for (loop=OAMStart ;loop<OAMStart+1024;loop++)
			OAMData[loop+ps] = shofixtipilots1Data[loop-OAMStart];
		}
	}
	else if (pl->destruct==2)
	{
			if (pilot)
			{
			//change status
			for (loop=OAMStart ;loop<OAMStart+1024;loop++)
				OAMData[loop+ps] = shofixtipilots2Data[loop-OAMStart];
			}
	}
	else if (pl->destruct==3)
	{
		for (loop=OAMStart ;loop<OAMStart+1024;loop++)
			OAMData[loop+ps] = shofixtipilots3Data[loop-OAMStart];
		//detonate
		ModifyCrew(pl,MAX_CREW*-1,1);
		ModifyBatt(pl,MAX_ENERGY*-1);
		play_sfx(&shofixti_kyaiee,pl->plr-1);

		sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+64 | PRIORITY(0);

		//check if anything caught in exp


		s16 destruction;

		pPlayer opp = (pPlayer)pl->opp;
		s32 d = distanceBetweenPoints(pl->object.xpos,pl->object.ypos,opp->object.xpos,opp->object.ypos);
		int i;

		if (d<=DESTRUCT_RANGE+opp->offset)
		{
			destruction = ((MAX_DESTRUCTION * (DESTRUCT_RANGE - d)
						/ DESTRUCT_RANGE) + 1);
			ModifyCrew(opp,-1*destruction);
		}

		for (i=0;i<12;i++)
		{
			if (opp->weapon[i].object.life>0)
			{
				d = distanceBetweenPoints(pl->object.xpos,pl->object.ypos,
					opp->weapon[i].object.xpos,opp->weapon[i].object.ypos);
				if (d<=DESTRUCT_RANGE+(opp->weapon[i].object.size>>1))
					opp->weapon[i].object.life=0;

			}
		}

		for (i=0;i<5;i++)
		{
			if (asteroids[i].object.life>0)
			{
			d = distanceBetweenPoints(pl->object.xpos,pl->object.ypos,
			asteroids[i].object.xpos,asteroids[i].object.ypos);
			if (d<=DESTRUCT_RANGE+16)
				asteroids[i].object.life=0;

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
	sprites[43+off].attribute1 =SIZE_16 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 =SIZE_16 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+72 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | SQUARE | 160;
	sprites[45+off].attribute1 =SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |SQUARE  | 160;
	sprites[46+off].attribute1 =SIZE_16 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+112 | PRIORITY(2);

	
}



void RestoreGFXShofixti(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].object.life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 =SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+128| PRIORITY(1);
			}



		}
	}
}
void Postshofixti(pPlayer pl)
{
	if (pl->destruct>2)
	{		
		pl->destruct++;
		if (pl->destruct<5)
		{
			//outline
			sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+32| PRIORITY(0);
		}
		else if (pl->destruct<11)
		{
			sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+96 | PRIORITY(0);
			RotateSprite(pl->plr==1?0:13, pl->object.angle, (s32)(zoom/(1+(pl->destruct-4)*0.3)), (s32)(zoom/(1+(pl->destruct-4)*0.3)));
		}
		else if (pl->destruct==11)
		{
			sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+96 | PRIORITY(0);
			RotateSprite(pl->plr==1?0:13, pl->object.angle, zoom>>2, zoom>>2);
		}
		else if (pl->destruct<18)
		{
			sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+96 | PRIORITY(0);
			RotateSprite(pl->plr==1?0:13, pl->object.angle, (s32)(zoom/(4-(pl->destruct-11)*0.4)), (s32)(zoom/(4-(pl->destruct-11)*0.4)));
		}

		else if (pl->destruct>17)
			sprites[pl->plr==1?0:13].attribute2 = pl->SpriteStart+64 | PRIORITY(0);

	}

}


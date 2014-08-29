#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "syreen_out.h"
#include "syreen_crew.h"
#include "syreen_fire.h"

#include "syreen_sfx.h"

#include "syreenpilot.h"
#include "syreenpilotl.h"
#include "syreenpilotr.h"
#include "syreenpilott.h"
#include "syreenpilotf.h"
#include "syreenpilots.h"



extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireSyreen(pPlayer pl);
int SpecialSyreen(pPlayer pl);
int aiSyreen(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetSyreenPilot(pPlayer p);
//void PostSyreen(pPlayer p);
void RestoreGFXSyreen(pPlayer p);


void LoadSyreen(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = syreenData[loop-OAMStart];
       		OAMData[loop+512] = syreen_outData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512] = syreen_fireData[loop-OAMStart]; //loads some garb
		OAMData[loop+512+512+32] = syreen_crewData[loop-OAMStart]; //loads some garb
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+32+32] = syreenpilotData[loop-OAMStart];

	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*2)+64] = syreenpilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+192] = syreenpilotrData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+64;loop++)
		OAMData[loop+(1024*2)+192+128] = syreenpilottData[loop-OAMStart];
	for (loop=OAMStart ;loop<OAMStart+512;loop++)//152
		OAMData[loop+(1024*2)+192+128+64] = syreenpilotfData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
		OAMData[loop+(1024*2)+512+64+192+128] = syreenpilotsData[loop-OAMStart];


}

int SpecialSyreen(pPlayer pl)
{
	s16 b;
	s16 angle;
	pPlayer opp=(pPlayer)pl->opp;
	int ret=0;
	s32 dist=distanceBetweenPoints(pl->xpos,pl->ypos,opp->xpos,
		opp->ypos);
	if (dist<208&&(!(opp->ship_flags & CREW_IMMUNE)))
		{

			s8 crew_loss;
			crew_loss = ((8		* (208 - dist)/ 209));
			if (crew_loss >= opp->crew)
				crew_loss = opp->crew - 1;

	for (int i=0;i<crew_loss&&opp->crew-ret>1;i++)
	{
		b=nextWeapon(pl,4,12);
	if (b>0)
	{
		pl->weapon[b].type=CREW;
		pl->weapon[b].life=1;
		pl->weapon[b].status=200;//GUESS
		pl->weapon[b].damage=0;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=8;
		pl->weapon[b].angle = 0;

		pl->weapon[b].xspeed=0;
		pl->weapon[b].yspeed=0;


		angle=ModifyAngle(opp->angle,((i*2)-crew_loss)*20);

		pl->weapon[b].xpos = opp->xpos-((s32)(opp->offset * SIN[angle])>>8);
		pl->weapon[b].ypos = opp->ypos+((s32)(opp->offset * COS[angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	 	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+66 | PRIORITY(1);
		ret++;

	}
	}	//loop
	}
	ModifyCrew(opp,ret*-1,1);

	play_sfx(&syreen_song,pl->plr-1);
	return 1;
}

void SetSyreen(pPlayer pl)
{
	pl->crew=12;
	pl->maxcrew=42;
	pl->batt=16;
	pl->maxbatt=16;

	pl->maxspeed=36;

	pl->accel_inc=9;

	pl->firebatt=1;
	pl->specbatt=5;

	pl->offset=15;

	pl->batt_wait=6;
	pl->turn_wait=1;
	pl->thrust_wait=1;
	pl->weapon_wait=8;
	pl->special_wait=20;
	pl->batt_regen=1;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=200;

	pl->fireangle=45;

	pl->firefunc=&FireSyreen;
	pl->specfunc=&SpecialSyreen;
	pl->aifunc=&aiSyreen;
	pl->loadfunc=&LoadSyreen;
	pl->loadpilots=&SetSyreenPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXSyreen;

	pl->ditty=&syreen_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->mass=2;

	pl->pilot_sprite=(512+512+64)/16;
	pl->pilots[0].x=34;
	pl->pilots[0].y=15;
	pl->pilots[1].x=36;
	pl->pilots[1].y=15;
	pl->pilots[2].x=20;
	pl->pilots[2].y=1;
	pl->pilots[3].x=3;
	pl->pilots[3].y=0;
	pl->pilots[4].x=36;
	pl->pilots[4].y=0;
}

int FireSyreen(pPlayer pl)
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

	s32 speed=20;//20;
	pl->weapon[b].xspeed = ((speed * (s32)SIN[pl->angle])>>9);///SPEED_REDUCT;
	pl->weapon[b].yspeed = ((speed * (s32)COS[pl->angle])>>9);///SPEED_REDUCT;

	pl->weapon[b].xpos = pl->xpos+((40 * (s32)SIN[pl->angle])>>8)/3;
	pl->weapon[b].ypos = pl->ypos-((40 * (s32)COS[pl->angle])>>8)/3;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
  	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+62 | PRIORITY(1);

	play_sfx(&syreen_bullet,pl->plr-1);
	return 1;
	}
	return 0;
}

int aiSyreen(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

		ship_intelligence (ai,
				ObjectsOfConcern, ConcernCounter);

		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		if (lpEvalDesc->parent != 0)
		{
			if (!(opp->ship_flags & CREW_IMMUNE)
					&& ai->special_turn == 0
					&& opp->crew > 1
					&& lpEvalDesc->which_turn <= 14)
				ai->ship_input_state |= SPECIAL;
			else
				ai->ship_input_state &= ~SPECIAL;
	}



}

void SetSyreenPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[43+off].attribute1 = SIZE_16 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_16 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+72 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[45+off].attribute1 = SIZE_8 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+84 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_8| 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+118 | PRIORITY(2);
}

void MoveCrew(pWeapon ur)
{
	pPlayer target=(pPlayer)ur->target;

	pPlayer parent=(pPlayer)ur->parent;
	ur->status--;

	if(ur->status==0)
	{
		ur->life=0;
		return;
	}

	if (DetectWeaponToShip(parent,ur)==1)
	{
		//attach
		ModifyCrew(parent,1);
		play_sfx(&syreen_pickup,parent->plr-1);
		ur->life=0;
	}
	else if (DetectWeaponToShip(target,ur)==1)
	{
		//attach
		ModifyCrew(target,1);
		play_sfx(&syreen_pickup,parent->plr-1);
		ur->life=0;
	}


}

void RestoreGFXSyreen(pPlayer p)
{
	int o;
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
				o=62;
			else //crew
				o=66;

			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+o | PRIORITY(1);

		}
	}
}


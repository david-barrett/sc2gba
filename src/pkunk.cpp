#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "pkunk_out.h"
#include "pkunk_fire.h"

#include "pkunk_sfx.h"
#include "TektronicWave.h"

#include "pkunkpilot.h"
#include "pkunkpilotl.h"
#include "pkunkpilotr.h"
#include "pkunkpilott.h"
#include "pkunkpilotf.h"
#include "pkunkpilots.h"



extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireFury(pPlayer pl);
int SpecialFury(pPlayer pl);
int aiFury(pPlayer pl, pObject ObjectsOfConcern, COUNT ConcernCounter);
void LoadFury(s16 SpriteStart);
void SetPkunkPilot(pPlayer pl);
void RestoreGFXFury(pPlayer p);

const PCMSOUND insults[] = {pkunk_baby,pkunk_dodo,pkunk_fool,pkunk_idiot,pkunk_jerk,pkunk_loser,pkunk_moron,pkunk_nerd,pkunk_nitwit,pkunk_stupid,pkunk_twit,pkunk_wimp,pkunk_worm};

void SetFury(pPlayer pl)
{
	pl->crew=8;
	pl->maxcrew=8;
	pl->batt=12;
	pl->maxbatt=12;

	pl->maxspeed=64;

	pl->accel_inc=16;

	pl->firebatt=1;
	pl->specbatt=0;

	pl->offset=13;

	pl->batt_wait=0;
	pl->turn_wait=0;
	pl->thrust_wait=0;
	pl->weapon_wait=0;
	pl->special_wait=16;
	pl->batt_regen=0;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=12+o;

	pl->range=120;

	pl->fireangle=100;

	pl->firefunc=&FireFury;
	pl->specfunc=&SpecialFury;
	pl->aifunc=&aiFury;
	pl->loadfunc=&LoadFury;
	pl->loadpilots=&SetPkunkPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXFury;

	pl->ditty=&pkunk_ditty;

	pl->ship_flags = FIRES_FORE | FIRES_LEFT | FIRES_RIGHT;
	pl->mass=1;
	pl->pilot_sprite=(1024+32)/16;

	pl->pilots[0].x=27;
	pl->pilots[0].y=15;
	pl->pilots[1].x=4;
	pl->pilots[1].y=22;
	pl->pilots[2].x=8;
	pl->pilots[2].y=6;
	pl->pilots[3].x=0;
	pl->pilots[3].y=0;
	pl->pilots[4].x=15;
	pl->pilots[4].y=0;

}

void LoadFury(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
    {
       	OAMData[loop] = pkunkData[loop-OAMStart];
       	OAMData[loop+512] = pkunk_outData[loop-OAMStart];
	}
    for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+1024] = pkunk_fireData[loop-OAMStart]; //loads some garbv
    }

     for (loop=OAMStart ;loop<OAMStart+1024;loop++)
		OAMData[loop+1024+32] = pkunkpilotData[loop-OAMStart];

	 for (loop=OAMStart ;loop<OAMStart+256;loop++)
		OAMData[loop+(1024*2)+32] = pkunkpilotlData[loop-OAMStart];
	 for (loop=OAMStart ;loop<OAMStart+64;loop++)
		OAMData[loop+(1024*2)+32+256] = pkunkpilotrData[loop-OAMStart];
	 for (loop=OAMStart ;loop<OAMStart+128;loop++)
		OAMData[loop+(1024*2)+32+256+64] = pkunkpilottData[loop-OAMStart];
	 for (loop=OAMStart ;loop<OAMStart+1024;loop++)
		OAMData[loop+(1024*2)+32+256+64+128] = pkunkpilotfData[loop-OAMStart];
	 for (loop=OAMStart ;loop<OAMStart+512;loop++)
		OAMData[loop+(1024*3)+32+256+64+128] = pkunkpilotsData[loop-OAMStart];


}


int FireFuryA(pPlayer pl,s32 angle)
{
	//s16 n = nextFireSprite(pl);
	s16 b = nextWeapon(pl);

	if (b>=0)//&&n>=0)
	{
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=5;//range
	pl->weapon[b].damage=-1;
	pl->weapon[b].angle=0;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;
	pl->weapon[b].size=8;
	//this determines the middle of the ship which aint so good

	pl->weapon[b].xpos = pl->xpos;//+(s32)(15 * SIN[angle])>>8;
	pl->weapon[b].ypos = pl->ypos;//-(s32)(15 * COS[angle])>>8 ;

	//drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
	//	pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	s32 speed=24;
	pl->weapon[b].xspeed = (s32)(speed * SIN[angle])>>7;
	pl->weapon[b].yspeed = (s32)(speed * COS[angle])>>7;

	//pl->weapon[b].xpos+=pl->weapon[b].xspeed;
	//pl->weapon[b].ypos-=pl->weapon[b].yspeed;

	//pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle])>>8)/3;
	//pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle])>>8)/3;


	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,8);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
    return 1;
	}
	return 0;
}

int FireFury(pPlayer pl)
{
	int r1=0;
	int r2=0;
	int r3=0;
	r1=FireFuryA( pl,pl->angle);

	s32 angle=pl->angle-90;
	if (angle<0)
		angle+=360;
	r2=FireFuryA( pl,angle);
	angle=pl->angle+90;
	if (angle>360)
		angle-=360;
	r3=FireFuryA( pl,angle);

	int r=r1||r2||r3;

	if (r)
		play_sfx(&pkunk_bullet,pl->plr-1);

	return (r);
}

int SpecialFury(pPlayer pl)
{

	ModifyBatt(pl,2);
	int r=ran(0,12);
	play_sfx(&insults[r],pl->plr-1);

	return 1;
}

int DeathFury(pPlayer pl)
{
	int r=ran(0,1);
	if (r&&pl->ai<WEAK)
	{
		//SetFury(pl);
		SetShip(pl);
		SetNew(pl);
		UpdateCrew(pl);
		UpdateBatt(pl);
		pl->warp=0;
		pl->xspeed=0;
		pl->yspeed=0;
		pl->reinc=3;
		play_sfx(&pkunk_ress,pl->plr-1);
		return 1;
	}
	return 0;
}


int aiFury(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	if (ai->batt<ai->maxbatt&&ai->special_turn==0&&ran(0,150)<20*ai->ai)
		ai->ship_input_state |= SPECIAL;

	ship_intelligence(ai,ObjectsOfConcern, ConcernCounter);
}

void SetPkunkPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
		sprites[43+off].attribute1 = SIZE_32 | 240;
		sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

		sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
		sprites[44+off].attribute1 = SIZE_8 | 240;
		sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

		sprites[45+off].attribute0 = COLOR_256 | SQUARE  | 160;
		sprites[45+off].attribute1 = SIZE_16 | 240;
		sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+84 | PRIORITY(2);

		sprites[46+off].attribute0 = COLOR_256 | TALL  | 160;
		sprites[46+off].attribute1 = SIZE_64 | 240;
		sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+92 | PRIORITY(2);

		sprites[47+off].attribute0 = COLOR_256 | SQUARE  | 160;
		sprites[47+off].attribute1 = SIZE_32 | 240;
		sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+156 | PRIORITY(2);
}

void RestoreGFXFury(pPlayer p)
{
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			if(p->weapon[i].type==SIMPLE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}
		}
	}
}
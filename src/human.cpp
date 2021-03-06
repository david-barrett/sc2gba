#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "human_out.h"
#include "human_miss.h"
#include "human_pd.h"


#include "human_sfx.h"

#include "humanpilot.h"
#include "humanpilotl.h"
#include "humanpilotr.h"
#include "humanpilott.h"
#include "humanpilotf.h"
#include "humanpilots.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;
extern pAsteroid asteroids;
//extern s32 planet.xpos,planet.ypos;
extern Object planet;

int FireHuman(pPlayer pl);
int SpecialHuman(pPlayer pl);
int aiHuman(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetHumanPilot(pPlayer p);
void RestoreGFXHuman(pPlayer p);
void MoveHumanMissile(pWeapon ur);
void Posthuman(pPlayer pl);

#define MAX_CREW 18
#define MAX_ENERGY 18
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 9
#define SPECIAL_ENERGY_COST 4
#define ENERGY_WAIT 8
#define MAX_THRUST /* DISPLAY_TO_WORLD (6) */ SHIP_SPEED(24)
#define THRUST_INCREMENT /* DISPLAY_TO_WORLD (2) */ SHIP_SPEED(3)
#define TURN_WAIT 1
#define THRUST_WAIT 4
#define WEAPON_WAIT 10
#define SPECIAL_WAIT 9

#define SHIP_MASS 6
#define MISSILE_LIFE 60
#define MIN_MISSILE_SPEED 10
#define MAX_MISSILE_SPEED 20

#define MISSILE_SPEED (MAX_THRUST >= MIN_MISSILE_SPEED ? \
										MAX_THRUST : MIN_MISSILE_SPEED)
#define TRACK_WAIT 3
#define MISSILE_DAMAGE 4

void LoadHuman(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = humanData[loop-OAMStart];
       		OAMData[loop+512] = human_outData[loop-OAMStart];
       		OAMData[loop+512+512] = human_missData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512+512] = human_pdData[loop-OAMStart]; //loads some garb
   	}

	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+128+512] = humanpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		OAMData[loop+(1024*2)+128+512] = humanpilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+256+512] = humanpilotrData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+32;loop++)
	{
		OAMData[loop+(1024*2)+256+512+128] = humanpilottData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+256+512+128+32] = humanpilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+128+32] = humanpilotsData[loop-OAMStart];

	}

}

int FireHuman(pPlayer pl)
{

	s16 b= nextWeapon(pl);
	int ret=0;

	if (b>=0)
	{
		pl->weapon[b].type=MISSILE;
		pl->weapon[b].movefunc=&MoveHumanMissile;
		pl->weapon[b].object.life=1;
		pl->weapon[b].status=MISSILE_LIFE;
		pl->weapon[b].damage=MISSILE_DAMAGE*-1;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=0;

		pl->weapon[b].object.size=32;
		pl->weapon[b].object.actualangle = pl->object.actualangle;
		pl->weapon[b].object.angle=(pl->weapon[b].object.actualangle*45)>>1;

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
		sprites[pl->weapon[b].sprite].attribute1 =SIZE_32 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);
		ret=1;
		play_sfx(&human_missile,pl->plr-1);
	}



	return ret;
}

void SetHuman(pPlayer pl)
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

	pl->object.mass_points=SHIP_MASS;


	pl->offset=14;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=MISSILE_SPEED*MISSILE_LIFE;

	pl->firefunc=&FireHuman;
	pl->specfunc=&SpecialHuman;
	pl->aifunc=&aiHuman;
	pl->loadfunc=&LoadHuman;
	pl->loadpilots=&SetHumanPilot;
	pl->postfunc=&Posthuman;
	pl->restorefunc=&RestoreGFXHuman;

	pl->ditty=&human_ditty;

	pl->ship_flags = FIRES_FORE | SEEKING_WEAPON | POINT_DEFENSE;

	pl->pilot_sprite=(1024+128+512)/16;
	pl->pilots[0].x=37;
	pl->pilots[0].y=9;
	pl->pilots[1].x=37;
	pl->pilots[1].y=8;
	pl->pilots[2].x=46;
	pl->pilots[2].y=5;
	pl->pilots[3].x=3;
	pl->pilots[3].y=0;
	pl->pilots[4].x=25;
	pl->pilots[4].y=0;


}

void CreatePD(pPlayer pl,s16 x,s16 y,s16 a)
{
	s16 b= nextWeapon(pl);


	if (b>=0)
	{
		pl->weapon[b].type=LASER;
		pl->weapon[b].movefunc=0;
		pl->weapon[b].object.life=1;
		pl->weapon[b].damage=0;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=0;

		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].object.size=16;
		pl->weapon[b].object.angle = a;

		pl->weapon[b].object.xspeed = 0;
		pl->weapon[b].object.yspeed = 0;


		pl->weapon[b].turn_wait=0;


		pl->weapon[b].object.xpos = x;
		pl->weapon[b].object.ypos = y;

		drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
				pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,pl->weapon[b].object.size);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 =SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);


		}





}


int SpecialHuman(pPlayer pl)
{
	pPlayer opp=(pPlayer)pl->opp;

	s16 x1,y1,x2,y2;

	x1=pl->object.xpos;//+pl->object.xspeed;
	y1=pl->object.ypos;//-pl->object.yspeed;


	//oppent
	x2=opp->object.xpos;//+opp->object.xspeed;
	y2=opp->object.ypos;//-opp->object.yspeed;
	if (distanceBetweenPoints(x1,y1,x2,y2)<16+opp->offset)
	{
		CreatePD(pl,(x1+x2)>>1,(y1+y2)>>1,FindAngle(x1,y1,x2,y2));
		ModifyCrew(opp,-1);
	}

	//weapons
	for (int i=0;i<12;i++)
	{
		if (opp->weapon[i].object.life>0)
		{
		x2=opp->weapon[i].object.xpos+opp->weapon[i].object.xspeed;
		y2=opp->weapon[i].object.ypos-opp->weapon[i].object.yspeed;
		if (distanceBetweenPoints(x1,y1,x2,y2)<16+(opp->weapon[i].object.size>>1))
		{
			CreatePD(pl,(x1+x2)>>1,(y1+y2)>>1,FindAngle(x1,y1,x2,y2));
			opp->weapon[i].object.life--;
		}
		}
	}

	//asteroids
	for (int i=0;i<5;i++)
	{
		if (asteroids[i].object.life>0)
		{
		x2=asteroids[i].object.xpos+asteroids[i].object.xspeed;
		y2=asteroids[i].object.ypos-asteroids[i].object.yspeed;
		if (distanceBetweenPoints(x1,y1,x2,y2)<24)
		{
			CreatePD(pl,(x1+x2)>>1,(y1+y2)>>1,FindAngle(x1,y1,x2,y2));
			asteroids[i].object.life--;
		}
		}
	}


	//planet
	x2=planet.xpos;
	y2=planet.ypos;
	if (distanceBetweenPoints(x1,y1,x2,y2)<48)
	{
		CreatePD(pl,(x1+x2)>>1,(y1+y2)>>1,FindAngle(x1,y1,x2,y2));
	}






	play_sfx(&human_point,pl->plr-1);
	return 1;

}

int aiHuman(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;


		if (ai->special_turn == 0
				&& ((ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent != 0
				&& ObjectsOfConcern[ENEMY_WEAPON_INDEX].which_turn <= 2)
				|| (ObjectsOfConcern[ENEMY_SHIP_INDEX].parent != 0
				&& ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn <= 4)))
			ai->ship_input_state |= SPECIAL;
		else
			ai->ship_input_state &= ~SPECIAL;
		ObjectsOfConcern[ENEMY_WEAPON_INDEX].parent =0;

		ship_intelligence (ai,
				ObjectsOfConcern, ConcernCounter);

		if (ai->weapon_turn == 0)
		{
			if (ObjectsOfConcern[ENEMY_SHIP_INDEX].parent
					&& (!(ai->ship_input_state & (LEFT | RIGHT /* | THRUST */))
					|| ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn <= 12))
				ai->ship_input_state |= WEAPON;
	}



}

void SetHumanPilot(pPlayer pl)
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
	sprites[45+off].attribute1 =SIZE_8 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |WIDE  | 160;
	sprites[46+off].attribute1 =SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+82 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 =SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+98 | PRIORITY(2);
}

void  MoveHumanMissile(pWeapon ur)
{
	pPlayer target=(pPlayer)ur->target;

		if (DetectWeaponToShip(target,ur)==1)
				{
					ModifyCrew(target,ur->damage);
		//			ur->object.life==0;
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
				ur->object.life=0;
				return;
			}


			angle = FindAngle(ur->object.xpos,ur->object.ypos,target->object.xpos,target->object.ypos);

				int ret=TurnAngle(angle,ur->object.angle,15);
				if (ret==0)
				{


					s32 x = ((MISSILE_SPEED) * (s32)SIN[ur->object.angle])>>8;
					s32 y = ((MISSILE_SPEED) * (s32)COS[ur->object.angle])>>8;


					ur->object.xspeed = x;//(ur->object.xspeed + x)/2;
					ur->object.yspeed = y;//(ur->object.yspeed + y)/2;
				}
				else if (ret<0)
				{
					ur->object.actualangle--;
					if (ur->object.actualangle==-1)
						ur->object.actualangle=15;
				}
				else if (ret>0)
				{
					ur->object.actualangle++;
					if (ur->object.actualangle==16)
						ur->object.actualangle=0;
				}
				ur->object.angle=(ur->object.actualangle*45)>>1;


			ur->turn_wait = TRACK_WAIT;
		}
	

}

void RestoreGFXHuman(pPlayer p)
{
	//only restore limpets
	//im sure laser will be fine...
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].object.life>0)
		{
			if(p->weapon[i].type==MISSILE)
			{
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 =SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}



		}
	}
}
void Posthuman(pPlayer pl)
{

}

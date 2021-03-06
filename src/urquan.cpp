#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "urquan_out.h"  //urqan
#include "urquan_fire.h"
#include "urquan_fighters.h"
#include "urquan_fighters_fire.h"

//#include "sound/urquan-fusion.h"
//#include "sound/urquan-fighter.h"
//#include "sound/urquan-recover.h"
//#include "sound/urquan_launch.h"

#include "urquan_sfx.h"

#include "urquanpilot.h"
#include "urquanpilotl.h"
#include "urquanpilotr.h"
#include "urquanpilott.h"
#include "urquanpilotf.h"
#include "urquanpilots.h"

#define MAX_CREW 42
#define MAX_ENERGY 42
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 6
#define SPECIAL_ENERGY_COST 8
#define ENERGY_WAIT 4
#define MAX_THRUST SHIP_SPEED(30)
#define THRUST_INCREMENT SHIP_SPEED (6)
#define TURN_WAIT 4
#define THRUST_WAIT 6
#define WEAPON_WAIT 6
#define SPECIAL_WAIT 9

#define SHIP_MASS 10
#define MISSILE_SPEED 20 //DISPLAY_TO_WORLD (20)
#define MISSILE_LIFE 20
#define MISSILE_RANGE  MISSILE_SPEED*MISSILE_LIFE

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireDreadnaught(pPlayer pl);
int SpecialDreadnaught(pPlayer pl);
int aiDreadnaught(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetUrquanPilot(pPlayer pl);
void RestoreGFXDreadnaught(pPlayer p);
int FightersFire(pWeapon f,s16 angle);
void MoveFighters(pWeapon ur);

void LoadDreadnaught(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = urquanData[loop-OAMStart];
       		OAMData[loop+512] = urquan_outData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512] = urquan_fireData[loop-OAMStart]; //loads some garb
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+512+128+512] = urquan_fightersData[loop-OAMStart]; //loads some garb
   	}

   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+512+128+32+512] = urquan_fighters_fireData[loop-OAMStart]; //loads some garb
   	}

   	 for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	 {
			OAMData[loop+1216] = urquanpilotData[loop-OAMStart];
	 }

	 for (loop=OAMStart ;loop<OAMStart+256;loop++)
			OAMData[loop+1024+1216] = urquanpilotlData[loop-OAMStart];

	for (loop=OAMStart ;loop<OAMStart+32;loop++)
			OAMData[loop+1024+1216+256] = urquanpilotrData[loop-OAMStart];
	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
			OAMData[loop+1024+1216+32+256] = urquanpilottData[loop-OAMStart];
			OAMData[loop+1024+1216+32+512] = urquanpilotfData[loop-OAMStart];
	}
	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
			OAMData[loop+1024+1216+32+512+256] = urquanpilotsData[loop-OAMStart];


}

int SpecialDreadnaught(pPlayer pl)
{
	//s16 n = nextSpecSprite(pl);
		s16 b;
		int res=0;
		for (int i =0;i<2;i++)
		{
		b= nextWeapon(pl,4,11);

		if (b>=0&&pl->crew>1)//&&n>=0)
		{
		pl->weapon[b].type=UR_FIGHTERS;
		pl->weapon[b].object.life=1;//range
		pl->weapon[b].damage=4;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;
		pl->weapon[b].movefunc=&MoveFighters;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=1;
		
		//pl->weapon[b].sprite=n;
		pl->weapon[b].object.actualangle = pl->object.actualangle;
		pl->weapon[b].object.angle=(pl->weapon[b].object.actualangle*45)>>1;
		pl->weapon[b].object.xpos = pl->object.xpos+((s32)(24 * (s32)SIN[pl->object.angle]));
		pl->weapon[b].object.ypos = pl->object.ypos-((s32)(24 * (s32)COS[pl->object.angle]));
		pl->weapon[b].object.size=8;
		pl->weapon[b].status=400;

		//this determines the middle of the ship which aint so good
		//pl->weapon[b].object.xpos = pl->object.xpos + 24;
		//pl->weapon[b].object.ypos = pl->object.ypos + 24;

		int e;
		e=(i==0?125:205);

		s16 a = pl->object.angle+e;
		if (a>360)
			a-=360;




		s32 x = ((pl->offset) * (s32)SIN[a])>>7;
		s32 y = ((pl->offset) * (s32)COS[a])>>7;

		pl->weapon[b].object.xpos=pl->object.xpos+(x);
		pl->weapon[b].object.ypos=pl->object.ypos-(y);

		drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
			pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,8);

		s32 speed=0;
		pl->weapon[b].object.xspeed = 0;//(speed * (s32)SIN[pl->object.angle])>>8;
		pl->weapon[b].object.yspeed = 0;//(speed * (s32)COS[pl->object.angle])>>8;

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[pl->weapon[b].sprite].attribute1 =SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+72 | PRIORITY(1);
	    //sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart | PRIORITY(1);
	    ModifyCrew(pl,-1);
	    res+=1;
		}


	}
	if (res>0)
		play_sfx(&urquan_launch,pl->plr-1);
	return res;
}

int FightersFire(pWeapon f,s16 angle)
{
	//we know we are in range
	//so just draw gfx and damage
	pPlayer pl=(pPlayer)f->parent;
	pPlayer target=(pPlayer)f->target;

	s16 b= nextWeapon(pl);

	if (b!=-1)
	{

		pl->weapon[b].type=UR_FIGHTERS_FIRE;
		pl->weapon[b].object.life=1;//range
		pl->weapon[b].object.angle =angle;
		pl->weapon[b].object.xpos = f->object.xpos;
		pl->weapon[b].object.ypos = f->object.ypos;
		pl->weapon[b].object.size=8;
		pl->weapon[b].object.xspeed = 0;
		pl->weapon[b].object.yspeed = 0;
		pl->weapon[b].movefunc=0;
		pl->weapon[b].hitfunc=0;
		pl->weapon[b].object.ignorecollision=0;

		drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
				pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,8);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 =SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+74 | PRIORITY(0);
	}
    ModifyCrew(target,-1);
    play_sfx(&urquan_fighter,pl->plr-1);
	return 1;

}

void SetDreadnaught(pPlayer pl)
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
		pl->offset=16;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=MISSILE_RANGE;

	pl->firefunc=&FireDreadnaught;
	pl->specfunc=&SpecialDreadnaught;
	pl->aifunc=&aiDreadnaught;
	pl->loadfunc=&LoadDreadnaught;
	pl->loadpilots=&SetUrquanPilot;
	pl->postfunc=0;
	pl->restorefunc=&RestoreGFXDreadnaught;

	pl->ditty=&urquan_ditty;
	pl->batt_regen=1;

	pl->ship_flags = FIRES_FORE | SEEKING_SPECIAL;

	pl->pilot_sprite=1216/16;
	pl->pilots[0].x=17;
	pl->pilots[0].y=15;
	pl->pilots[1].x=28;
	pl->pilots[1].y=23;
	pl->pilots[2].x=4;
	pl->pilots[2].y=15;
	pl->pilots[3].x=27;
	pl->pilots[3].y=15;
	pl->pilots[4].x=0;
	pl->pilots[4].y=0;
}

int FireDreadnaught(pPlayer pl)
{

	s16 b = nextWeapon(pl,0,3);

	if (b>=0)
	{
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].object.life=20;//range
	pl->weapon[b].damage=-6;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;
	pl->weapon[b].movefunc=0;
	pl->weapon[b].hitfunc=0;
	pl->weapon[b].object.ignorecollision=0;

	pl->weapon[b].object.size=16;
	pl->weapon[b].object.angle = pl->object.angle;

	s32 speed=40;//20;
	pl->weapon[b].object.xspeed = ((speed * (s32)SIN[pl->object.angle])>>8);///SPEED_REDUCT;
	pl->weapon[b].object.yspeed = ((speed * (s32)COS[pl->object.angle])>>8);///SPEED_REDUCT;

	pl->weapon[b].object.xpos = pl->object.xpos+((pl->offset * (s32)SIN[pl->object.angle])>>8);//3;
	pl->weapon[b].object.ypos = pl->object.ypos-((pl->offset * (s32)COS[pl->object.angle])>>8);//3;

	#ifdef MISSILE_START
	pl->weapon[b].object.xpos-=pl->weapon[b].object.xspeed;
	pl->weapon[b].object.ypos+=pl->weapon[b].object.yspeed;
	#endif

	drawOnScreen(&pl->weapon[b].object.xscreen,&pl->weapon[b].object.yscreen,
		pl->weapon[b].object.xpos,pl->weapon[b].object.ypos,screenx,screeny,pl->weapon[b].object.size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].object.yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 =SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].object.xscreen;
    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(1);

	play_sfx(&urquan_fusion,pl->plr-1);
    return 1;
	}
	return 0;
}

int aiDreadnaught(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{

		pObject lpEvalDesc;

		ObjectsOfConcern[ENEMY_SHIP_INDEX].MoveState = PURSUE;
		lpEvalDesc = &ObjectsOfConcern[ENEMY_WEAPON_INDEX];
		if (lpEvalDesc->parent
				&& lpEvalDesc->MoveState == ENTICE
				&& (!(lpEvalDesc->type == CREW)
				|| lpEvalDesc->which_turn <= 8))
			//	&& (!(lpEvalDesc->tr->state_flags & FINITE.object.life)
				//|| (lpEvalDesc->tr->object.object.mass_points_points_points >= 4
			//	&& lpEvalDesc->which_turn == 2
			//	&& ObjectsOfConcern[ENEMY_SHIP_INDEX].which_turn > 16)))
			lpEvalDesc->MoveState = PURSUE;

		ship_intelligence(ai,ObjectsOfConcern, ConcernCounter);



		lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
		{

			pPlayer opp = (pPlayer)ai->opp;
			if (ai->special_turn == 0
					&& lpEvalDesc->parent
					&& ai->crew > 10
					&& !(opp->ship_flags	& POINT_DEFENSE)
					&& (ai->special_wait < 6
					|| opp->ManeuverabilityIndex <= SLOW_SHIP
					&& !(opp->speed<opp->maxspeed))
					|| (lpEvalDesc->which_turn <= 12
					&& (ai->ship_input_state & (LEFT | RIGHT))
					&& ai->batt >= 20))
				ai->ship_input_state |= SPECIAL;
			else
				ai->ship_input_state &= ~SPECIAL;
		}


}

void MoveFighters(pWeapon ur)
{
	s16 angle;
	pPlayer target=(pPlayer)ur->target;

	pPlayer parent=(pPlayer)ur->parent;
	ur->status--;

	//if out too long return
	if (ur->status==0)
	{
		//been out too long die
		ur->object.life=0;
		MoveOffscreen(&sprites[ur->sprite]);

		return;
	}
	if (target->crew==0&&ur->status>149)
		ur->status=149;
	if (ur->status<150||target->cloak)
	{
		//if reached mother ship dock
		if (DetectWeaponToShip(parent,ur)==1)
		{
			ur->object.life=0;//-1;
			ModifyCrew(parent,1);
			MoveOffscreen(&sprites[ur->sprite]);
			play_sfx(&urquan_recover,parent->plr-1);
			return;
		}
		//desired.object.angle = mothership
	angle = FindAngle(ur->object.xpos,ur->object.ypos,parent->object.xpos,parent->object.ypos);

	}
	else
	{

		//if in range fire
		if (InRange(ur->object.xpos,ur->object.ypos,target->object.xpos,target->object.ypos,8+(target->offset/2)))//calc dist from target
		{
			if (ur->status%3==0) //otherwise const firing
			{
				//fire -
				FightersFire(ur,FindAngle(ur->object.xpos,ur->object.ypos,target->object.xpos,target->object.ypos));
			}
		angle=target->object.angle; // turn to match target.object.angle;
		}
		else // not in range
		{

			// turn towards opp
		angle = FindAngle(ur->object.xpos,ur->object.ypos,target->object.xpos,target->object.ypos);

		}
	}

	int ret=TurnAngle(angle,ur->object.angle,15);
	if (ret==0)
	{
		s32 x = ((6) * (s32)SIN[ur->object.angle])>>8;
				s32 y = ((6) * (s32)COS[ur->object.angle])>>8;

				ur->object.xspeed = (ur->object.xspeed + x)/2;
		ur->object.yspeed = (ur->object.yspeed + y)/2;
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
/*
	//now calc if should turn
	s32 a =.object.angle+360;
	s32 b = ur->object.angle+360;

	if (a<b)
	{
		ur->object.angle-=15;
							if (ur->object.angle<0)
			ur->object.angle+=360
	}
	else if (b<a)
	{
		ur->object.angle+=15;
		if (ur->object.angle>360)
			ur->object.angle-=360;
	}


	//if facing right way thrust
	s16 a1 = 360.object.angle;
	s16 a2 = 360+ur->object.angle;
	if (a1>a2-30&&a1<a2+30)//thrust if going roughly the right way
	{
		s32 x = ((3) * (s32)SIN[ur->object.angle])>>8;
		s32 y = ((3) * (s32)COS[ur->object.angle])>>8;

		ur->object.xspeed = (ur->object.xspeed + x)/2;
		ur->object.yspeed = (ur->object.yspeed + y)/2;
	}
*/
	//always do
	ur->object.xpos+=ur->object.xspeed;
	ur->object.ypos-=ur->object.yspeed;
}

void SetUrquanPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 =SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 =SIZE_8 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[45+off].attribute1 =SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+82 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[46+off].attribute1 =SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+98 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 =SIZE_64 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+114 | PRIORITY(2);
}

void RestoreGFXDreadnaught(pPlayer p)
{
	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].object.life>0)
		{
			if(p->weapon[i].type==UR_FIGHTERS)
			{

				sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
				sprites[p->weapon[i].sprite].attribute1 =SIZE_8 | ROTDATA(p->weapon[i].sprite) | 240;
	    		sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+72 | PRIORITY(1);
			}
			else if(p->weapon[i].type==SIMPLE)
			{

				sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG |SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			    sprites[p->weapon[i].sprite].attribute1 =SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
    			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(1);
			}

		}
	}
}

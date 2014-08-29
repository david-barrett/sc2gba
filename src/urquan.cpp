#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "gfx/urquan_out.h"  //urqan
#include "gfx/urquan_fire.h"
#include "gfx/urquan_fighters.h"
#include "gfx/urquan_fighters_fire.h"

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






extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireDreadnaught(pPlayer pl);
int SpecialDreadnaught(pPlayer pl);
int aiDreadnaught(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetUrquanPilot(pPlayer pl);

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
		pl->weapon[b].life=1;//range
		pl->weapon[b].damage=4;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;
		//pl->weapon[b].sprite=n;
		pl->weapon[b].angle = pl->angle;
		pl->weapon[b].xpos = pl->xpos+((s32)(24 * (s32)SIN[pl->angle]));
		pl->weapon[b].ypos = pl->ypos-((s32)(24 * (s32)COS[pl->angle]));
		pl->weapon[b].size=8;
		pl->weapon[b].status=400;

		//this determines the middle of the ship which aint so good
		//pl->weapon[b].xpos = pl->xpos + 24;
		//pl->weapon[b].ypos = pl->ypos + 24;

		int e;
		e=(i==0?125:205);

		s16 a = pl->angle+e;
		if (a>360)
			a-=360;


		s32 x = ((pl->offset) * (s32)SIN[a])>>8;
		s32 y = ((pl->offset) * (s32)COS[a])>>8;

		pl->weapon[b].xpos=pl->xpos+(x);
		pl->weapon[b].ypos=pl->ypos-(y);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,8);

		s32 speed=0;
		pl->weapon[b].xspeed = 0;//(speed * (s32)SIN[pl->angle])>>8;
		pl->weapon[b].yspeed = 0;//(speed * (s32)COS[pl->angle])>>8;

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+72 | PRIORITY(1);
	    //sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart | PRIORITY(1);
	    ModifyCrew(pl,-1);
	    res+=1;
	    pl->fighters++;
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
		pl->weapon[b].life=1;//range
		pl->weapon[b].angle = angle;
		pl->weapon[b].xpos = f->xpos;
		pl->weapon[b].ypos = f->ypos;
		pl->weapon[b].size=8;
		pl->weapon[b].xspeed = 0;
		pl->weapon[b].yspeed = 0;

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
				pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,8);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	    sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+74 | PRIORITY(0);
	}
    ModifyCrew(target,-1);
    play_sfx(&urquan_fighter,pl->plr-1);
	return 1;

}

void SetDreadnaught(pPlayer pl)
{
	pl->crew=42;
	pl->maxcrew=42;
	pl->batt=42;
	pl->maxbatt=42;

	pl->maxspeed=30;

	pl->accel_inc=6;

	pl->firebatt=6;
	pl->specbatt=8;

	pl->offset=30;

	pl->batt_wait=4;
	pl->turn_wait=4;
	pl->thrust_wait=6;
	pl->weapon_wait=6;
	pl->special_wait=9;

	s16 o = (pl->plr-1)*13;

	pl->ffiresprite=1+o;
	pl->lfiresprite=4+o;

	pl->fspecsprite=5+o;
	pl->lspecsprite=12+o;

	pl->range=200;

	pl->fireangle=45;

	pl->firefunc=&FireDreadnaught;
	pl->specfunc=&SpecialDreadnaught;
	pl->aifunc=&aiDreadnaught;
	pl->loadfunc=&LoadDreadnaught;
	pl->loadpilots=&SetUrquanPilot;

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
	pl->weapon[b].life=20;//range
	pl->weapon[b].damage=-6;
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=16;
	pl->weapon[b].angle = pl->angle;

	s32 speed=40;//20;
	pl->weapon[b].xspeed = ((speed * (s32)SIN[pl->angle])>>9);///SPEED_REDUCT;
	pl->weapon[b].yspeed = ((speed * (s32)COS[pl->angle])>>9);///SPEED_REDUCT;

	pl->weapon[b].xpos = pl->xpos+((52 * (s32)SIN[pl->angle])>>8)/3;
	pl->weapon[b].ypos = pl->ypos-((52 * (s32)COS[pl->angle])>>8)/3;

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
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
			//	&& (!(lpEvalDesc->ObjectPtr->state_flags & FINITE_LIFE)
				//|| (lpEvalDesc->ObjectPtr->mass_points >= 4
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

void MoveURFighters(pWeapon ur)
{
	s16 angle;
	pPlayer target=(pPlayer)ur->target;

	pPlayer parent=(pPlayer)ur->parent;
	ur->status--;

	//if out too long return
	if (ur->status==0)
	{
		//been out too long die
		ur->life=0;
		MoveOffscreen(&sprites[ur->sprite]);
		parent->fighters--;

		return;
	}
	if (target->crew==0&&ur->status>199)
		ur->status=199;
	if (ur->status<200||target->cloak)
	{
		//if reached mother ship dock
		if (DetectWeaponToShip(parent,ur)==1)
		{
			ur->life=0;//-1;
			parent->fighters--;
			//mothership crew++;
			ModifyCrew(parent,1);
			MoveOffscreen(&sprites[ur->sprite]);
			play_sfx(&urquan_recover,parent->plr-1);
			return;
		}
		//desired angle = mothership
		angle = FindAngle(ur->xpos,ur->ypos,parent->xpos,parent->ypos);

	}
	else
	{

		//if in range fire
		if (InRange(ur->xpos,ur->ypos,target->xpos,target->ypos,8+(target->offset/2)))//calc dist from target
		{
			if (ur->status%3==0) //otherwise const firing
			{
				//fire -
				FightersFire(ur,FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos));
			}
			angle=target->angle; // turn to match target angle;
		}
		else // not in range
		{

			// turn towards opp
			angle = FindAngle(ur->xpos,ur->ypos,target->xpos,target->ypos);

		}
	}

	int ret=TurnAngle(angle,ur->angle,15);
	if (ret==0)
	{
		s32 x = ((6) * (s32)SIN[ur->angle])>>8;
				s32 y = ((6) * (s32)COS[ur->angle])>>8;

				ur->xspeed = (ur->xspeed + x)/2;
		ur->yspeed = (ur->yspeed + y)/2;
	}
	else if (ret<0)
		{
			ur->angle-=15;
					if (ur->angle<0)
			ur->angle+=360;
	}
	else if (ret>0)
		{
			ur->angle+=15;
					if (ur->angle>360)
			ur->angle-=360;
	}
/*
	//now calc if should turn
	s32 a = angle+360;
	s32 b = ur->angle+360;

	if (a<b)
	{
		ur->angle-=15;
							if (ur->angle<0)
			ur->angle+=360
	}
	else if (b<a)
	{
		ur->angle+=15;
		if (ur->angle>360)
			ur->angle-=360;
	}


	//if facing right way thrust
	s16 a1 = 360+angle;
	s16 a2 = 360+ur->angle;
	if (a1>a2-30&&a1<a2+30)//thrust if going roughly the right way
	{
		s32 x = ((3) * (s32)SIN[ur->angle])>>8;
		s32 y = ((3) * (s32)COS[ur->angle])>>8;

		ur->xspeed = (ur->xspeed + x)/2;
		ur->yspeed = (ur->yspeed + y)/2;
	}
*/
	//always do
	ur->xpos+=ur->xspeed;
	ur->ypos-=ur->yspeed;
}

void SetUrquanPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | SQUARE  | 160;
	sprites[44+off].attribute1 = SIZE_8 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+82 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[46+off].attribute1 = SIZE_32 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+98 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_64 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+114 | PRIORITY(2);
}

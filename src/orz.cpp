#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "orz_out.h"
#include "orz_body.h"
#include "orz_turret.h"

#include "orz_bullet.h"
#include "orz_exp.h"

#include "orz_spaceman.h"
#include "orz_status.h"
#include "orz_statusflash.h"


#include "orz_sfx.h"

#include "orzpilot.h"
#include "orzpilotl.h"
#include "orzpilotr.h"
#include "orzpilott.h"
#include "orzpilotf.h"
#include "orzpilots.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireOrz(pPlayer pl);
int SpecialOrz(pPlayer pl);
int aiOrz(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetOrzPilot(pPlayer p);
void RestoreGFXOrz(pPlayer p);
void MoveMarine(pWeapon ur);
void PostOrz(pPlayer p);
void MarineStatus(pPlayer orz);

extern s32 zoom;


void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#define MAX_CREW 16
#define MAX_ENERGY 20
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST (MAX_ENERGY / 3)
#define SPECIAL_ENERGY_COST 0
#define ENERGY_WAIT 6
#define MAX_THRUST SHIP_SPEED(35)
#define THRUST_INCREMENT SHIP_SPEED(5)
#define TURN_WAIT 1
#define THRUST_WAIT 0
#define WEAPON_WAIT 4
#define SPECIAL_WAIT 0//12
#define MARINE_WAIT 12

#define SHIP_MASS 4
#define ORZ_OFFSET 9
#define MISSILE_SPEED 30 //DISPLAY_TO_WORLD (30)
#define MISSILE_LIFE 12
#define MISSILE_DAMAGE 3

#define MARINE_SPEED 10 //guess
#define MARINE_ATTACK_WAIT 5

void LoadOrz(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)       
  	{
       		OAMData[loop] = orz_bodyData[loop-OAMStart];
       		OAMData[loop+512] = orz_outData[loop-OAMStart];
			OAMData[loop+1024] = orz_turretData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)        
   	{
		OAMData[loop+1024+512] = orz_bulletData[loop-OAMStart]; 
		OAMData[loop+1024+512+32] = orz_expData[loop-OAMStart];

		OAMData[loop+1024+512+64] = orz_spacemanData[loop-OAMStart];

		OAMData[loop+1024+512+96] = orz_statusData[loop-OAMStart];
		OAMData[loop+1024+512+128] = orz_statusflashData[loop-OAMStart];


   	}

   	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+1024+512+160] = orzpilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+(1024*2)+512+160] = orzpilotlData[loop-OAMStart];
		OAMData[loop+(1024*2)+256+512+160] = orzpilotrData[loop-OAMStart];
		OAMData[loop+(1024*3)+160] = orzpilottData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+128;loop++)
	{
		
		OAMData[loop+(1024*3)+160+256] = orzpilotfData[loop-OAMStart];
		OAMData[loop+(1024*3)+256+160+128] = orzpilotsData[loop-OAMStart];
	}
}

int FireOrz(pPlayer pl)
{

	s16 b= nextWeapon(pl,8,9);
	s16 ret=0;

	if (b>=0)
	{

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=MISSILE_LIFE;	
	pl->weapon[b].damage=-1*MISSILE_DAMAGE; 
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = (pl->turretangle*45)>>1;
	pl->weapon[b].actualangle = pl->turretangle;

	pl->weapon[b].xspeed=((s32)(MISSILE_SPEED * SIN[pl->weapon[b].angle])>>8);
	pl->weapon[b].yspeed=((s32)(MISSILE_SPEED * COS[pl->weapon[b].angle])>>8);;


	pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset * SIN[pl->weapon[b].angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset * COS[pl->weapon[b].angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);

	play_sfx(&orz_cannon,pl->plr-1);
	ret =1;

	b=10;
	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=1;	
	pl->weapon[b].damage=0; 
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = 0;
	pl->weapon[b].actualangle = 0;

	pl->weapon[b].xspeed=0;
	pl->weapon[b].yspeed=0;


	pl->weapon[b].xpos = pl->xpos+((s32)(10 * SIN[(pl->turretangle*45)>>1])>>8);
	pl->weapon[b].ypos = pl->ypos-((s32)(10 * COS[(pl->turretangle*45)>>1])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+98 | PRIORITY(0);

	}

	return ret;
}

void SetOrz(pPlayer pl)
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

		pl->range=1440;

	pl->fireangle=45;

	pl->firefunc=&FireOrz;
	pl->specfunc=&SpecialOrz;
	pl->aifunc=&aiOrz;
	pl->loadfunc=&LoadOrz;
	pl->loadpilots=&SetOrzPilot;
	pl->postfunc=&PostOrz;
	pl->restorefunc=&RestoreGFXOrz;

	pl->ditty=&orz_ditty;

	pl->ship_flags = FIRES_FORE | SEEKING_SPECIAL;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(1024+512+160)/16;
	pl->pilots[0].x=46;
	pl->pilots[0].y=0;
	pl->pilots[1].x=3;
	pl->pilots[1].y=0;
	pl->pilots[2].x=16;
	pl->pilots[2].y=7;
	pl->pilots[3].x=24;
	pl->pilots[3].y=15;
	pl->pilots[4].x=18;
	pl->pilots[4].y=1;

	pl->turretangle=pl->actualangle;

	//turret
	pl->weapon[11].type=TURRET;
	pl->weapon[11].movefunc=0;
	pl->weapon[11].life=10;
	pl->weapon[11].damage=0;
	pl->weapon[11].target=pl->opp;
	pl->weapon[11].parent=pl;
	pl->weapon[11].damageparent=0;
	pl->weapon[11].size=8;//32;
	pl->weapon[11].angle = pl->angle;
	pl->weapon[11].actualangle = pl->actualangle;
	pl->weapon[11].xspeed=0;
	pl->weapon[11].yspeed=0;
	pl->weapon[11].xpos = pl->xpos;
	pl->weapon[11].ypos = pl->ypos;
	
	sprites[pl->weapon[11].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;
	sprites[pl->weapon[11].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[11].sprite) | 240;
	sprites[pl->weapon[11].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(0);

	pl->charging=0;



}

void CreateMarine(pPlayer pl,s16 x,s16 y,s16 status,s16 b)
{
	pl->weapon[b].type=MARINE;
			pl->weapon[b].life=1;	
			pl->weapon[b].status=status;	
			pl->weapon[b].movefunc=&MoveMarine;
			pl->weapon[b].damage=0; 
			pl->weapon[b].target=pl->opp;
			pl->weapon[b].parent=pl;
			pl->weapon[b].damageparent=0;

			pl->weapon[b].size=8;
			pl->weapon[b].angle = 0;
			pl->weapon[b].actualangle = 0;

			pl->weapon[b].xspeed=0;//((s32)(MARINE_SPEED * SIN[pl->weapon[b].angle])>>8);
			pl->weapon[b].yspeed=0;//((s32)(MARINE_SPEED * COS[pl->weapon[b].angle])>>8);;


			pl->weapon[b].xpos = x;
			pl->weapon[b].ypos = y;
	
			drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
				pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

			sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
			sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
			sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+100 | PRIORITY(1);

}

int SpecialOrz(pPlayer pl)
{
	if (pl->ship_input_state & WEAPON)
	{
		s16 b= nextWeapon(pl,0,7);
		//marine
		if (pl->charging==0&&pl->crew>1&&b>=0)
		{
			pl->charging=MARINE_WAIT;
			
			ModifyCrew(pl,-1);

			CreateMarine(pl,pl->xpos+((s32)(pl->offset * SIN[pl->weapon[b].angle])>>8)
				,pl->ypos+((s32)(pl->offset * COS[pl->weapon[b].angle])>>8),200, b);
			
			

			play_sfx(&orz_gogo,pl->plr-1);


		}
	}
	else if (pl->ship_input_state & LEFT)
	{
		//turret left
		pl->turretangle--;
		if (pl->turretangle==-1)
			pl->turretangle=15;		
	}
	else if (pl->ship_input_state & RIGHT)
	{
		pl->turretangle++;
		if (pl->turretangle==16)
			pl->turretangle=0;		
	}
	else 
		return 0;
	
		

	pl->ship_input_state &= ~THRUST;
	pl->ship_input_state &= ~LEFT;
	pl->ship_input_state &= ~RIGHT;

	pl->ship_input_state=0;

	return 1;


}

int count_marines(pPlayer p)
{
	s16 count=8;
	for (int i=0;i<8;i++)
	{
		if (p->weapon[i].life>0)
			count--;
	}
	return count;
}

int aiOrz(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	//++TurretPtr->turn_wait;
	ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);
	//--TurretPtr->turn_wait;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	if (lpEvalDesc->parent == 0)
		ai->ship_input_state &= ~SPECIAL;
	else if (ai->special_turn != 1)
	{
			if (ai->turn_turn == 0
				&& lpEvalDesc->MoveState == ENTICE
				&& lpEvalDesc->which_turn < 24
			//	&& (StarShipPtr->cur_status_flags
			//	& (SHIP_AT_MAX_SPEED | SHIP_BEYOND_MAX_SPEED))
				&& !(ai->ship_input_state & THRUST)
				)
				/*
				&& NORMALIZE_ANGLE (
				GetVelocityTravelAngle (&ShipPtr->velocity)
				- ARCTAN (
						lpEvalDesc->ObjectPtr->next.location.x
						- ShipPtr->next.location.x,
						lpEvalDesc->ObjectPtr->next.location.y
						- ShipPtr->next.location.y
				) + (QUADRANT - (OCTANT >> 1))) >=
				((QUADRANT - (OCTANT >> 1)) << 1))
				*/
			ai->ship_input_state &= ~(LEFT | RIGHT);

		ai->ship_input_state &= ~SPECIAL;
		if (ai->turn_turn == 0
				&& !(ai->ship_input_state & (LEFT | RIGHT | WEAPON)))
				
		{
			
			s16 facing = (ai->turretangle*45)>>1;
			s16 delta_facing;

			if ((delta_facing = TurnAngle(facing,FindAngle(ai->xpos,ai->ypos,
				lpEvalDesc->xpos,lpEvalDesc->ypos),15) > 0))
			{
				ai->ship_input_state |= SPECIAL;
				
				if (delta_facing >1)
					ai->ship_input_state |= RIGHT;
				else
					ai->ship_input_state |= LEFT;
			}
		}

		pPlayer opp=(pPlayer)ai->opp;

		if (ai->charging == 0
				&& !(ai->ship_input_state & WEAPON)
				&& (ai->crew >
				ai->crew >> 2)
				&& !(opp->ship_flags & POINT_DEFENSE)
				&& (opp->ManeuverabilityIndex) < SLOW_SHIP
				|| lpEvalDesc->which_turn <= 12
				|| count_marines (ai) < 2)
		{
			ai->ship_input_state |= WEAPON | SPECIAL;
		}
	}


}

void SetOrzPilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | WIDE | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |SQUARE  | 160;
	sprites[46+off].attribute1 = SIZE_16 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+112 | PRIORITY(1);

	sprites[47+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[47+off].attribute1 = SIZE_16 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+120 | PRIORITY(2);
}

void RestoreGFXOrz(pPlayer p)
{

	for(int i=0;i<12;i++)
	{
		if (p->weapon[i].life>0)
		{
			s16 o;
			if(p->weapon[i].type==SIMPLE)
				o=96;
			else if (p->weapon[i].type==MARINE);
				o=100;
			
			sprites[p->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
			sprites[p->weapon[i].sprite].attribute1 = SIZE_16 | ROTDATA(p->weapon[i].sprite) | 240;
   			sprites[p->weapon[i].sprite].attribute2 = p->SpriteStart+o | PRIORITY(1);
			

		}
	}
}

void PostOrz(pPlayer pl)
{

	//draw turret
	pl->weapon[11].xpos = pl->xpos;
	pl->weapon[11].ypos = pl->ypos;
	pl->weapon[11].life=10;
	sprites[pl->weapon[11].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;
	sprites[pl->weapon[11].sprite].attribute1 = SIZE_32 | ROTDATA(pl->weapon[11].sprite) | 240;
	sprites[pl->weapon[11].sprite].attribute2 = pl->SpriteStart+64 | PRIORITY(0);
	RotateSprite(pl->weapon[11].sprite, (pl->turretangle*45)>>1, zoom, zoom);
	MoveSprite(&sprites[pl->weapon[11].sprite], pl->xscreen, pl->yscreen);

	if (pl->charging>0)
		pl->charging--;

	pPlayer opp = (pPlayer)pl->opp;
	s16 r;

	for (s16 i=0;i<8;i++)
	{
		if (pl->weapon[i].status<0) //have boarded
		{
			if (pl->weapon[i].turn_wait==0)
			{
				pl->weapon[i].turn_wait=MARINE_ATTACK_WAIT;
				r=ran(0,255);
				
				if (r<16)
				{
					pl->weapon[i].status=0;
					pl->weapon[i].life=0;
					MoveOffscreen(&sprites[pl->weapon[i].sprite]);
					play_sfx(&orz_marinedie,pl->plr-1);

				}
			else if (r>127)
			{
				ModifyCrew(opp,-1);
				pl->weapon[i].status=-2;//attacking
				play_sfx(&orz_zap,pl->plr-1);
			}
			}
			else
				pl->weapon[i].turn_wait--;
		}
		if (opp->EndGame==20&&opp->crew<1)
		{
			for (s16 i=0;i<8;i++)
			{
				if (pl->weapon[i].status<0)
					CreateMarine(pl,opp->xpos,opp->ypos,49,i);
			}

		}

	}

	MarineStatus(pl);
	
}

void MoveMarine(pWeapon ur)
{
	s16 angle;
	pPlayer target=(pPlayer)ur->target;

	pPlayer parent=(pPlayer)ur->parent;
	if (ur->status>0)
	{
	ur->status--;

	//if out too long return
	if (ur->status==0)
	{
		//been out too long die
		ur->life=0;
		MoveOffscreen(&sprites[ur->sprite]);

		return;
	}
	if (target->crew==0&&ur->status>50)
		ur->status=149;
	if (ur->status<50||target->cloak)
	{
		//if reached mother ship dock
		if (DetectWeaponToShip(parent,ur)==1)
		{
			ur->life=0;//-1;
			ModifyCrew(parent,1);
			MoveOffscreen(&sprites[ur->sprite]);			
			return;
		}
		//desired angle = mothership
		angle = FindAngle(ur->xpos,ur->ypos,parent->xpos,parent->ypos);

	}
	else
	{

		//if in range board
		if (DetectWeaponToShip(target,ur)==1)
		{
			//board!!
			play_sfx(&orz_intruder,parent->plr-1);
			//AddMarine
			ur->turn_wait=MARINE_ATTACK_WAIT;
			ur->status=-1;
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
		s32 x = ((MARINE_SPEED) * (s32)SIN[ur->angle])>>8;
		s32 y = ((MARINE_SPEED) * (s32)COS[ur->angle])>>8;

		ur->xspeed = (ur->xspeed + x)/2;
		ur->yspeed = (ur->yspeed + y)/2;
	}
	else if (ret<0)
		{
						ur->actualangle--;
						if (ur->actualangle==-1)
							ur->actualangle=15;
					}
					else if (ret>0)
					{
						ur->actualangle++;
						if (ur->actualangle==16)
							ur->actualangle=0;
					}
				ur->angle=(ur->actualangle*45)>>1;
	//always do
	ur->xpos+=ur->xspeed;
	ur->ypos-=ur->yspeed;
	}
}

void MarineStatus(pPlayer orz)
{
	pPlayer opp=(pPlayer)orz->opp;
	s16 s=orz->SpriteStart+102;
	s16 x;
	for (s16 i=0;i<8;i++)
	{
		if (orz->weapon[i].status<0&&orz->weapon[i].life>0)
		{
			if (orz->plr==1)
				x=228-(i*9);
			else
				x= 2+(i*9);
			if (orz->weapon[i].status==-2)
				s=orz->SpriteStart+104;
			sprites[orz->weapon[i].sprite].attribute0 = COLOR_256 | SQUARE | MODE_TRANSPARENT | 2;
			sprites[orz->weapon[i].sprite].attribute1 = SIZE_8 | ROTDATA(orz->weapon[i].sprite) |x;
   			sprites[orz->weapon[i].sprite].attribute2 = s | PRIORITY(0);
		}
	
	}
}

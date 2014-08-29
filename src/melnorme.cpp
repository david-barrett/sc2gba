#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sincosrad.h"

#include "melnorme_out.h"
#include "melnorme_confuse.h"
#include "melnorme_confusehit.h"
#include "melnorme_firegreen1.h"
#include "melnorme_firegreen2.h"
#include "melnorme_fireblue1.h"
#include "melnorme_fireblue2.h"
#include "melnorme_firepurple1.h"
#include "melnorme_firered1.h"
#include "melnorme_firepurple2.h"
#include "melnorme_firered2.h"

#include "melnorme_sfx.h"

#include "melnormepilot.h"
#include "melnormepilotl.h"
#include "melnormepilotr.h"
#include "melnormepilott.h"
#include "melnormepilotf.h"
#include "melnormepilots.h"

extern s32 screenx,screeny;
extern pOAMEntry sprites;
extern double scale;
extern int turn;
extern unsigned long state;

int FireMelnorme(pPlayer pl);
int SpecialMelnorme(pPlayer pl);
int aiMelnorme(pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
void SetMelnormePilot(pPlayer p);
void RestoreGFXMelnorme(pPlayer p);
void PostMelnorme(pPlayer p);

void MoveConfuse(pWeapon w);

extern s32 zoom;
extern s8 v3do;

#define ps 1024+512+128+64+128+512+128

void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale);

#define MAX_CREW 20
#define MAX_ENERGY 42
#define ENERGY_REGENERATION 1
#define WEAPON_ENERGY_COST 5
#define SPECIAL_ENERGY_COST 20
#define ENERGY_WAIT 4
#define MAX_THRUST 36
#define THRUST_INCREMENT 6
#define TURN_WAIT 4
#define THRUST_WAIT 4
#define WEAPON_WAIT 1
#define SPECIAL_WAIT 20

#define SHIP_MASS 7
#define PUMPUP_SPEED 45 //DISPLAY_TO_WORLD (45)
#define PUMPUP_LIFE 10
#define PUMPUP_DAMAGE 2

#define CMISSILE_SPEED 30 //DISPLAY_TO_WORLD (30)
#define CMISSILE_LIFE 20
#define CMISSILE_TIME 100//400

void LoadMelnorme(s16 SpriteStart)
{
	s16 OAMStart=16*SpriteStart;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = melnormeData[loop-OAMStart];
       		OAMData[loop+512] = melnorme_outData[loop-OAMStart];
			OAMData[loop+1024] = melnorme_confusehitData[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+1024+512] = melnorme_confuseData[loop-OAMStart];
	}

	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
   	{
		OAMData[loop+1024+512+128] = melnorme_firegreen1Data[loop-OAMStart];
		OAMData[loop+1024+512+128+32] = melnorme_firegreen2Data[loop-OAMStart];
	}

	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+1024+512+128+64] = melnorme_fireblue1Data[loop-OAMStart];
		OAMData[loop+1024+512+128+64+128] = melnorme_fireblue2Data[loop-OAMStart];
		OAMData[loop+1024+512+128+64+256] = melnorme_firepurple1Data[loop-OAMStart];
		OAMData[loop+1024+512+128+64+128+256] = melnorme_firepurple2Data[loop-OAMStart];
		OAMData[loop+1024+512+128+64+512] = melnorme_firered1Data[loop-OAMStart];
		OAMData[loop+1024+512+128+64+128+512] = melnorme_firered2Data[loop-OAMStart];
	}

	

   	//pilot

   	for (loop=OAMStart ;loop<OAMStart+1024;loop++)
	{
			OAMData[loop+ps] = melnormepilotData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+256;loop++)
	{
		OAMData[loop+ps+1024] = melnormepilotlData[loop-OAMStart];
		OAMData[loop+ps+1024+256] = melnormepilotrData[loop-OAMStart];
		OAMData[loop+ps+1024+512] = melnormepilottData[loop-OAMStart];
	}

	for (loop=OAMStart ;loop<OAMStart+1024;loop++)		
		OAMData[loop+ps+1024+512+256] = melnormepilotfData[loop-OAMStart];
	for (loop=OAMStart ;loop<OAMStart+256;loop++)
		OAMData[loop+ps+(1024*2)+256+512] = melnormepilotsData[loop-OAMStart];

	

}

int FireMelnorme(pPlayer pl)
{
	if (pl->charging>0)
	{
		pl->charging++;
		pl->weapon_turn=pl->weapon_wait;
		return 0;
	}
/*
	pl->currentweapon++;
	if (pl->currentweapon==4)
		pl->currentweapon=0;
*/

	pl->currentweapon=2; //tmp
	s16 b= pl->currentweapon;
	

	pl->weapon[b].type=SIMPLE;
	pl->weapon[b].life=PUMPUP_LIFE;
	pl->weapon[b].damage=-1*PUMPUP_DAMAGE; 
	pl->weapon[b].target=pl->opp;
	pl->weapon[b].parent=pl;
	pl->weapon[b].damageparent=0;

	pl->weapon[b].size=8;
	pl->weapon[b].angle = 0;
	pl->weapon[b].actualangle = 0;

	pl->weapon[b].xspeed=0;
	pl->weapon[b].yspeed=0;

	pl->weapon[b].turn_wait=0;


	pl->weapon[b].xpos = pl->xpos+((s32)(pl->offset * SIN[pl->angle])>>8);
	pl->weapon[b].ypos = pl->ypos-((s32)(pl->offset * COS[pl->angle])>>8);

	drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
		pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

	sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
	sprites[pl->weapon[b].sprite].attribute1 = SIZE_8 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
	sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+104 | PRIORITY(1);

	play_sfx(&melnorme_charge,pl->plr-1);

	pl->charging=1;



	return 1;
}

void SetMelnorme(pPlayer pl)
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

	pl->firefunc=&FireMelnorme;
	pl->specfunc=&SpecialMelnorme;
	pl->aifunc=&aiMelnorme;
	pl->loadfunc=&LoadMelnorme;
	pl->loadpilots=&SetMelnormePilot;
	pl->postfunc=&PostMelnorme;
	pl->restorefunc=&RestoreGFXMelnorme;

	pl->ditty=&melnorme_ditty;

	pl->ship_flags = FIRES_FORE ;
	pl->mass=SHIP_MASS;

	pl->pilot_sprite=(ps)/16;
	pl->pilots[0].x=25;
	pl->pilots[0].y=15;
	pl->pilots[1].x=9;
	pl->pilots[1].y=16;
	pl->pilots[2].x=37;
	pl->pilots[2].y=0;
	pl->pilots[3].x=0;
	pl->pilots[3].y=0;
	pl->pilots[4].x=12;
	pl->pilots[4].y=0;

	pl->charging=0;
	pl->currentweapon=0;

}

int SpecialMelnorme(pPlayer pl)
{

	s16 b=1;
	{
		pl->weapon[b].type=CONFUSE;
		pl->weapon[b].life=CMISSILE_LIFE;
		pl->weapon[b].movefunc=&MoveConfuse;;
		pl->weapon[b].damage=0;
		pl->weapon[b].target=pl->opp;
		pl->weapon[b].parent=pl;
		pl->weapon[b].damageparent=0;

		pl->weapon[b].size=16;
		pl->weapon[b].angle = 0;

		pl->weapon[b].xspeed=((s32)(CMISSILE_SPEED * SIN[pl->angle])>>8);
		pl->weapon[b].yspeed=((s32)(CMISSILE_SPEED * COS[pl->angle])>>8);


		pl->weapon[b].xpos = pl->xpos-((s32)(pl->offset * SIN[pl->angle])>>8);
		pl->weapon[b].ypos = pl->ypos+((s32)(pl->offset * COS[pl->angle])>>8);

		drawOnScreen(&pl->weapon[b].xscreen,&pl->weapon[b].yscreen,
			pl->weapon[b].xpos,pl->weapon[b].ypos,screenx,screeny,pl->weapon[b].size);

		sprites[pl->weapon[b].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | pl->weapon[b].yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[pl->weapon[b].sprite].attribute1 = SIZE_16 | ROTDATA(pl->weapon[b].sprite) | pl->weapon[b].xscreen;
		sprites[pl->weapon[b].sprite].attribute2 = pl->SpriteStart+96 | PRIORITY(1);
			


		}
	play_sfx(&melnorme_fire,pl->plr-1);


	return 1;


}

void MoveConfuse(pWeapon w)
{
	pPlayer target=(pPlayer)w->target;
	pPlayer p = (pPlayer)w->parent;
	if (DetectWeaponToShip(target,w)==1)
	{
		target->scrambled=CMISSILE_TIME;
		sprites[p->weapon[0].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
		sprites[p->weapon[0].sprite].attribute1 = SIZE_32 | ROTDATA(p->weapon[0].sprite) | 240;
   		sprites[p->weapon[0].sprite].attribute2 = p->SpriteStart+64 | PRIORITY(0);
		w->life=0;

	}
	else
	{
		w->xpos+=w->xspeed;
		w->ypos-=w->yspeed;	
	}
}

int aiMelnorme(pPlayer ai, pObject ObjectsOfConcern, COUNT ConcernCounter)
{
	pObject lpEvalDesc;
	pPlayer opp=(pPlayer)ai->opp;

	s8 old_count = ai->weapon_turn;

	if (ai->weapon_turn == WEAPON_WAIT)
		ai->weapon_turn = 0;

	lpEvalDesc = &ObjectsOfConcern[ENEMY_SHIP_INDEX];
	if (lpEvalDesc->parent)
	{
		if (ai->batt < SPECIAL_ENERGY_COST
				+ WEAPON_ENERGY_COST
				&& (ai->charging=0))
			lpEvalDesc->MoveState = ENTICE;
		else
		{
						if (!(ai->ship_flags
					& IMMEDIATE_WEAPON))
				lpEvalDesc->MoveState = PURSUE;
		}
	}
	ship_intelligence (ai, ObjectsOfConcern, ConcernCounter);

	if (ai->weapon_turn == 0
			&& (old_count != 0
			|| ((ai->special_turn
			|| ai->batt >= SPECIAL_ENERGY_COST
			+ WEAPON_ENERGY_COST)
			&& !(ai->ship_input_state & WEAPON))))
		ai->ship_input_state ^= WEAPON;

	ai->ship_input_state &= ~SPECIAL;
	if (ai->special_turn == 0
			&& ai->batt >= SPECIAL_ENERGY_COST)
	{
		s8 old_input_state;

		old_input_state = ai->ship_input_state;

		//StarShipPtr->RaceDescPtr->init_weapon_func = initialize_confusion;
		//somehow need to check if fired confu

		++ai->turn_turn;
		++ai->thrust_turn;
		ship_intelligence (ai, ObjectsOfConcern, ENEMY_SHIP_INDEX + 1);
		--ai->thrust_wait;
		--ai->turn_wait;

		if (ai->ship_input_state & WEAPON)
		{
			ai->ship_input_state &= ~WEAPON;
			ai->ship_input_state |= SPECIAL;
		}

		ai->ship_input_state = (unsigned char)(old_input_state
				| (ai->ship_input_state & SPECIAL));
	}

	ai->weapon_turn = old_count;

	//StarShipPtr->RaceDescPtr->init_weapon_func = initialize_pump_up;

	

}

void SetMelnormePilot(pPlayer pl)
{
	//setup pilot
	int off=(pl->plr==1)?0:6;

	sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[43+off].attribute1 = SIZE_32 | 240;
	sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

	sprites[44+off].attribute0 = COLOR_256 | TALL  | 160;
	sprites[44+off].attribute1 = SIZE_32 | 240;
	sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+80 | PRIORITY(2);

	sprites[45+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[45+off].attribute1 = SIZE_32 | 240;
	sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+96 | PRIORITY(2);

	sprites[46+off].attribute0 = COLOR_256 |TALL  | 160;
	sprites[46+off].attribute1 = SIZE_64 | 240;
	sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+112 | PRIORITY(2);

	sprites[47+off].attribute0 = COLOR_256 | WIDE  | 160;
	sprites[47+off].attribute1 = SIZE_32 | 240;
	sprites[47+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+176 | PRIORITY(2);
}

void RestoreGFXMelnorme(pPlayer p)
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

void PostMelnorme(pPlayer p)
{
	s16 a=p->angle-180;
	if (a<0)
		a+=360;
	RotateSprite(p->plr==1?0:13, a, zoom, zoom);

	pPlayer opp=(pPlayer)p->opp;

	if (opp->scrambled)
	{
		if (v3do)
		{
			opp->actualangle++;
			if(opp->actualangle==16)
				opp->actualangle=0;
			opp->angle=(opp->actualangle*45)>>1;
		}
		opp->scrambled--;

		if (opp->scrambled==0)
			p->weapon[0].life=0;

		MoveSprite(&sprites[p->weapon[0].sprite],opp->xscreen,opp->yscreen);
		RotateSprite(p->weapon[0].sprite,opp->angle,zoom,zoom);

	}

	
	if (p->charging>0)
	{
		if (!(p->ship_input_state & WEAPON))
		{
			//fire
			p->charging=0;
			p->weapon[p->currentweapon].xspeed=((s32)(PUMPUP_SPEED * SIN[p->angle])>>8);
			p->weapon[p->currentweapon].yspeed=((s32)(PUMPUP_SPEED * COS[p->angle])>>8);
			play_sfx(&melnorme_fire,p->plr-1);
		}
		else
		{	
			p->weapon[p->currentweapon].life=PUMPUP_LIFE;
			
			s16 sprite=0;
			s8 size=1;
			s16 o=6;

			if (p->charging<45)
			{
				o=2;
				size=0;
				p->weapon[p->currentweapon].turn_wait=!p->weapon[p->currentweapon].turn_wait;
				if (p->weapon[p->currentweapon].turn_wait)
					sprite=p->SpriteStart+106;
				else
					sprite=p->SpriteStart+104;



				
			}
			else if (p->charging<90)
			{
				p->weapon[p->currentweapon].size=16;
				p->weapon[p->currentweapon].damage=-2*PUMPUP_DAMAGE; //guess				
				p->weapon[p->currentweapon].turn_wait=!p->weapon[p->currentweapon].turn_wait;
				if (p->weapon[p->currentweapon].turn_wait)
					sprite=p->SpriteStart+116;
				else
					sprite=p->SpriteStart+108;
			}
			else if (p->charging<135)
			{
				p->weapon[p->currentweapon].damage=-3*PUMPUP_DAMAGE; //guess			
				p->weapon[p->currentweapon].turn_wait=!p->weapon[p->currentweapon].turn_wait;
				if (p->weapon[p->currentweapon].turn_wait)
					sprite=p->SpriteStart+132;
				else
					sprite=p->SpriteStart+124;
			}
			else
			{
				p->weapon[p->currentweapon].damage=-4*PUMPUP_DAMAGE; //guess			
				p->weapon[p->currentweapon].turn_wait=!p->weapon[p->currentweapon].turn_wait;
				if (p->weapon[p->currentweapon].turn_wait)
					sprite=p->SpriteStart+148;
				else
					sprite=p->SpriteStart+140;

			}

			p->weapon[p->currentweapon].xpos = p->xpos+((s32)((p->offset+o) * SIN[p->angle])>>8);
			p->weapon[p->currentweapon].ypos = p->ypos-((s32)((p->offset+o) * COS[p->angle])>>8);



			drawOnScreen(&p->weapon[p->currentweapon].xscreen,&p->weapon[p->currentweapon].yscreen,
					p->weapon[p->currentweapon].xpos,p->weapon[p->currentweapon].ypos,screenx,screeny,p->weapon[p->currentweapon].size);

			sprites[p->weapon[p->currentweapon].sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | p->weapon[p->currentweapon].yscreen;	//setup sprite info, 256 colour, shape and y-coord
			if (size)
				sprites[p->weapon[p->currentweapon].sprite].attribute1 = SIZE_16 | ROTDATA(p->weapon[p->currentweapon].sprite) | p->weapon[p->currentweapon].xscreen;
			else
				sprites[p->weapon[p->currentweapon].sprite].attribute1 = SIZE_8 | ROTDATA(p->weapon[p->currentweapon].sprite) | p->weapon[p->currentweapon].xscreen;
			sprites[p->weapon[p->currentweapon].sprite].attribute2 = sprite | PRIORITY(0);	
					
		}//still holding
	}//chargine
	
}


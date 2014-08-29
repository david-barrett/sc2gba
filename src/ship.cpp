#include "keypad.h"     //button registers
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"
#include "bg.h"

#include<math.h>
#include<stdlib.h>

extern u16* OAM;
extern pOAMEntry sprites;
extern pRotData rotData;
extern int* KEYS;

extern s8 pilot;


extern pTrail trails;
extern pAsteroid asteroids;

extern int turn;

extern s32 zoom,screenx,screeny;

extern double scale;

extern s32 planetx,planety;

const s8 EXPX[12] = {5,-10,9,-2,7,0,-6,4,-12,13,-14,8};
const s8 EXPY[12] = {-7,0,-15,12,-5,4,-9,10,-4,9,-2,4};

void GenerateStart(pPlayer p,s32 range)
{   //y=640
	//x=960
	int good=0;
	s32 xpos,ypos;
	s16 dist;

	pPlayer opp=(pPlayer)p->opp;
	p->angle=ran(0,23)*15;

	p->xspeed = ((20) * (s32)SIN[p->angle])>>8;
	p->yspeed = ((20) * (s32)COS[p->angle])>>8;
	do
	{
		good=0;
		/*
		s32 rand=ran(0,1820)-910;//960-50 *2
		p->xpos=opp->xpos+rand+(rand<0?-50:50);
		rand=ran(0,1180)-590;//960-50 *2
		p->ypos=opp->ypos+rand+(rand<0?-50:50);
		*/

		s32 rand=ran(0,range)-(range/2);//960-50 *2

	//	print("\nrand val:=");
	//	print(rand);
		p->xpos=opp->xpos+rand+(rand<0?-50:50);

		p->ypos=opp->ypos+rand+(rand<0?-50:50);
	//	print("\nxpos:=");
	//	print(p->xpos);
	//	print("\nypos:=");
	//	print(p->ypos);

		//s16 dist=distanceBetweenPoints(p->xpos,p->ypos,planetx,planety);
					//if (dist<(p->offset+64)/2)
					/*
		if (dist<150)
		{
						//bad try again
						good=1;
						break;
	}
		//s16 angle = FindAngle(p->xpos,p->ypos,planetx,planety);

	//	p->angle=angle+(angle>180)?-180:180;
	p->angle=FindAngle(planetx,planety,p->xpos,p->ypos);


*/

	    xpos = p->xpos;
	    ypos = p->ypos;
		//check we aint going to hit a planet
		dist=distanceBetweenPoints(xpos,ypos,planetx,planety);
		//print("\ndist:=");
		//print(dist);
		if (dist<200)
		{
			p->xpos=opp->xpos-rand+(rand<0?50:-50);

			p->ypos=opp->ypos-rand+(rand<0?50:-50);

				//good=1;
		}
	}while(good);
	good=0;
		for (int i=0;i<6;i++)
		{
			dist=distanceBetweenPoints(xpos,ypos,planetx,planety);
			//if (dist<(p->offset+64)/2)
			if (dist<200)
			{
				//bad try again
				good=1;
				break;
			}
			xpos+=p->xspeed;
			ypos-=p->yspeed;
		}
	if (good==1)
	{
		p->angle=ModifyAngle(p->angle,180);
		p->xspeed*=-1;
		p->yspeed*=-1;
	}



	//while(good);
}



void DetonateShip(pPlayer pl)
{

	for (int i=0;i<12;i++)
	{
		if (pl->weapon[i].life==-1)
		{
			pl->weapon[i].xpos=pl->xpos+(s32)(EXPX[i]*scale);
			pl->weapon[i].ypos=pl->ypos+(s32)(EXPY[i]*scale);
			pl->weapon[i].size=8;
			CreateExplosion(&pl->weapon[i],5);
		}
	}
}

void TurnRight(pPlayer pl, int i)
{
	if (pl->turn_turn==0)
	{
	pl->turn_turn==pl->turn_wait;
	pl->angle+=i;
	if(pl->angle>=360)
		pl->angle-=360;;
	}
}

void TurnLeft(pPlayer pl,int i)
{
	if (pl->turn_turn==0)
	{
	pl->turn_turn==pl->turn_wait;
	pl->angle-=i;
	if(pl->angle<0)
		pl->angle+=360;;
	}
}

void GetInput(pPlayer pl)
{
	if(!(*KEYS & KEY_UP))                   //if the UP key is pressed
	{
		pl->ship_input_state |= THRUST;
	}
	if(!(*KEYS & KEY_DOWN))                 //if the DOWN key is pressed
	{
	}
	if(!(*KEYS & KEY_LEFT))                 //if the LEFT key is pressed
	{
        pl->ship_input_state |= LEFT;
	}
	if(!(*KEYS & KEY_RIGHT))                //if the RIGHT key is pressed
	{
		pl->ship_input_state |= RIGHT;
	}
	if(!(*KEYS & KEY_A))                	//if the A key is pressed
	{
		pl->ship_input_state |= WEAPON;
    }
	if(!(*KEYS & KEY_B))                	//if the B key is pressed
	{
		pl->ship_input_state |= SPECIAL;
    }
	if(!(*KEYS & KEY_L))                	//if the L key is pressed
	{
		pl->ship_input_state |= THRUST;

	}
	if(!(*KEYS & KEY_R))                	//if the R key is pressed
	{

	}
	if(!(*KEYS & KEY_SELECT))               //if the SELECT key is pressed
	{

	}
	if(!(*KEYS & KEY_START))                //if the START key is pressed
	{
		MoveSprite(&sprites[55],64,64);
		MoveSprite(&sprites[56],96,64);
		MoveSprite(&sprites[57],128,64);
		MoveSprite(&sprites[58],160,64);
		WaitForVsync();
		CopyOAM();
		for(int i=0;i<200;i++)
			WaitForVsync();
		do
		{
		}while((*KEYS & KEY_START));
		do
		{
		}while(!(*KEYS & KEY_START));
		MoveSprite(&sprites[55],240,160);
		MoveSprite(&sprites[56],240,160);
		MoveSprite(&sprites[57],240,160);
		MoveSprite(&sprites[58],240,160);
		WaitForVsync();
		CopyOAM();

	}
}

void Thrust(pPlayer plr)
{
	if (plr->thrust_turn==0)
	{
	plr->thrust_turn==plr->thrust_wait;
	s32 x,y;
	if (plr->accel_inc == plr->maxspeed)
	{
		//arilou
		plr->thrustspeed==plr->maxspeed;

		plr->xspeed = (s32)(double(plr->maxspeed) * (s32)SIN[plr->angle])>>8;
		plr->yspeed = (s32)(double(plr->maxspeed) * (s32)COS[plr->angle])>>8;

	}
	else
	{

		if (plr->angle==plr->thrustangle)
		{

			s32 actual_speed=distanceBetweenPoints(0,0,plr->xspeed,plr->yspeed);
			plr->thrustspeed+=plr->accel_inc;
			if (actual_speed+plr->accel_inc>plr->maxspeed)
			{
			//	print("max speed");
				plr->thrustspeed=plr->maxspeed;
			}

		}
		else
		{
			plr->thrustspeed=2*plr->accel_inc;
			if (plr->thrustspeed>plr->maxspeed)
			{
			//	print("max speed");
				plr->thrustspeed=plr->maxspeed;
			}

			plr->thrustangle=plr->angle;
		}
    	x = (s32)(double(plr->thrustspeed) * (s32)SIN[plr->angle])>>8;
		y = (s32)(double(plr->thrustspeed) * (s32)COS[plr->angle])>>8;

		plr->xspeed = (plr->xspeed + x)>>3;
		plr->yspeed = (plr->yspeed + y)>>3;
	}

	if (!plr->cloak)
		CreateTrail(plr);
	}

}


void Regen(pPlayer pl)
{
//perhaps should call regen less?
	if (pl->batt_turn>0)
		pl->batt_turn--;
	if (pl->turn_turn>0)
		pl->turn_turn--;
	if (pl->thrust_turn>0)
		pl->thrust_turn--;
	if (pl->weapon_turn>0)
		pl->weapon_turn--;
	if (pl->special_turn>0)
		pl->special_turn--;
	if (pl->aiturn>0)
		pl->aiturn--;

	if (pl->batt<pl->maxbatt&&pl->batt_turn==0)
	{
		pl->batt_turn=pl->batt_wait*2;
		ModifyBatt(pl,pl->batt_regen);
	}

	//if (pl->ship==TERMINATOR&&pl->shield>0)

}

inline int GetNextTrail(int off)
{
	for (int i=0+off;i<5+off;i++)
	{
		if (trails[i].life==0)
			return i;
	}
	return -1;
}

void CreateTrail(pPlayer p)
{
	int t = GetNextTrail((p->plr-1)*5); //=p1=0 or p2=5

	s32 x = ((p->offset) * (s32)SIN[p->angle])>>8;
	s32 y = ((p->offset) * (s32)COS[p->angle])>>8;
	if (t!=-1)
	{
		trails[t].life=5;
		trails[t].xpos=p->xpos-(x);
		trails[t].ypos=p->ypos+(y);
	}
}

void DrawTrails()
{
	for (int i=0;i<10;i++)
	{
		if (trails[i].life>0)
		{
			drawOnScreen(&trails[i].xscreen,&trails[i].yscreen,trails[i].xpos,trails[i].ypos,screenx,screeny,4);
			sprites[32+i].attribute0 = COLOR_256 | SQUARE | MODE_TRANSPARENT | trails[i].yscreen;	//setup sprite info, 256 colour, shape and y-coord
			sprites[32+i].attribute1 = SIZE_8 | trails[i].xscreen;
    		sprites[32+i].attribute2 = TrailSprite | PRIORITY(1);
    		//sprites[32+i].attribute2 = FireSprite1+2 | PRIORITY(0); //if i need to see the trail big
    		trails[i].life--;
		}
		else if (trails[i].life==0)
		{
			MoveOffscreen(&sprites[32+i]);
		}

	}

}

int CreateActualOutline(s32 xpos,s32 ypos,s16 angle,pWeapon w,s32 SpriteStart)
{
	w->type=TRAIL;
	w->life=5;
	w->damage=0;
	w->target=NULL;
	w->parent=NULL;
	w->damageparent=0;

	w->size=32;
	w->angle = angle;

	w->xspeed = 0;
	w->yspeed = 0;

	w->xpos = xpos;
	w->ypos = ypos;

	drawOnScreen(&w->xscreen,&w->yscreen,
		w->xpos,w->ypos,screenx,screeny,w->size);

	sprites[w->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | w->yscreen;	//setup sprite info, 256 colour, shape and y-coord
    sprites[w->sprite].attribute1 = SIZE_32 | ROTDATA(w->sprite) | w->xscreen;
    sprites[w->sprite].attribute2 = SpriteStart+32 | PRIORITY(2);


    return 1;
}

int CreateOutline(pPlayer pl)
{
	s16 b = nextWeapon(pl);

	if (b>=0)
	{
		return CreateActualOutline(pl->xpos,pl->ypos,pl->angle,&pl->weapon[b],pl->SpriteStart);
	}
	return 0;
}

int CreateReinc(pPlayer pl)
{
	short d=20*(pl->reinc+1);
	short r=pl->reinc*4;
	CreateActualOutline(pl->xpos-d,pl->ypos-d,pl->angle,&pl->weapon[r],pl->SpriteStart);
	CreateActualOutline(pl->xpos-d,pl->ypos+d,pl->angle,&pl->weapon[1+r],pl->SpriteStart);
	CreateActualOutline(pl->xpos+d,pl->ypos-d,pl->angle,&pl->weapon[2+r],pl->SpriteStart);
	CreateActualOutline(pl->xpos+d,pl->ypos+d,pl->angle,&pl->weapon[3+r],pl->SpriteStart);
	return 1;
}

void Bounce(pPlayer pl)
{
	//pl->xspeed*=-1;
	//pl->yspeed*=-1;//try 2 instead of 1

	pl->xspeed=(-1*pl->xspeed)>>1;
	pl->yspeed=(-1*pl->yspeed)>>1;

	pl->xpos+=pl->xspeed;
	pl->xpos+=pl->xspeed;
}

void ProcessPlayer(pPlayer pl)
{
	pl->ship_input_state=0;
	int off=0;
	int x=509;
	if (pl->plr==2)
	{
		off=6;
		x=179;
	}
	if (pl->crew>0)
	{
		if (pl->warp>1)
		{
			CreateOutline(pl);
			pl->warp--;
		}
		else if(pl->warp==1)
		{
			pl->xspeed=0;
			pl->yspeed=0;
			pl->warp=0;
		}
		else if(pl->reinc>0)
		{
			pl->reinc--;
			CreateReinc(pl);
		}
		else
		{
			if (pl->ai==PLAYER1||pl->ai==PLAYER2)
				GetInput(pl);
			else if(pl->ai!=DISABLED&&pPlayer(pl->opp)->crew>0)
				tactical_intelligence (pl);

		}

		pl->xpos+=pl->xspeed;
		pl->ypos-=pl->yspeed;
		pl->speed=distanceBetweenPoints(0,0,pl->xspeed,pl->yspeed);


		if (pl->ship_input_state & LEFT)
			TurnLeft(pl);
		else if (pl->ship_input_state & RIGHT)
			TurnRight(pl);
		if (pl->ship_input_state & THRUST)
			Thrust(pl);;
		if (pl->ship_input_state & WEAPON)
			Fire(pl);
		if (pl->ship_input_state & SPECIAL)
			Special(pl);



		//pilot
			if (pilot)
			{

				if (1)
				{
					if (pl->ship_input_state & LEFT)
						MoveSprite(&sprites[43+off],x+pl->pilots[0].x,pl->pilots[0].y);
					else
						MoveSprite(&sprites[43+off], 240, 160);

					if (pl->ship_input_state & RIGHT)
						MoveSprite(&sprites[44+off],x+pl->pilots[1].x,pl->pilots[1].y);
					else
						MoveSprite(&sprites[44+off], 240, 160);

					if (pl->ship_input_state & THRUST)
						MoveSprite(&sprites[45+off],x+pl->pilots[2].x,pl->pilots[2].y);
					else
						MoveSprite(&sprites[45+off], 240, 160);

					if (pl->ship_input_state & WEAPON)
						MoveSprite(&sprites[46+off],x+pl->pilots[3].x,pl->pilots[3].y);
					else
						MoveSprite(&sprites[46+off], 240, 160);

					if (pl->ship_input_state & SPECIAL)
						MoveSprite(&sprites[47+off],x+pl->pilots[4].x,pl->pilots[4].y);
					else
						MoveSprite(&sprites[47+off], 240, 160);
				}


			}

	}
	else
	{
		if (pl->ship==FURY&&pl->EndGame==20)
		{
			if (DeathFury(pl)==1)
				return;
		}
		pPlayer opp = (pPlayer)pl->opp;
		if (opp->crew>0&&pl->EndGame==20)
		{
			play_sfx(opp->ditty,pl->plr-1);
			//remove pilot

			MoveSprite(&sprites[42+off], 240, 160);
			MoveSprite(&sprites[43+off], 240, 160);
			MoveSprite(&sprites[44+off], 240, 160);
			MoveSprite(&sprites[45+off], 240, 160);
			MoveSprite(&sprites[46+off], 240, 160);
			MoveSprite(&sprites[47+off], 240, 160);
		}
		pl->EndGame--;
		ModifyBatt(pl,-1*pl->batt);
		DetonateShip(pl);
	}

}

int Special(pPlayer pl)
{

	int ret=0;
	if (pl->special_turn==0&&pl->batt>=pl->specbatt)
	{
		ret=pl->specfunc(pl);

		if (ret>0)
		{
			ModifyBatt(pl,-1*pl->specbatt);
			pl->special_turn=pl->special_wait;
		}
	}
	return ret;
}

void Fire(pPlayer pl)
{
	int ret=0;
	if (pl->weapon_turn==0&&pl->batt>=pl->firebatt)
	{
		ret=pl->firefunc(pl);

		if (ret>0)
		{
			ModifyBatt(pl,-1*pl->firebatt);
			pl->weapon_turn=pl->weapon_wait;
		}
	}
}

int nextWeapon(pPlayer pl,int f,int l)
{
	//defaults f=1 & l=11
	for (int i=f;i<l+1;i++)
	{
		if (pl->weapon[i].life==-1)
			return i;
	}
	return -1;
}

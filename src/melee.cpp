//////////////////////////////////////////////////////////////////////////
// File: 	star control gba.cpp                                            	//
// Description: a sc2 clone			//
// Author:	david barrett 	  					//
// Date: 	4th Aug 2004                                      	//
//////////////////////////////////////////////////////////////////////////

#include "gba.h"	//GBA register definitions
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


pTrail trails;
pAsteroid asteroids;

int turn;

s16 planet_screenx,planet_screeny;



//extern FIXED SIN2[360];	    //Look-Up Tabless for sign and cosign
//extern FIXED COS2[360];

const s8 EXPX[12] = {5,-10,9,-2,7,0,-6,4,-12,13,-14,8};
const s8 EXPY[12] = {-7,0,-15,12,-5,4,-9,10,-4,9,-2,4};



double scale;

s32 zoom,screenx,screeny;

//centre of screen
//note ships draw from TOP_LEFT
s32 const centrex=120;
s32 const centrey=80;

#define ASIZE  512 //128 // 512 ?


char s[100];

void
print(char *s)
{
/*
    asm volatile ("mov r0, %0;" "swi 0xff0000;":        // warning! will crash on hardware!
                  :"r" (s):"r0");
*/
}


void
printi(s32 s)
{
  //  s8 n = s%10;
    switch (s)
    {
		case 0:
			print("0");
			break;

		case 1:
					print("1");
			break;
		case 2:
					print("2");
			break;
		case 3:
					print("3");
			break;
	case 4:
				print("4");
			break;

		case 5:
					print("5");
			break;
	case 6:
				print("6");
			break;
	case 7:
				print("7");
			break;
	case 8:
				print("8");
			break;
	case 9:
				print("9");
			break;
}


}

void
print(s32 s)
{

	s32 t=s/10000;
	printi(t);
	s=s-(t*10000);

	t=s/1000;
		printi(t);
	s=s-(t*1000);

	t=s/100;
		printi(t);
	s=s-(t*100);

	t=s/10;
	printi(t);
	s=s-(t*10);

	printi(s%10);

}

void GenerateStart(pPlayer p)
{   //y=640
	//x=960
	int good=0;
	s32 xpos,ypos,xspeed,yspeed;
	s16 dist;

	pPlayer opp=(pPlayer)p->opp;
			p->angle=ran(0,24)*15;
			print("\n Generate start\nrand angle:=");
		print(p->angle);
	xspeed = ((20) * (s32)SIN[p->angle])>>8;
	yspeed = ((20) * (s32)COS[p->angle])>>8;
	do
	{
		good=0;
		/*
		s32 rand=ran(0,1820)-910;//960-50 *2
		p->xpos=opp->xpos+rand+(rand<0?-50:50);
		rand=ran(0,1180)-590;//960-50 *2
		p->ypos=opp->ypos+rand+(rand<0?-50:50);
		*/

		s32 rand=ran(0,1180)-590;//960-50 *2

		print("\nrand val:=");
		print(rand);
		p->xpos=opp->xpos+rand+(rand<0?-50:50);

		p->ypos=opp->ypos+rand+(rand<0?-50:50);
		print("\nxpos:=");
		print(p->xpos);
		print("\nypos:=");
		print(p->ypos);

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
		print("\ndist:=");
		print(dist);
		if (dist<200)
			good=1;
	}while(good);
	print("\nbreak from loop");
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
			xpos+=xspeed;
			ypos+=yspeed;
		}
	if (good==1)
		p->angle=ModifyAngle(p->angle,180);


	//while(good);
}



//Copy our sprite array to OAM
void CopyOAM()
{
	/*
	u16 loop;
	u16* temp;
	temp = (u16*)sprites;
	for(loop = 0; loop < 128*4; loop++)
	{
		OAM[loop] = temp[loop];
	}
	*/
	//try this - eventually replace CopyOAM with this...
	REG_DMA3SAD = (u32)sprites;
	REG_DMA3DAD = (u32)OAM;
	REG_DMA3CNT = 128 * 4 |DMA_16NOW;
}

//Set sprites to off screen
void InitializeSprites()
{
	u16 loop;
	for(loop = 0; loop < 128 ;loop++)
	{
		sprites[loop].attribute0 = 160;  //y to > 159
		sprites[loop].attribute1 = 240; //x to > 239
		//sprites[loop].attribute2 = 0;
	}
}

//wait for the screen to stop drawing
void WaitForVsync()
{
	while((volatile u16)REG_VCOUNT != 160){}
}

void swap(s32 *one, s32 *two)
{
	s32 three;
	three=*one;
	*one=*two;
	*two=three;
}


void DrawExplosion(pWeapon w)
{
	s16 life=w->life;

	s16 off=0;

	if (life<3)
	{
		if (life==2)
			off=2;
		w->size=8;
		sprites[w->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | w->yscreen;	//setup sprite info, 256 colour, shape and y-coord
	    sprites[w->sprite].attribute1 = SIZE_8 | ROTDATA(w->sprite) | w->xscreen;
    	sprites[w->sprite].attribute2 = FireSprite1+off | PRIORITY(0);
	}
	else if (life<=5)
	{
		off=4;
		if (life==4)
			off=12;
		else if (life==5)
			off=20;
		w->size=16;
		sprites[w->sprite].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | w->yscreen;	//setup sprite info, 256 colour, shape and y-coord
		sprites[w->sprite].attribute1 = SIZE_16 | ROTDATA(w->sprite) | w->xscreen;
	    sprites[w->sprite].attribute2 = FireSprite1+off | PRIORITY(0);
	}

}

void CreateExplosion(pWeapon w,s16 life)
{
	w->life=life;;
	w->xspeed=0;
	w->yspeed=0;
	w->type=EXP;
	w->angle=0;
	DrawExplosion(w);
}

void DetonateShip(pPlayer pl)
{

	for (int i=0;i<12;i++)
	{
		if (pl->weapon[i].life==-1)
		{
			pl->weapon[i].xpos=pl->xpos+(s32)(EXPX[i]*scale);
			pl->weapon[i].ypos=pl->ypos+(s32)(EXPY[i]*scale);
			CreateExplosion(&pl->weapon[i],5);
		}
	}
}

int DetectWeaponToShip(pPlayer p,pWeapon w)
{
	//square (w.xpos-4,w.ypos-4), (w.xpos+4,w.ypos+4)
	//square (p.xpos-16,w.ypos-16), (w.xpos+16,w.ypos+16)

	s16 offset=p->offset/2;

	if (w->xpos>p->xpos-offset&&w->xpos<p->xpos+offset&&w->ypos>p->ypos-offset&&w->ypos<p->ypos+offset)
	{
		return 1;
	}
	return 0;

}

int DetectWeaponToWeapon(pWeapon tar,pWeapon w)
{
	//square (w.xpos-4,w.ypos-4), (w.xpos+4,w.ypos+4)
	//square (p.xpos-16,w.ypos-16), (w.xpos+16,w.ypos+16)

	s16 offset=tar->size/2;

	if (w->xpos>tar->xpos-offset&&w->xpos<tar->xpos+offset&&w->ypos>tar->ypos-offset&&w->ypos<tar->ypos+offset)
	{
		return 1;
	}
	return 0;

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
		for(int i=0;i<100;i++);
		do
		{
		}while(!(*KEYS & KEY_START));

	}
}



//move the sprite
void MoveSprite(OAMEntry* sp, int x, int y)
{
	sp->attribute1 = sp->attribute1 & 0xFE00;  //clear the old x value
	sp->attribute1 = sp->attribute1 | x;

	sp->attribute0 = sp->attribute0 & 0xFF00;  //clear the old y value
	sp->attribute0 = sp->attribute0 | y;
}

void MoveOffscreen(OAMEntry* sp)
{
	MoveSprite(sp,-64,-64);
}

s16 FindAngle(s32 x,s32 y, s32 ox, s32 oy)
{
	s16 angle,final;

	if (x==ox)
	{
		if (y<oy)
			return 180;
		else
			return 0;
	}
	if (y==oy)
	{
		if (x>ox)
			return 270;
		else
			return 90;
	}

	double r = double(y-oy)/double(x-ox);
	//double r = double(oy-y)/double(ox-x);

	angle = (s16) (atan(r) * 180 / PI); //maybe find a way to calc this?
	if (angle<0) //make positive
		angle=(angle*-1);

	//normalise to nearest 15.
	//big assumption:tan only returns 0-90?
	//0 =0-7
	//15 = 8-22
	//30 = 23-37
	//45 = 38-52
	//60 = 53-67
	//75 = 68-82
	//90 = 83-90

	if (angle<=7)
		angle=0;
	else if (angle>=8&&angle<=22)
		angle=15;
	else if (angle>=23&&angle<=37)
		angle=15;
	else if (angle>=38&&angle<=52)
		angle=45;
	else if (angle>=53&&angle<=67)
		angle=15;
	else if (angle>=68&&angle<=82)
		angle=75;
	else if (angle<=90)
		angle=90;



	if (ox>x&&oy>y)
		final=90+angle;//right?
	else if (ox>x&&oy<=y)
		final=90-angle;//right
	else if (ox<=x&&oy>y)
		final=270-angle;
	else if (ox<=x&&oy<=y)
		final=270+angle;//right





	return (final);
}

int InRange(s32 xpos,s32 ypos,s32 txpos,s32 typos,s16 range)
{

	return (distanceBetweenPoints(xpos,ypos,txpos,typos)<range);

	/*
	if (xpos>txpos-range&&xpos<txpos+range&&ypos>typos-range&&ypos<typos+range)
	{
		return 1;
	}
	return 0;
	*/
}



void MoveBullets(pPlayer pl)
{
	pPlayer p;
	pPlayer o;

	for (int i=0;i<12;i++)
	{
		if (pl->weapon[i].life==0)//destroy it
		{

			print(sprites[pl->weapon[i].sprite].attribute1);
			sprites[pl->weapon[i].sprite].attribute0 = 160;  //y to > 159
			sprites[pl->weapon[i].sprite].attribute1 = 240; //x to > 239
			sprites[pl->weapon[i].sprite].attribute2 = 0;
			pl->weapon[i].life--;



		}
		if (pl->weapon[i].life>0)
		{

			if (pl->weapon[i].type==SIMPLE||pl->weapon[i].type==ILWRATHFIRE)
			{
				if (pl->weapon[i].type==ILWRATHFIRE)
					DrawExplosion(&pl->weapon[i]);
				pPlayer target=(pPlayer)pl->weapon[i].target;
				pl->weapon[i].xpos+=pl->weapon[i].xspeed;
				pl->weapon[i].ypos-=pl->weapon[i].yspeed;


				pl->weapon[i].life--;

				int stop=1;  //stop processing when destroyedto save time
				//opp ship
				if (DetectWeaponToShip(target,&pl->weapon[i]))
				{
					ModifyCrew(target,pl->weapon[i].damage);
					CreateExplosion(&pl->weapon[i],5);
					stop=0;
				}
				//parent ship
				if (pl->weapon[i].damageparent==1&&stop)
					if (DetectWeaponToShip((pPlayer)pl->weapon[i].parent,&pl->weapon[i]))
					{
						ModifyCrew((pPlayer)pl->weapon[i].parent,pl->weapon[i].damage);
						CreateExplosion(&pl->weapon[i],5);
						stop=0;
					}

				//opp weapon
				if (stop)
				{
				for (int j=0;j<12;j++)
				{
					if (target->weapon[j].life>0)
					if (DetectWeaponToWeapon(&target->weapon[j],&pl->weapon[i])==1)
					{
						pl->weapon[i].life=0;
						target->weapon[j].life=0;
						stop=0;
						if (target->weapon[j].type==UR_FIGHTERS)
							target->fighters--;
						break;
					}
				}
				}

				s16 dist;
				//planet
				if (stop)
				{
					dist = distanceBetweenPoints(pl->weapon[i].xpos,pl->weapon[i].ypos,planetx,planety);
					if (dist<(pl->weapon[i].size+64)/2)
					{
							pl->weapon[i].life=0;
							stop=0;
					}
				}

				//asteroids
				if (stop)
				{
					for (int j=0;j<5;j++)
					{
						dist = distanceBetweenPoints(pl->weapon[i].xpos,pl->weapon[i].ypos,
							asteroids[j].xpos,asteroids[j].ypos);

						if (dist<(pl->weapon[i].size+25)/2)
						{
							pl->weapon[i].life=0;
							asteroids[j].life=0;
							stop=0;
							break;
						}
					}
				}
				if (stop==0)
					pl->weapon[i].life=0;

			}//simple
			else if (pl->weapon[i].type==UR_FIGHTERS)
			{
				MoveURFighters(&pl->weapon[i]);
			}
			else if (pl->weapon[i].type==UR_FIGHTERS_FIRE)
			{
				pl->weapon[i].life--;
			}
			else if(pl->weapon[i].type==EXP)
			{
				pl->weapon[i].life--;
				DrawExplosion(&pl->weapon[i]);
			}
			else if(pl->weapon[i].type==TRAIL)
			{
					pl->weapon[i].life--;
			}
			drawOnScreen(&(pl->weapon[i].xscreen),&(pl->weapon[i].yscreen),
				pl->weapon[i].xpos,pl->weapon[i].ypos,screenx,screeny,pl->weapon[i].size);
			MoveSprite(&sprites[pl->weapon[i].sprite], pl->weapon[i].xscreen, pl->weapon[i].yscreen);
			RotateSprite(pl->weapon[i].sprite, pl->weapon[i].angle, zoom,zoom);
		}//end if life>0
	}//end for loop

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
				print("max speed");
				plr->thrustspeed=plr->maxspeed;
			}

		}
		else
		{
			plr->thrustspeed=2*plr->accel_inc;
			if (plr->thrustspeed>plr->maxspeed)
			{
				print("max speed");
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

void NewThrust(pPlayer plr)
{
	print("started thrust");
	if (plr->thrust_turn==0)
	{
	plr->thrust_turn==plr->thrust_wait;
	s32 x,y;
/*
	if (plr->accel_inc == plr->maxspeed)
	{
		//arilou
		x = (double(plr->maxspeed) * SIN[plr->angle]);
		y = (double(plr->maxspeed) * COS[plr->angle]);
		//return (SHIP_AT_MAX_SPEED);
	}
	else if ((FindAngle(0,0,plr->xspeed,plr->yspeed) == plr->angle
				&& (plr->speed>=plr->maxspeed))
	{
		//ship flying at max speed
	}
	else
	{
		register s32 delta_x, delta_y;
		s32 cur_delta_x, cur_delta_y;
		s32 desired_speed, max_speed;

			//thrust_increment = WORLD_TO_VELOCITY (thrust_increment);
			//GetCurrentVelocityComponents (VelocityPtr, &cur_delta_x, &cur_delta_y);
			//delta_x = cur_delta_x
			//		+ COSINE (CurrentAngle, thrust_increment);
			//delta_y = cur_delta_y
					+ SINE (CurrentAngle, thrust_increment);
			//desired_speed = (DWORD)((long)delta_x * delta_x)
					+ (DWORD)((long)delta_y * delta_y);
			//max_speed = (DWORD)WORLD_TO_VELOCITY (max_thrust)
			//		* WORLD_TO_VELOCITY (max_thrust);
			if (desired_speed <= max_speed)
				SetVelocityComponents (VelocityPtr, delta_x, delta_y);
			else
			{
	#define MAX_ALLOWED_SPEED WORLD_TO_VELOCITY (DISPLAY_TO_WORLD (18))
				DWORD current_speed;

				if (((StarShipPtr->cur_status_flags & SHIP_IN_GRAVITY_WELL)
						&& desired_speed <=
						(DWORD)MAX_ALLOWED_SPEED * (DWORD)MAX_ALLOWED_SPEED)
						|| (current_speed =
						(DWORD)((long)cur_delta_x * (long)cur_delta_x)
						+ (DWORD)((long)cur_delta_y * (long)cur_delta_y)) > desired_speed)
				{
					SetVelocityComponents (VelocityPtr, delta_x, delta_y);
					return (SHIP_AT_MAX_SPEED | SHIP_BEYOND_MAX_SPEED);
				}
				else if (TravelAngle == CurrentAngle)
				{
					if (current_speed <= max_speed)
						SetVelocityVector (VelocityPtr,
								max_thrust, StarShipPtr->ShipFacing);
					return (SHIP_AT_MAX_SPEED);
				}
				else
				{
					VELOCITY_DESC v;

					v = *VelocityPtr;

					DeltaVelocityComponents (&v,
							COSINE (CurrentAngle, thrust_increment >> 1)
							- COSINE (TravelAngle, thrust_increment),
							SINE (CurrentAngle, thrust_increment >> 1)
							- SINE (TravelAngle, thrust_increment));
					GetCurrentVelocityComponents (&v, &cur_delta_x, &cur_delta_y);
					desired_speed = (long)cur_delta_x * (long)cur_delta_x
							+ (long)cur_delta_y * (long)cur_delta_y;
					if (desired_speed > max_speed)
					{
						if (desired_speed < current_speed)
							*VelocityPtr = v;
						return (SHIP_AT_MAX_SPEED | SHIP_BEYOND_MAX_SPEED);
					}

					*VelocityPtr = v;
				}
			}
	}


	*/
	if (plr->accel_inc == plr->maxspeed)
	{
			//arilou
			plr->xspeed = (s32)(double(plr->maxspeed) * SIN[plr->angle])>>9;
			plr->yspeed = (s32)(double(plr->maxspeed) * COS[plr->angle])>>9;
			//return (SHIP_AT_MAX_SPEED);
	}

	else
	{/*
		s32 velocity_angle=FindAngle(0,0,plr->xspeed,plr->yspeed);
		print("\nvel ang");print(velocity_angle);print("ship angle");print(plr->angle);
		if (TurnAngle(velocity_angle,plr->angle,45)==0)
		{
			plr->speed+=plr->accel_inc;
			if (plr->speed>plr->maxspeed)
				plr->speed=plr->maxspeed;
			plr->thrustspeed=plr->speed;
			print("continue to thrust");

			plr->xspeed = (s32)(double(plr->thrustspeed) * SIN[plr->angle])>>9;
			plr->yspeed = (s32)(double(plr->thrustspeed) * SIN[plr->angle])>>9;


		}
		else
		{*/
			if(plr->thrustangle==plr->angle)
			{
				plr->thrustspeed=distanceBetweenPoints(0,0,plr->xspeed,plr->yspeed);
				plr->thrustspeed+=plr->accel_inc;
				if (plr->thrustspeed>plr->maxspeed)
					plr->thrustspeed=plr->maxspeed;
				print("thrust speed");print(plr->thrustspeed);

				/*
				s32 velocity_angle=FindAngle(0,0,plr->xspeed,plr->yspeed);
				print("\nvel ang");print(velocity_angle);print("ship angle");print(plr->angle);
				if (TurnAngle(velocity_angle,plr->angle,45)==0)
				{
					plr->thrustspeed+=plr->accel_inc;
					if (plr->thrustspeed>plr->maxspeed)
						plr->thrustspeed=plr->maxspeed;
				}
				*/

				print("thrust same a");
			}
			else
			{
				plr->thrustspeed=plr->accel_inc;
				print("new thrust");
				plr->thrustangle=plr->angle;
			}

		 	x = (s32)(double(plr->thrustspeed) * SIN[plr->angle])>>8;
			y = (s32)(double(plr->thrustspeed) * COS[plr->angle])>>8;

		//	x = (double(plr->accel_inc) * SIN[plr->angle]);
		//	y = (double(plr->accel_inc) * COS[plr->angle]);


			plr->xspeed = (plr->xspeed + x)>>1;
			plr->yspeed = (plr->xspeed + x)>>1;
	//	}
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
		pl->batt_turn=pl->batt_wait;
		ModifyBatt(pl,pl->batt_regen);
	}

	//if (pl->ship==TERMINATOR&&pl->shield>0)
	if (pl->shield>0)
	{
		pl->shield--;
		if (pl->shield==0)
		{
			pl->spriteoffset=0;
			sprites[(pl->plr=1)?0:13].attribute2 = pl->SpriteStart+pl->spriteoffset | PRIORITY(1);
		}
	}

}

void RotateSprite(int rotDataIndex, s32 angle, s32 x_scale,s32 y_scale)
{

	s32 pa,pb,pc,pd;

	pa = ((x_scale) * (s32)COS[angle])>>8;    //(do my fixed point multiplies and shift back down)
	pb = ((y_scale) * (s32)SIN[angle])>>8;
	pc = ((x_scale) * (s32)-SIN[angle])>>8;
	pd = ((y_scale) * (s32)COS[angle])>>8;


	rotData[rotDataIndex].pa = pa;  //put them in my data struct
	rotData[rotDataIndex].pb = pb;
	rotData[rotDataIndex].pc = pc;
	rotData[rotDataIndex].pd = pd;
}

s32 distanceBetweenPoints(s32 xpos1,s32 ypos1,s32 xpos2,s32 ypos2)
{
	s32 x,y,z;
	if (xpos1>xpos2)
		x=xpos1-xpos2;
	else
		x=xpos2-xpos1;
	if (ypos1>ypos2)
		y=ypos1-ypos2;
	else
		y=ypos2-ypos1;

	z = (s32)sqrt((x*x)+(y*y));
	return z;
}

inline void findCentre(s32 *x,s32 *y,pPlayer p1,pPlayer p2)
{
	*x = ((p1->xpos+p2->xpos)/2);//-32;
	*y = ((p1->ypos+p2->ypos)/2);//-32;
}

void shifteverythingx(pPlayer p1,pPlayer p2,int shift)
{
	//shift everything except the planet

	p1->xpos+=shift;
	p2->xpos+=shift;
	screenx+=shift;
	int i;
	for (i=0;i<12;i++)
	{
		p1->weapon[i].xpos+=shift;
		p2->weapon[i].xpos+=shift;
	}
	for (i=0;i<10;i++)
	{
		trails[i].xpos+=shift;
	}
	//asteroids
}

void shifteverythingy(pPlayer p1,pPlayer p2,int shift)
{
	//shift everything except the planet

	p1->ypos+=shift;
	p2->ypos+=shift;
	screeny+=shift;
	int i;
	for (i=0;i<12;i++)
	{
		p1->weapon[i].ypos+=shift;
		p2->weapon[i].ypos+=shift;
	}
	for (i=0;i<10;i++)
	{
		trails[i].ypos+=shift;
	}
	//asteroids
}


void setScreen(pPlayer p1,pPlayer p2,Bg* bg0,Bg* bg1)
{
	s32 x, y, changex,changey;
	findCentre(&x,&y,p1,p2);

	changex = screenx-x;
	changey = screeny-y;
	/*
	bg0.x_scroll-=2*changex;
    bg1.x_scroll-=changex;

	bg0.y_scroll-=2*changey;
    bg1.y_scroll-=changey;
*/

	bg0->x_scroll-=2*changex;
    bg1->x_scroll-=changex;

	bg0->y_scroll-=2*changey;
    bg1->y_scroll-=changey;

	screenx=x;
	screeny=y;

	x=screenx-50;
	y=screenx-50;

	s32 d= distanceBetweenPoints(p1->xpos,p1->ypos,p2->xpos,p2->ypos);
	//Calcuate zoom level


	if (d<160)
	{
		zoom = 1<<8; //256
		scale = 1;
	}
	else if (d < 320) //needs calc
	{
		zoom=1<<9;//512
		scale = 0.5;

	}
	else
	{
		zoom=768;
		scale = 0.25;
		//scale=4;

		//if reached the end of space swap ships

		s32 xd = p1->xpos-p2->xpos;
		if (xd<-960 || xd>960)
		{
			if (screenx>1000)
				shifteverythingx(p1,p2,-500);
			else
				shifteverythingx(p1,p2,500);
			swap(&p1->xpos,&p2->xpos);
		}
		s32 yd = p1->ypos-p2->ypos;
		if (yd<-640 || yd>640)
		{
			swap(&p1->ypos,&p2->ypos);
			if (screeny>1000)
				shifteverythingy(p1,p2,-500);
			else
				shifteverythingy(p1,p2,500);
		}

	}



	// want to reset so values dont overflow

	if(screenx<50)
		shifteverythingx(p1,p2,1600);
	if (screenx>1950)
		shifteverythingx(p1,p2,-1600);
	if(screeny<50)
		shifteverythingy(p1,p2,1600);
	if (screeny>1950)
		shifteverythingy(p1,p2,-1600);
/*
	if(p1->xpos>8000||
		p2->xpos>8000)
	{
		shifteverythingx(p1,p2,-6000);
	}
	if (p1->ypos>8000||
		p2->ypos>8000)

	{
		shifteverythingy(p1,p2,-6000);
	}
	if(p1->xpos<0||
		p2->xpos<0)
	{
		shifteverythingx(p1,p2,6000);
	}
	if(p1->ypos<0||
		p2->ypos<0)
	{
		shifteverythingy(p1,p2,6000);
	}

*/
	drawOnScreen(&p1->xscreen,&p1->yscreen,p1->xpos,p1->ypos,screenx,screeny,32,1);
	drawOnScreen(&p2->xscreen,&p2->yscreen,p2->xpos,p2->ypos,screenx,screeny,32,1);
}

void drawOnScreen(s16* x,s16* y,s32 xpos,s32 ypos, s16 screenx, s16 yscreen,s16 size,s16 pl)
{
//	*x = (s16)(((xpos-screenx)*scale)+centrex-(size/scale));
//	*y = (s16)(((ypos-screeny)*scale)+centrey-(size/scale));


	*x = (s16)(((xpos-screenx)*scale)+centrex-(size));
	*y = (s16)(((ypos-screeny)*scale)+centrey-(size));

//	*x = (s16)(((xpos-screenx)*scale)+centrex);
//	*y = (s16)(((ypos-screeny)*scale)+centrey);

	if (!pl)
	{
		//dont draw on screen if out of bounds
		if (*x<0-16||*x>240+16||*y<0-16||*y>160+16)
		{
			*x=304;
			*y=224;
		}
	}

	if (*x <0)
		*x+=512;
	if (*y <0)
		*y+=255;



}
/*
int CanHitOpp(pPlayer pl)
{
	pPlayer opp = (pPlayer)pl->opp;
	s16 angle = FindAngle(pl->xpos,pl->ypos,opp->xpos,opp->ypos);

	s16 a1 = 360+angle;
	s16 a2 = 360+pl->angle;

	s16 x=30;
	if (pl->ship==FURY)
		x=100;
	if (a1>a2-x&&a1<a2+x)//are they roughly at right angle?
	{
		return 1;
	}
	return 0;
}
*/
int TurnAngle(s16 yourangle, s16 desiredangle,s8 precision)
{
	//takes your angle, the angle you want and possilbe a precsion
	//returns 0 if on right path
	//-1 if need to turn left
	//1 if need to turn right

	int diff = yourangle-desiredangle;


	int posdiff=(diff<0)?diff*-1:diff;

	if (posdiff<=precision)
		return 0;

	if (diff<0)
	{
		if (diff<-180)
			return 1;
		else
			return-1;
	}

	//if (yourangle>desiredangle)//else
	else
	{
		if (diff>180)
			return -1;
		else
			return 1;
	}

	return 0;
}

void aiTurn(pPlayer ai)
{

	//PursueShip ( ai, (pPlayer)ai->opp);
	//Entice(ai);
	//ship_intelligence (ai);
	tactical_intelligence (ai);

	if (ai->ship_input_state & LEFT)
				TurnLeft(ai);
		else if (ai->ship_input_state & RIGHT)
				TurnRight(ai);
		if (ai->ship_input_state & THRUST)
				Thrust(ai);;
		if (ai->ship_input_state & WEAPON)
				Fire(ai);
		if (ai->ship_input_state & SPECIAL)
			Special(ai);

	/*
	//do special?
	if (ai->aiturn==0)
	{
		if (ai->ai!=STANDARD)//only do specials on higher skill
		{

			if(ai->aispecial==0)
			{
				if (ai->aispecfunc(ai))//decide if do special
					ai->aispecial=1;
			}
			if (ai->aispecial==1)//do special
			{
				if (ai->batt>ai->specbatt)
				{
					if(Special(ai)>0)
						ai->aispecial=0;
				}
			}

		}

	pPlayer opp = (pPlayer)ai->opp;
	s16 angle = FindAngle(ai->xpos,ai->ypos,opp->xpos,opp->ypos);
	s16 oangle = angle+(angle<180)?180:-180;
	//s16 desangle=angle;



	int InRangeToFire=InRange(ai->xpos,ai->ypos,opp->xpos,opp->ypos,ai->range);

	int InRangeToBeHit=(TurnAngle(oangle,opp->angle,opp->fireangle)==0)&&InRange(opp->xpos,opp->ypos,ai->xpos,ai->ypos,opp->range);

	if(InRangeToBeHit)
	{
		angle=angle+180;//should work this out better!
		if (angle>360)
			angle-=360;
		int ret=TurnAngle(angle,ai->angle,90);
		if (ret==0)
			Thrust(ai);
		else if (ret<0)
			TurnLeft(ai);
		else if (ret>0)
			TurnRight(ai);

	}

	else if (InRangeToFire)
	{
		//desange=angle still

		//are we pointing towards opp? if so fire
		//s16 a1 = 360+angle;
		//s16 a2 = 360+ai->angle;


		if (ai->aispecial==0&&TurnAngle(angle,ai->angle,ai->fireangle)==0)//fire if going roughly the right way
		{
			Fire(ai);
		}

		int ret=TurnAngle(angle,ai->angle,15);
		if (ret<0)
			TurnLeft(ai);
		else if (ret>0)
			TurnRight(ai);



	}
	else //not in range
	{
		if (ai->range<opp->range)//we have shorter range
		{
			//choose a spot to the side of opponent
			//again better
			angle=angle-40;
		}

		int ret=TurnAngle(angle,ai->angle,30);
		if (ret==0)
			Thrust(ai);
		else if (ret<0)
			TurnLeft(ai);
		else if (ret>0)
			TurnRight(ai);

	}

	ai->aiturn=ai->ai;//*3;

	//do special?
	}	//end if turn
	*/
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

int ModifyAngle(s16 a,int o)
{
	s16 r = a+o;
	if (r>360)
		r-=360;
	else if (r<0)
		r+=360;
	return r;
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

inline void Bounce(pPlayer pl)
{
	pl->xspeed*=-1;
	pl->yspeed*=-1;

	pl->xpos+=pl->xspeed;
	pl->xpos+=pl->xspeed;
}

void CreateAsteroid(int i)
{
	print("asteroid created");
	s16 d = ran(0,240);

	asteroids[i].xpos=screenx+(s32)((d-centrex)/scale);
	d=ran(0,1);
	d*=160;
	print("d=");
	print(d);
	asteroids[i].ypos=screeny+(s32)((d-centrey)/scale);
	//asteroids[i].ypos=screeny;

	/*

	*x = (s16)(((xpos-screenx)*scale)+centrex-(size));
	asteroids[i].xpos=screenx+d+(s32)(d<0?-120/scale:120/scale);
	asteroids[i].ypos=screeny+d+(s32)(d<0?-80/scale:80/scale);

	asteroids[i].xpos=screenx+d+(s32)(d<0?-480:480);
	asteroids[i].ypos=screeny+d+(s32)(d<0?-320:320/scale);
*/
	//asteroids[i].xpos=screenx;
	//asteroids[i].ypos=screeny;
	asteroids[i].life=1;

	print("\n xpos");
	print(asteroids[i].xpos);
	print("\n ypos");
	print(asteroids[i].ypos);

	print("\nscreenx");
	print(screenx);
	print("\nscreeny");
	print(screeny);

	//s32 angle=ran(0,24)*15;
	s32 angle=FindAngle(screenx,screeny,asteroids[i].xpos,asteroids[i].ypos);
	asteroids[i].xspeed = ((2) * (s32)SIN[angle])>>8;
	asteroids[i].yspeed = ((2) * (s32)COS[angle])>>8;

	print("\nangle");
	print(angle);

	sprites[26+i].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 160;	//setup sprite info, 256 colour, shape and y-coord
    sprites[26+i].attribute1 = SIZE_32 | ROTDATA(26+i) | 240;
    sprites[26+i].attribute2 = SpriteAsteroidStart | PRIORITY(2);
}

void BounceAsteroid(int i)
{
	asteroids[i].xspeed*=-1;
	asteroids[i].yspeed*=-1;
}

void ProcessAsteroids(pPlayer p1,pPlayer p2)
{
	s16 dist;
	for (int i=0;i<5;i++)
	{
		if (asteroids[i].life==0)
		{
			CreateAsteroid(i);
		}

		//planet
		dist=distanceBetweenPoints(asteroids[i].xpos,asteroids[i].ypos,planetx,planety);
		if (dist<64)
		{
			print("\n ast hit planet");
			asteroids[i].life=0;
		}
		else
		{

		//if asteroid hit player
		dist=distanceBetweenPoints(p1->xpos,p1->ypos,asteroids[i].xpos,asteroids[i].ypos);
		if (dist<(p1->offset+25)/2)
		{
			//ModifyCrew(p1,-1);
			Bounce(p1);
			BounceAsteroid(i);
			print("\n ast hit p1");
		}
		dist=distanceBetweenPoints(p2->xpos,p2->ypos,asteroids[i].xpos,asteroids[i].ypos);
		if (dist<(p2->offset+25)/2)
		{
			//ModifyCrew(p2,-1);
			Bounce(p2);
			BounceAsteroid(i);
			print("\n ast hit p2");
		}
		//each other
		for (int j=0;j<5;j++)
		{
			if (i!=j)
			{
				dist=distanceBetweenPoints(asteroids[j].xpos,asteroids[j].ypos,asteroids[i].xpos,asteroids[i].ypos);
				if (dist<25)
				{
					BounceAsteroid(i);
					BounceAsteroid(j);
				}
			}
		}


		asteroids[i].xpos+=asteroids[i].xspeed;
		asteroids[i].ypos+=asteroids[i].yspeed;

		if (asteroids[i].xpos<-1000||asteroids[i].xpos>3000||
			asteroids[i].ypos<-1000||asteroids[i].ypos>3000)
		{
			asteroids[i].life=0;
			print("\n ast out of bounds");
		}
		else
		{
			RotateSprite(i+26, 0, zoom,zoom);
			drawOnScreen(&asteroids[i].xscreen,&asteroids[i].yscreen,asteroids[i].xpos,asteroids[i].ypos,screenx,screeny,32);
			MoveSprite(&sprites[i+26],asteroids[i].xscreen,asteroids[i].yscreen);
		}
	}//not hit planet
	}//end for loop
}//end func

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
			else if(pl->ai!=DISABLED)
				tactical_intelligence (pl);

		}


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

		pl->xpos+=pl->xspeed;
		pl->ypos-=pl->yspeed;
		pl->speed=distanceBetweenPoints(0,0,pl->xspeed,pl->yspeed);

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

				else  //oldstyle
				{

				if (pl->ship_input_state & LEFT)
				{
					sprites[43+off].attribute0 = COLOR_256 | TALL  | 0;
					sprites[43+off].attribute1 = SIZE_64 | x;
					sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+64 | PRIORITY(2);

				}
				else if (pl->ship_input_state & RIGHT)
				{
					sprites[43+off].attribute0 = COLOR_256 | TALL  | 0;
					sprites[43+off].attribute1 = SIZE_64 | x;
					sprites[43+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+128 | PRIORITY(2);

				}
				else
				{
					sprites[43+off].attribute0 = COLOR_256 | TALL  | 160;
					sprites[43+off].attribute1 = SIZE_64 | 240;
				}

				if (pl->ship_input_state & THRUST)
				{
					sprites[44+off].attribute0 = COLOR_256 | TALL  | 0;
					sprites[44+off].attribute1 = SIZE_64 | x;
					sprites[44+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+192 | PRIORITY(2);

				}
				else
				{
					sprites[44+off].attribute0 = COLOR_256 | TALL  | 160;
					sprites[44+off].attribute1 = SIZE_64 | 240;
				}
				/*
				if (pl->ship_input_state & WEAPON)
				{
					sprites[45+off].attribute0 = COLOR_256 | TALL  | 0;
					sprites[45+off].attribute1 = SIZE_64 | x;
					sprites[45+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+256 | PRIORITY(2);
				}
				else
				{
					sprites[45+off].attribute0 = COLOR_256 | TALL  | 160;
					sprites[45+off].attribute1 = SIZE_64 | 240;
				}

				if (pl->ship_input_state & SPECIAL)
				{
					sprites[46+off].attribute0 = COLOR_256 | TALL  | 0;
					sprites[46+off].attribute1 = SIZE_64 | x;
					sprites[46+off].attribute2 = pl->SpriteStart+pl->pilot_sprite+288 | PRIORITY(2);
				}
				else
				{
					sprites[46+off].attribute0 = COLOR_256 | TALL  | 160;
					sprites[46+off].attribute1 = SIZE_64 | 240;
				}
				*/
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
			sprites[42+off].attribute0 = COLOR_256 | TALL  | 160;
			sprites[42+off].attribute1 = SIZE_64 | 240;
		}
		pl->EndGame--;
		DetonateShip(pl);
	}





	/* not gonna
	else
	{
		sprites[42].attribute0 = COLOR_256 | TALL  | 0;
		sprites[42].attribute1 = SIZE_64 | 509;
		sprites[42].attribute2 = p1->SpriteStart+p1->pilot_sprite | PRIORITY(3);
	}
	*/
}



void DrawPlanet()
{
	drawOnScreen(&planet_screenx,&planet_screeny,planetx,planety,screenx,screeny,64);
	RotateSprite(31, 0, zoom, zoom);
	sprites[31].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | planet_screeny;	//setup sprite info, 256 colour, shape and y-coord
	sprites[31].attribute1 = SIZE_64 | ROTDATA(31) | planet_screenx;
	sprites[31].attribute2 = PlanetSprite | PRIORITY(1);
}

void CalcPlanet(pPlayer pl)
{
	s16 dist=distanceBetweenPoints(pl->xpos,pl->ypos,planetx,planety);
	if (dist<(pl->offset+64)/2)
	{
		ModifyCrew(pl,-2,1);
		Bounce(pl);
	}
	//gravity
	if (pl->ship!=SKIFF&&dist<70)//maybe
	{
		s16 a = FindAngle(pl->xpos,pl->ypos,planetx,planety);

		s32 x = ((2) * (s32)SIN[a])>>8;
		s32 y = ((2) * (s32)COS[a])>>8;

		pl->xspeed = pl->xspeed + x;
		pl->yspeed = pl->yspeed + y;

	}
}

void Melee(pPlayer p1,pPlayer p2,Bg *bg0, Bg *bg1)
{
	u32 loop;       //generic loop variable
	print("Melee started");

	screenx=100;
	screeny=100;

	pilot=1;


	//holds planet vars
	//may make these global


	trails=(pTrail)malloc(sizeof(Trail)*10);
	for (int i=0;i<10;i++)
	{
		trails->life=0;
	}

	asteroids=(pAsteroid)malloc(sizeof(Asteroid)*5);
	{
		for(int i=0;i<5;i++)
		{
			asteroids->life=0;
		}
	}


	SetupBackground(bg0,bg1);

	//try - should load in sc2.cpp but oam mem seems to be blank...
	LoadPal();
	print("load pal");
	//LoadShip(p1);
	//LoadShip(p2);
	p1->loadfunc(p1->SpriteStart);
	p2->loadfunc(p2->SpriteStart);
	print("load ships");
	LoadExp(FireSprite1*16);
	LoadTrail(TrailSprite*16);
	LoadPlanet(PlanetSprite*16);
	LoadAsteroid(SpriteAsteroidStart*16);

//	p1->crew=1;

	InitializeSprites();                       //set all sprites off screen (stops artifact)

	RestoreGFX(p1);
	RestoreGFX(p2);





	setScreen(p1,p2,bg0,bg1);

		//create pl1 + pl2
       	sprites[0].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | p1->yscreen;	//setup sprite info, 256 colour, shape and y-coord
        sprites[0].attribute1 = SIZE_32 | ROTDATA(0) | p1->xscreen;            //size 32x32 and x-coord
        sprites[0].attribute2 = p1->SpriteStart+p1->spriteoffset | PRIORITY(1);                      //pointer to tile where sprite starts

		sprites[13].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT |p2->yscreen;//setup sprite info, 256 colour, shape and y-coord
        sprites[13].attribute1 = SIZE_32 | ROTDATA(13) | p2->xscreen;           //size 16x16 and x-coord
        sprites[13].attribute2 = p2->SpriteStart+p2->spriteoffset | PRIORITY(1);     //pointer to tile where sprite starts
/*
        //test loop
        	zoom=100;
        	screenx=120;
			screeny=80;
			p1->xpos=120;
			p1->ypos=80;
			p1->angle=0;
			MoveSprite(&sprites[13], -100, -100);
			while (1)
			{

				if(!(*KEYS & KEY_A))                	//if the A key is pressed
					{
				       zoom--;
				    }
					if(!(*KEYS & KEY_B))                	//if the B key is pressed
					{
						zoom++;
    				}
    				drawOnScreen(&p1->xscreen,&p1->yscreen,p1->xpos,p1->ypos,screenx,screeny,32,1);
    					RotateSprite(0, p1->angle, zoom, zoom);
					MoveSprite(&sprites[0], p1->xscreen, p1->yscreen);
				WaitForVsync();					//waits for the screen to stop drawing
				CopyOAM();

			}

*/


/*
// test show explos
// handy for testing sprites
        	sprites[60].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[60].attribute1 = SIZE_32 | 100;
			   	sprites[60].attribute2 = SpriteAsteroidStart | PRIORITY(0);
/*
			   	sprites[61].attribute0 = COLOR_256 | SQUARE | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[61].attribute1 = SIZE_8 | 20;
			   	sprites[61].attribute2 = FireSprite1+2 | PRIORITY(0);

			   	sprites[62].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[62].attribute1 = SIZE_16 | 40;
			   	sprites[62].attribute2 = FireSprite1+10 | PRIORITY(0);

			   	sprites[63].attribute0 = COLOR_256 | SQUARE   | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[63].attribute1 = SIZE_16 | 60;
			   	sprites[63].attribute2 = FireSprite1+18 | PRIORITY(0);

			   	sprites[64].attribute0 = COLOR_256 | SQUARE  | 130;	//setup sprite info, 256 colour, shape and y-coord
				sprites[64].attribute1 = SIZE_16 | 80;
			   	sprites[64].attribute2 = FireSprite1+26 | PRIORITY(0);
*/


//Pilots
//42 - p1 pilot
//43-47 p1 (left/right)/thrust/fire/special
//48 - p2 pilo5
//49-54 = p2
if (pilot)
{
sprites[42].attribute0 = COLOR_256 | TALL  | 0;
sprites[42].attribute1 = SIZE_64 | 509;
sprites[42].attribute2 = p1->SpriteStart+p1->pilot_sprite | PRIORITY(3);
p1->loadpilots(p1);

sprites[48].attribute0 = COLOR_256 | TALL  | 0;
sprites[48].attribute1 = SIZE_64 | 179;
sprites[48].attribute2 = p2->SpriteStart+p2->pilot_sprite | PRIORITY(3);
p2->loadpilots(p2);
}
		SetupStatus(p1,p2);
		//setScreen(p1,p2);
		turn=0;
		WaitForVsync();
		//s8 EndGame1=20;
		//s8 EndGame2=20;
		//UpdateStatus();
print("before loop");
		while(p1->EndGame&&p2->EndGame)                                //main loop
        {
print("\nloop");
			ProcessPlayer(p1);
			ProcessPlayer(p2);



  			//should also check for collisions with
  			//other player
  			//this isnt too great
  			if (distanceBetweenPoints(p1->xpos,p1->ypos,p2->xpos,p2->ypos)<(p1->offset+p2->offset)/2&&p1->crew>0&&p2->crew>0)
  			{
			//	ModifyCrew(p1,-2);
			//	ModifyCrew(p2,-2);
				Bounce(p1);
				Bounce(p2);
			}
  			//asteroids
  			ProcessAsteroids(p1,p2);
  			//planet
  			CalcPlanet(p1);
  			CalcPlanet(p2);

			setScreen(p1,p2,bg0,bg1);

			MoveBullets(p1);

			MoveBullets(p2);

			MoveSprite(&sprites[0], p1->xscreen, p1->yscreen);

			MoveSprite(&sprites[13], p2->xscreen, p2->yscreen);

			//asteroids
  			ProcessAsteroids(p1,p2);


			turn++;

			if (turn==5)
			{
				turn=0;

			}


			DrawTrails();
			DrawPlanet();

			//UpdateStatus();  now we only update when changed
/*
			if (p1->crew>0)
				Regen(p1);
			if (p2->crew>0)
				Regen(p2);
*/			if (p1->crew>0)
				Regen(p1);
			if (p2->crew>0)
				Regen(p2);


			RotateSprite(0, p1->angle, zoom, zoom);
			RotateSprite(13, p2->angle, zoom,zoom);

			WaitForVsync();					//waits for the screen to stop drawing
			UpdateBackground(bg0);
			UpdateBackground(bg1);
			CopyOAM();			//Copies sprite array into OAM.

		}//end while one or both ships are destroyed
	print("\n melee game ended");
	//should clear screen
	InitializeSprites();
	CopyOAM();
}




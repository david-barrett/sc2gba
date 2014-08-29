//////////////////////////////////////////////////////////////////////////
// File: 	star control gba.cpp                                            	//
// Description: a sc2 clone			//
// Author:	david barrett 	  					//
// Date: 	4th Aug 2004                                      	//
//////////////////////////////////////////////////////////////////////////

#include "gba.h"	//GBA register definitions

#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"
#include "bg.h"

#include "sc2title.h"

#include<math.h>
#include<stdlib.h>

//create an OAM variable and make it point to the address of OAM
u16* OAM = (u16*)0x7000000;
//create the array of sprites (128 is the maximum)
pOAMEntry sprites;

//create the rotation and scaling array (overlaps the OAMEntry array memory)
pRotData rotData;// = (pRotData)sprites;

extern const unsigned short sc2title[];

pBg bg0;
pBg bg1;

pPlayer p1;
pPlayer p2;

//int turn;

FIXED SIN2[360];	    //Look-Up Tabless for sign and cosign
FIXED COS2[360];

FIXED ATAN[360];

//double scale;

//s32 zoom,screenx,screeny;
//array to hold ships
int p1ships[7] = {DREADNAUGHT,FURY,DREADNAUGHT,FURY,DREADNAUGHT,FURY,DREADNAUGHT};
int p2ships[7]= {FURY,DREADNAUGHT,FURY,DREADNAUGHT,FURY,DREADNAUGHT,FURY};



#define ASIZE  512 //128 // 512 ?

//Copy our sprite array to OAM

void PlotPixel(int x,int y, unsigned short int c)
{
	VideoBuffer[(y) * 120 + (x)] = (c);
}

int getP1Ship()
{
	for (int i=0;i<7;i++)
	{
		if (p1ships[i] != -1)
		{
			p1->ship=p1ships[i];
			p1ships[i] = -1;
			return i;
		}
	}
	return -1;
}

int getP2Ship()
{
	for (int i=0;i<7;i++)
	{
		if (p2ships[i] != -1)
		{
			p2->ship=p2ships[i];
			p2ships[i] = -1;
			return i;
		}
	}
	return -1;
}

int main()
{
	u32 loop;       //generic loop variable

	p1=(pPlayer)malloc(sizeof(Player));
	p2=(pPlayer)malloc(sizeof(Player));
	bg0=(pBg)malloc(sizeof(Bg));
	bg1=(pBg)malloc(sizeof(Bg));
	sprites=(pOAMEntry)malloc(sizeof(Bg)*128);
	rotData = (pRotData)sprites;

	p1->plr=1;
	p2->plr=2;
	p1->opp=(void*)p2;
	p2->opp=(void*)p1;


p1->xpos = 2038;	//variables to hold position of sprite on screen
p1->ypos = 2023;
p1->angle = 305;
p1->thrustangle = 305;
p1->xspeed =0;
p1->yspeed =0;
p1->speed =0;
p1->ship = DREADNAUGHT;
//p1->ship = FURY;




p2->xpos = 1962;	//variables to hold position of sprite on screen
p2->ypos = 1977;
p2->angle = 0;
p2->xspeed =0;
p2->yspeed =0;
p1->speed =0;
p2->ship = FURY;
//p2->ship = DREADNAUGHT;


p1->OAMStart =P1_OAMStart;
p1->SpriteStart =P1_SpriteStart;

p2->OAMStart =P2_OAMStart;
p2->SpriteStart =P2_SpriteStart;


//tests
p1->xpos = 2038;	//variables to hold position of sprite on screen
p1->ypos = 2023;
p2->xpos = 1962;	//variables to hold position of sprite on screen
p2->ypos = 1977;


for (int i=0;i<12;i++)
{
	p1->weapon[i].sprite=i+1;
	p1->weapon[i].life=-1;
	p2->weapon[i].sprite=i+14;
	p2->weapon[i].life=-1;
}
// transparncy

	REG_BLDMOD = 0x2F40;
	//
	//	15   14   13   12   11   10   9    8    7    6    5    4    3    2    1    0
	//	          BD   OBJ  BG3  BG2  BG1  BG0   mode     BD  OBJ  BG3  BG2  BG1  BG0
	//			  1    0    1    1    1    1    0    1    0    0    0    0    0    0
	//
	// What this does is cause all sprites with transparency enabled to be blended with
	// all backgrounds, but not other sprites (which is impossible anyway :p)

	// This register controls the level of transparency
	REG_COLV = 0x0010;
	// Opaque = 0x0010
	// Midway = 0x0808
	// Clear  = 0x1000
	// So as you can see, for these blending options, as the sprite becomes
	// more transparent, the high byte of REG_COLV increases, and the low byte
	// decreases.

    for(loop = 0; loop < 360; loop++)
	{
		SIN2[loop] = (FIXED)(sin(RADIAN(loop)) * 256);  //sin and cos are computed and cast to fixed							//fixed
		COS2[loop] = (FIXED)(cos(RADIAN(loop)) * 256);
		ATAN[loop] = (FIXED)(atan(RADIAN(loop)) * 256);

	}


       SetMode(MODE_1 | OBJ_ENABLE | OBJ_MAP_1D); //set mode 1 and enable sprites and 1d mapping
       /* cant get title to do anything sensable
       	//draw title

		SetMode(MODE_4 | BG2_ENABLE);
		for(loop = 0; loop < 256; loop++)          //load the sprite palette into memory
			BGPaletteMem[loop] = sc2titlePalette[loop];
		int x,y;
			//while(*KEYS & KEY_START)//wait until start key?
			while(1)
			{
				for(y = 0; y < 96; y++)                  //screen height
				{
					for(x = 0; x < 120; x++)          //screen width
					{
						PlotPixel(x,y+30, sc2titleData[y*120+x]);	//load image data into
					}						//memory pixel by pixel
				}
		}
		*/
		int p1c=0;
		int p2c=0;
		p1->ship=p1ships[0];
		p2->ship=p2ships[0];
		SetShip(p1);
		SetShip(p2);
		SetNew(p1);
		SetNew(p2);
		LoadPal();
		LoadShip(p1);
		LoadShip(p2);
		LoadExp(OAMFireSprite1,FireSprite1);
		LoadTrail(OAMTrailSprite);

		p1->ai=PLAYER1;
		p2->ai=STANDARD;
		do
		{
       		Melee(p1,p2,bg0,bg1);
       		if (p1->crew<1)
       		{
				p1c++;
				if (p1c<7)
				{
       				p1->ship=p1ships[p1c];//choose?
       				SetShip(p1);
       				SetNew(p1);
       				LoadShip(p1);
				}
       			else
       				p1c=-1;
			}

			if (p2->crew<1)
			{
				p2c++;
				if (p2c<7)
				{
			    	p2->ship=p2ships[p2c];//choose?
			    	SetShip(p2);
			    	SetNew(p2);
			    	LoadShip(p2);
				}
			    else
			    	p2c=-1;
			}

		}
		while (p1c=-1||p2c!=-1);

		//game over!!!


}



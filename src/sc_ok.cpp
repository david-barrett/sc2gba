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

//#include "sc2title.h"
#include "sc2titles.h"

#include<math.h>
#include<stdlib.h>

//create an OAM variable and make it point to the address of OAM
u16* OAM = (u16*)0x7000000;
//create the array of sprites (128 is the maximum)
pOAMEntry sprites;

//create the rotation and scaling array (overlaps the OAMEntry array memory)
pRotData rotData;// = (pRotData)sprites;

extern const unsigned short sc2title[];


int* KEYS =  (int*)0x04000130;
unsigned long state;

int ran(int min, int max)
{
  state *= 69069;
//  print("ran no=");
 //  print(min + ((max - min + 1) * (state >> (4 * sizeof(unsigned long)))) /
 //   (0x1 << (4 * sizeof(unsigned long))));
  return min + ((max - min + 1) * (state >> (4 * sizeof(unsigned long)))) /
    (0x1 << (4 * sizeof(unsigned long)));



}

void ChooseShips(pPlayer pl, pPlrList list)
{
	//chooses team of 14 ships

	//cheating
	for (int i=0;i<14;i++)
	{
		if  (i%2==0)
			list[i].ship=FURY;
		else
			list[i].ship=DREADNAUGHT;
		list[i].active=1;
	}
}

int ChooseNextShipRand(pPlrList list)
{
	int ret=1;
	int choose=-1;
	do
	{
		choose=ran(0,13);
		{
			if (list[choose].active=1)
				ret=0;
		}
	}
	while (ret);

	return choose;

}

int ChooseNextShip(pPlayer pl, pPlrList list)
{
	//choose next ship
	//returns ship id choosen
	//or -1 if no more ships

	int i;
	int found=0;
	int selected=-1;
	int x=2,y=2;
	InitializeSprites();

	for (i =0;i<14;i++)
	{	if (list[i].active==1)
		{
			found=1;
			sprites[42+i].attribute2 = SpriteAllShips+(list[i].ship*32) | PRIORITY(1);
		}
		else if (list[i].active==0)
		{
			sprites[42+i].attribute2 = SpriteAllShips+864 | PRIORITY(1);
		}
	}

	sprites[56].attribute2 = SpriteAllShips+832 | PRIORITY(1);
	if (found==0)
		return -1;

	if (pl->ai>PLAYER2)
	{
		return ChooseNextShipRand(list);
	}
	LoadPal();
	LoadAllShips(OAMAllships);
	for (i=0;i<5;i++)
	{
		sprites[42+i].attribute0 = COLOR_256 | SQUARE | 2;
    	sprites[42+i].attribute1 = SIZE_32 | 2+(i*34);
    	sprites[47+i].attribute0 = COLOR_256 | SQUARE | 36;
    	sprites[47+i].attribute1 = SIZE_32 | 2+(i*34);
    	sprites[52+i].attribute0 = COLOR_256 | SQUARE | 70;
    	sprites[52+i].attribute1 = SIZE_32 | 2+(i*34);
	}


	sprites[57].attribute2 = SpriteAllShips+800 | PRIORITY(0);
	sprites[57].attribute0 = COLOR_256 | SQUARE | MODE_TRANSPARENT |x;
    sprites[57].attribute1 = SIZE_32 | x;

	do
	{
    	WaitForVsync();
		CopyOAM();
		if(!(*KEYS & KEY_UP)&&y>2)                   //if the UP key is pressed
		{
			y=y-34;
			MoveSprite(&sprites[57],x,y);
		}
		if(!(*KEYS & KEY_DOWN)&&y<70)                 //if the DOWN key is pressed
		{
			y=y+34;
			MoveSprite(&sprites[57],x,y);
		}
		if(!(*KEYS & KEY_LEFT)&&x>2)                 //if the LEFT key is pressed
		{
		    x=x-34;
			MoveSprite(&sprites[57],x,y);
		}
		if(!(*KEYS & KEY_RIGHT)&&x<138)                //if the RIGHT key is pressed
		{
			x=x+34;
			MoveSprite(&sprites[57],x,y);
		}
		if(!(*KEYS & KEY_A))//||!(*KEYS & KEY_B))                	//if the A key is pressed
		{
			print("button pressed");
				//found=0;
				//selected = ?
			if (y==70)
			{
				if (x==138)
				{
					found=0;
					selected=ChooseNextShipRand(list);
				}
				else
				{
					int t=10+((x-2)/34);
					if (list[t].active==1)
					{
						found=0;
						selected=t;
					}
				}
			}
			else if (y==36)
			{
				int t=5+((x-2)/34);
				print("\nt = ");
				print(t);
				if (list[t].active==1)
				{
					found=0;
					selected=t;
				}
			}
			else
			{
				int t=((x-2)/34);
				print("\nt = ");
				print(t);
				if (list[t].active==1)
				{
						found=0;
						selected=t;
				}
			}

		}
		WaitForVsync();
		CopyOAM();

	}
	while (found);
	InitializeSprites();
	WaitForVsync();
	CopyOAM();
	return selected;
}



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
//int p1ships[8] = {FURY,DREADNAUGHT,FURY,DREADNAUGHT,FURY,DREADNAUGHT,FURY,DREADNAUGHT};
//int p2ships[7]= {FURY,DREADNAUGHT,FURY,DREADNAUGHT,FURY,DREADNAUGHT,FURY};



#define ASIZE  512 //128 // 512 ?

//Copy our sprite array to OAM
/*
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
*/
int main()
{
	u32 loop;       //generic loop variable
	KEYS =  (int*)0x04000130;

	sprites=(pOAMEntry)malloc(sizeof(OAMEntry)*128);
	if (sprites==NULL)
		print("sprites null");
	else
		print("sprites ok");

	state=1;

	sprites->attribute0=5;
	print("sprites a0 ");
	print(sprites->attribute0);
	print("sprites[0] a0 ");
	print(sprites[0].attribute0);


	p1=(pPlayer)malloc(sizeof(Player));
	if (p1==NULL)
			print("p1 null");
		else
		print("p1 ok");
	p2=(pPlayer)malloc(sizeof(Player));
	if (p2==NULL)
			print("p2 null");
		else
		print("p2 ok");
	bg0=(pBg)malloc(sizeof(Bg));
	if (bg0==NULL)
			print("bg0 null");
		else
		print("bg0 ok");
	bg1=(pBg)malloc(sizeof(Bg));
	if (bg1==NULL)
			print("bg1 null");
		else
		print("bg1 ok");

	rotData = (pRotData)sprites;

	pPlrList plist2=(pPlrList)malloc(sizeof(PlrList)*14);
	pPlrList plist1=(pPlrList)malloc(sizeof(PlrList)*14);

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
p1->ship = FURY;




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
		do//main infinite loop
		{
		for(loop = 0; loop < 256; loop++)          //load the sprite palette into memory
			OBJPaletteMem[loop] = sc2titlesPalette[loop];
		for(loop = OAMTitleStart; loop < OAMTitleStart+1024; loop++)               //load sprite image data
		{
			OAMData[loop] = sc2titlesData[loop-OAMTitleStart];
		}
		sprites[30].attribute0 = COLOR_256 | TALL | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 20;	//setup sprite info, 256 colour, shape and y-coord
		sprites[30].attribute1 = SIZE_64 | ROTDATA(30) | 50;            //size 32x32 and x-coord
		sprites[30].attribute2 = SpriteTitleStart | PRIORITY(1); //pointer to tile where sprite starts
		RotateSprite(30, 0, 128,128);
		WaitForVsync();
		CopyOAM();
		while (*KEYS & KEY_START);
		sprites[30].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 20;	//setup sprite info, 256 colour, shape and y-coord
		sprites[30].attribute1 = SIZE_8 | ROTDATA(30) | 50;
		MoveSprite(&sprites[30],240,160);
		//set to point to 0.
		WaitForVsync();
		CopyOAM();

		p1->ai=PLAYER1;
		p2->ai=STANDARD;
		//p1->ai=STANDARD;
		//p1->ai=AWESOME;
		p2->ai=DISABLED;

		ChooseShips(p1,plist1);
		ChooseShips(p2,plist2);

		print("\n first p1 ships is ");
		print(plist1[0].ship);

		//int p1c=0;
		//int p2c=0;
		s16 nextp1=ChooseNextShip(p1,plist1);
		p1->ship=plist1[nextp1].ship;
		s16 nextp2=ChooseNextShip(p2,plist2);
		p2->ship=plist2[nextp2].ship;
		SetShip(p1);
		SetShip(p2);
		SetNew(p1);
		SetNew(p2);
		LoadPal();
		LoadShip(p1);
		LoadShip(p2);
		LoadExp(OAMFireSprite1,FireSprite1);
		LoadTrail(OAMTrailSprite);


		do
		{
       		Melee(p1,p2,bg0,bg1);
       		if (p1->crew<1)
       		{
				plist1[nextp1].active=0;
				nextp1 = ChooseNextShip(p1,plist1);

				if (nextp1!=-1)
				{	p1->ship=plist1[nextp1].ship;
       				SetShip(p1);
       				SetNew(p1);
       				LoadShip(p1);
				}
			}

			if (p2->crew<1)
			{
				plist2[nextp2].active=0;
				nextp2 = ChooseNextShip(p2,plist2);

				if (nextp2!=-1)
				{	p2->ship=plist2[nextp2].ship;
					SetShip(p2);
				    SetNew(p2);
				    LoadShip(p2);
				}
			}

		}
		while (nextp1=-1&&nextp2!=-1);
		//game over!!!

	}//end loop - got back to titles;
	while(1);



}



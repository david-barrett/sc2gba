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
#include "bg.h"

//sound
#include "sfx.h"
#include "urquan_sfx.h"


typedef void (*fnptr)(void);
#define REG_INTMAIN *(fnptr*)(0x03fffffc)

#include<math.h>
#include<stdlib.h>

//create an OAM variable and make it point to the address of OAM
u16* OAM = (u16*)0x7000000;
//create the array of sprites (128 is the maximum)
pOAMEntry sprites;

s8 pilot;
skill aiskill=GOOD;
s8 demo;
s8 planet;
s8 sound;
s8 scaled;
s8 v3do;

//create the rotation and scaling array (overlaps the OAMEntry array memory)
pRotData rotData;// = (pRotData)sprites;

extern const unsigned short sc2title[];
s32 planetx,planety;

int* KEYS =  (int*)0x04000130;
unsigned long state;

//battery
#define GAMEPAK_RAM  ((u8*)0x0E000000)
u8 *pSaveMemory = GAMEPAK_RAM;


int ran(int min, int max)
{
  state *= 69069;
//  print("ran no=");
 //  print(min + ((max - min + 1) * (state >> (4 * sizeof(unsigned long)))) /
 //   (0x1 << (4 * sizeof(unsigned long))));
  return min + ((max - min + 1) * (state >> (4 * sizeof(unsigned long)))) /
    (0x1 << (4 * sizeof(unsigned long)));



}







pBg bg0;
pBg bg1;



//int turn;

//FIXED SIN2[360];	    //Look-Up Tabless for sign and cosign
//FIXED COS2[360];

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
	pPlayer p1;
	pPlayer p2;

	sprites=(pOAMEntry)malloc(sizeof(OAMEntry)*128);
	state=1;

	v3do=1;

	pilot=1;
	aiskill=GOOD;

	u8 tmp=pSaveMemory[0];

	if (tmp==1)
		aiskill=WEAK;
	else if (tmp==2)
		aiskill=AWESOME;
	else if (tmp==3)
		aiskill=DISABLED;
	else
		aiskill=GOOD;

	tmp=pSaveMemory[1];

	if (tmp==0)
		pilot=1;
	else
		pilot=0;

	tmp=pSaveMemory[2];

	if (tmp==0)
		sound=1;
	else
		sound=0;

	scaled=pSaveMemory[3];

	p1=(pPlayer)malloc(sizeof(Player));
	p2=(pPlayer)malloc(sizeof(Player));
	bg0=(pBg)malloc(sizeof(Bg));
	bg1=(pBg)malloc(sizeof(Bg));
	rotData = (pRotData)sprites;

	pPlrList plist2=(pPlrList)malloc(sizeof(PlrList)*14);
	pPlrList plist1=(pPlrList)malloc(sizeof(PlrList)*14);

	p1->plr=1;
	p2->plr=2;
	p1->opp=(void*)p2;
	p2->opp=(void*)p1;




p1->SpriteStart =P1_SpriteStart;
p2->SpriteStart =P2_SpriteStart;

planet=0;
	planetx=screencentrex;
	planety=screencentrey;

//tests
p1->xpos = 3638;	//variables to hold position of sprite on screen
p1->ypos = 3623;
p2->xpos = 3562;	//variables to hold position of sprite on screen
p2->ypos = 3577;

	//switch on interupts for timers 0 & 1
	REG_IE=0x0008 | 0x0010;
    REG_IME=1;
    REG_INTMAIN= InterruptProcess;

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


	//this slows it all down -remove.
	/*
	for(loop = 0; loop < 360; loop++)
				{
					SIN2[loop] = (FIXED)(sin(RADIAN(loop)) * 256);  //sin and cos are computed and cast to fixed							//fixed
					COS2[loop] = (FIXED)(cos(RADIAN(loop)) * 256);
			}

*/

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
		SetupBackground(bg0,bg1);

		init_sfx_system();
	//	printLargeNumber(25,1,11,1,2);



		do//main  loop
		{

		//play_sfx(&pkunk_stupid,0);
		demo=7;
		s16 count=0;
		play_sfx(&urquan_ditty,1);
		do //demo loop
		{
			DrawTitle();

			if((!(*KEYS & KEY_START)) || (!(*KEYS & KEY_SELECT)) ||
					(!(*KEYS & KEY_A)) || (!(*KEYS & KEY_B)) ||
					(!(*KEYS & KEY_L)) || (!(*KEYS & KEY_R)))
			{
				demo=0;
				count=0;
			}

			count++;
			WaitForVsync();

			if (count==200)
			{
				Demo(p1,p2,bg0,bg1);
				count=0;
				DrawTitle();
				play_sfx(&urquan_ditty,1);
				for (int i=0;i<100;i++)
					WaitForVsync();
			}
		}
		while (demo>0);
		for (int i=0;i<100;i++)
			WaitForVsync();

		menu(p1,p2);
		p1->ai=PLAYER1;
		p2->ai=aiskill;

		planet++;

		sprites[30].attribute0 = COLOR_256 | SQUARE | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 20;	//setup sprite info, 256 colour, shape and y-coord
		sprites[30].attribute1 = SIZE_8 | ROTDATA(30) | 50;
		MoveSprite(&sprites[30],240,160);
		//set to point to 0.
		WaitForVsync();
		CopyOAM();


		//p2->ai=STANDARD;
		//p1->ai=STANDARD;
	//	p1->ai=AWESOME;
		//p2->ai=AWESOME;
		//p2->ai=DISABLED;

		//p2->ai=PLAYER2;

		ChooseShips(p1,plist1);
		ChooseShips(p2,plist2);



		//int p1c=0;
		//int p2c=0;
		s16 nextp1=ChooseNextShip(p1,plist1);
		p1->ship=plist1[nextp1].ship;
		s16 nextp2=ChooseNextShip(p2,plist2);

		p2->ship=plist2[nextp2].ship;

		SetNew(p1);
		SetNew(p2);

		SetShip(p1);

		SetShip(p2);
/*  DISPLAYS WINNER!
		plist1[0].active==DESTROYED;
			plist1[2].active=DESTROYED;
			plist1[3].active=DESTROYED;
			plist1[6].active=DESTROYED;
			plist1[7].active=DESTROYED;
			plist1[9].active=DESTROYED;
			plist1[10].active=DESTROYED;
			plist1[11].active=DESTROYED;
			plist1[13].active=DESTROYED;

		for (int pp=0;pp<14;pp++)
			plist2[pp].active=DESTROYED;

		DrawWinner(plist1,plist2);
		while(1);
*/

		/*
		LoadPal();
		//LoadShip(p1);
		//LoadShip(p2);
		p1->loadfunc(p1->SpriteStart);
		p2->loadfunc(p2->SpriteStart);
		LoadExp(OAMFireSprite1,FireSprite1);
		LoadTrail(OAMTrailSprite);
		LoadPlanet(OAMPlanetSprite);
		*/

		do
		{
       		Melee(p1,p2,bg0,bg1);
       		if (p1->crew<1&&p1->EndGame==0)
       		{
				plist1[nextp1].active=DESTROYED;
				nextp1 = ChooseNextShip(p1,plist1);

				if (nextp1!=-1)
				{	p1->ship=plist1[nextp1].ship;
					SetNew(p1);
       				SetShip(p1);

       				//LoadShip(p1);
       				p1->loadfunc(p1->SpriteStart);


				}
			}

			if (p2->crew<1&&p2->EndGame==0)
			{
				plist2[nextp2].active=DESTROYED;
				nextp2 = ChooseNextShip(p2,plist2);

				if (nextp2!=-1)
				{	p2->ship=plist2[nextp2].ship;
					SetNew(p2);
					SetShip(p2);

				    //LoadShip(p2);
				    p2->loadfunc(p2->SpriteStart);
				}
			}

		}
		while (nextp1!=-1&&nextp2!=-1);
		//game over!!!
		DrawWinner(plist1,plist2);
		/*
		if (nextp1==-1)
			DrawWinner(p2,plist2);
		else
			DrawWinner(p1,plist1);
		*/
	}//end loop - got back to titles;
	while(1);



}



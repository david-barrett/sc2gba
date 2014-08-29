#include "gba.h"	//GBA register definitions

#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"
#include "bg.h"




//typedef void (*fnptr)(void);
//#define REG_INTMAIN *(fnptr*)(0x03fffffc)

#include<math.h>
#include<stdlib.h>


extern u16* OAM;
extern pOAMEntry sprites;
extern pRotData rotData;
extern int* KEYS;


extern s8 pilot;


void ChooseShips(pPlayer pl, pPlrList list)
{
	//chooses team of 14 ships
/*
	//cheating
	for (int i=0;i<14;i++)
	{
		if  (i%4==0)
			list[i].ship=AVENGER;
		if  (i%3==0)
			list[i].ship=TERMINATOR;
		else if  (i%2==0)
			list[i].ship=DREADNAUGHT;
		else
			list[i].ship=FURY;
		list[i].active=1;
	}
*/
	int num=6;
	for (int i=0;i<num;i++)
		list[i].active=ACTIVE;
	for (int i=num;i<14;i++)
		list[i].active=EMPTY;
	list[0].ship=FURY;
	list[1].ship=TERMINATOR;
	list[2].ship=DREADNAUGHT;
	list[3].ship=AVENGER;
	list[4].ship=INTRUDER;
	list[5].ship=PENETRATOR;


}

int ChooseNextShipRand(pPlrList list)
{
	int ret=1;

	//should be 13 but 5 for now
	int choose=ran(0,5);
	do
	{
		if (list[choose].active==ACTIVE)
				ret=0;
		else
		{
			choose++;
			if (choose==14)
				choose=0;
		}

	}
	while (ret);

	return choose;

}

int score(int ship)
{
	if (ship==FURY)
		return 20;

	if (ship==DREADNAUGHT)
		return 30;

	if (ship==TERMINATOR)
		return 23;

	if (ship==AVENGER)
		return 10;

	if (ship==PENETRATOR)
		return 13 ;

	if (ship==INTRUDER)
		return 12;

	return 0;
}

int DrawRemainingShips(pPlayer pl,pPlrList list,int sx,int sy)
{
	int i;
	int found=0;
	InitializeSprites();
	int score=0;

	for (i=0;i<5;i++)
	{
		sprites[98+i].attribute0 = COLOR_256 | SQUARE | sy;
	   	sprites[98+i].attribute1 = SIZE_32 | (i*34)+sx;
	   	sprites[103+i].attribute0 = COLOR_256 | SQUARE | 34+sy;
	   	sprites[103+i].attribute1 = SIZE_32 | (i*34)+sx;
	   	sprites[108+i].attribute0 = COLOR_256 | SQUARE | 68+sy;
	   	sprites[108+i].attribute1 = SIZE_32 | (i*34)+sx;

	   	sprites[42+i].attribute0 = COLOR_256 | SQUARE | sy;
		sprites[42+i].attribute1 = SIZE_32 | (i*34)+sx;
		sprites[42+i].attribute2 = SpriteAllShips+864 | PRIORITY(1);
		sprites[47+i].attribute0 = COLOR_256 | SQUARE | 34+sy;
		sprites[47+i].attribute1 = SIZE_32 | (i*34)+sx;
		sprites[47+i].attribute2 = SpriteAllShips+864 | PRIORITY(1);
		sprites[52+i].attribute0 = COLOR_256 | SQUARE | 68+sy;
	   	sprites[52+i].attribute1 = SIZE_32 | (i*34)+sx;
	   	sprites[52+i].attribute2 = SpriteAllShips+864 | PRIORITY(1);

	   	sprites[113+i].attribute0 = COLOR_256 | SQUARE | sy;
		sprites[113+i].attribute1 = SIZE_32 | (i*34)+sx;
		sprites[113+i].attribute2 = SpriteAllShips+896 | PRIORITY(1);
		sprites[118+i].attribute0 = COLOR_256 | SQUARE | 34+sy;
		sprites[118+i].attribute1 = SIZE_32 | (i*34)+sx;
		sprites[118+i].attribute2 = SpriteAllShips+896 | PRIORITY(1);
		sprites[123+i].attribute0 = COLOR_256 | SQUARE | 68+sy;
		sprites[123+i].attribute1 = SIZE_32 | (i*34)+sx;
	   	sprites[123+i].attribute2 = SpriteAllShips+896 | PRIORITY(1);


	}
	MoveSprite(&sprites[56],240,160);

	for (i =0;i<14;i++)
	{	if (list[i].active==ACTIVE||list[i].active==DESTROYED)
		{
			sprites[98+i].attribute2 = SpriteAllShips+(list[i].ship*32) | PRIORITY(3);


			if (list[i].active==ACTIVE)
			{
				found=1;
				score+=list[i].ship;
				MoveSprite(&sprites[42+i],240,160);
			}
		}
		else if (list[i].active==EMPTY)
		{
			sprites[98+i].attribute2 = SpriteAllShips+896 | PRIORITY(1);
			MoveSprite(&sprites[42+i],240,160);
		}
	}

	sprites[112].attribute2 = SpriteAllShips+832 | PRIORITY(1);
	if (found==0)
		return -1;

	LoadPal();
	LoadAllShips(SpriteAllShips*16);
	LoadLetters(SpriteLettersStart);


    s16 text=sx-10;//20;
    DrawLetter('p',31,text+10,15);
    DrawLetter('l',32,text+20,15);
    DrawLetter('a',33,text+30,15);
    DrawLetter('y',34,text+40,15);
    DrawLetter('e',35,text+50,15);
    DrawLetter('r',36,text+60,15);

    if (pl->plr==1)
    {
		DrawLetter('o',37,text+80,15);
	    DrawLetter('n',38,text+90,15);
    	DrawLetter('e',39,text+100,15);
	}
	else
	{
		DrawLetter('t',37,text+80,15);
	    DrawLetter('w',38,text+90,15);
    	DrawLetter('o',39,text+100,15);
	}
	return score; // maybe return value of remaining ships!!
}

int DrawWinner(pPlayer pl, pPlrList list)
{
	int sx=30,sy=30;
	int score=DrawRemainingShips(pl,list,sx,sy);
	DrawLetter('w',60,sx+110,15);
	DrawLetter('i',61,sx+120,15);
	DrawLetter('n',62,sx+130,15);
	DrawLetter('s',63,sx+140,15);

	DrawLargeNumber(score,64,sx,140,3);

	//DrawLetter('o',64,sx,140);
	//DrawLetter('o',65,sx+10,140);
	//DrawLetter('o',66,sx+20,140);

	DrawLetter('p',67,sx+40,140);
	DrawLetter('o',68,sx+50,140);
	DrawLetter('i',69,sx+60,140);
	DrawLetter('n',70,sx+70,140);
	DrawLetter('t',71,sx+80,140);
	if (score!=1)
		DrawLetter('s',72,sx+90,140);

	WaitForVsync();
	CopyOAM();
	while (*KEYS & KEY_START);
	InitializeSprites();
}

int ChooseNextShip(pPlayer pl, pPlrList list)
{
	//choose next ship
	//returns ship id choosen
	//or -1 if no more ships

	int i;
	int found=1,count=0;
	int selected=-1;
	int sx=30,sy=30;
	int x=sx+4*34,y=sy+2*34;

	if (DrawRemainingShips(pl,list,sx,sy)==-1)
	{
		InitializeSprites();
		WaitForVsync();
		CopyOAM();
		return -1;
	}

	 DrawLetter('s',60,sx,140);
	 DrawLetter('e',61,sx+10,140);
	 DrawLetter('l',62,sx+20,140);
	 DrawLetter('e',63,sx+30,140);
	 DrawLetter('c',64,sx+40,140);
	 DrawLetter('t',65,sx+50,140);
	 DrawLetter('n',66,sx+70,140);
	 DrawLetter('e',67,sx+80,140);
	 DrawLetter('x',68,sx+90,140);
	 DrawLetter('t',69,sx+100,140);
	 DrawLetter('s',70,sx+120,140);
	 DrawLetter('h',71,sx+130,140);
	 DrawLetter('i',72,sx+140,140);
	 DrawLetter('p',73,sx+150,140);


	sprites[57].attribute2 = SpriteAllShips+800 | PRIORITY(0);
	sprites[57].attribute0 = COLOR_256 | SQUARE | MODE_TRANSPARENT |y;
    sprites[57].attribute1 = SIZE_32 | x;

    WaitForVsync();
	CopyOAM();



	// TAKEN OUT TEMPORARILY SO I CAN CHOOSE OPP
#ifndef DEBUG
	if (pl->ai>PLAYER2)
	{

		for(i=0;i<500;i++)
			WaitForVsync();
		InitializeSprites();
		WaitForVsync();
		CopyOAM();
		return ChooseNextShipRand(list);
	}
#endif


   	do
	{
    	//WaitForVsync();
		//CopyOAM();
		if(!(*KEYS & KEY_UP)&&y>sy)                   //if the UP key is pressed
		{
			count=0;
			y=y-34;
			MoveSprite(&sprites[57],x,y);
			WaitForVsync();
			CopyOAM();
			for (int i=0;i<100;i++)
					WaitForVsync();
		}
		if(!(*KEYS & KEY_DOWN)&&y<68+sy)                 //if the DOWN key is pressed
		{
			count==0;
			y=y+34;
			MoveSprite(&sprites[57],x,y);
			WaitForVsync();
			CopyOAM();
			for (int i=0;i<100;i++)
				WaitForVsync();
		}
		if(!(*KEYS & KEY_LEFT)&&x>sx)                 //if the LEFT key is pressed
		{
			count=0;
		    x=x-34;
			MoveSprite(&sprites[57],x,y);
			WaitForVsync();
			CopyOAM();
			for (int i=0;i<100;i++)
				WaitForVsync();
		}
		if(!(*KEYS & KEY_RIGHT)&&x<136+sx)                //if the RIGHT key is pressed
		{
			count=0;
			x=x+34;
			MoveSprite(&sprites[57],x,y);
			WaitForVsync();
			CopyOAM();
			for (int i=0;i<100;i++)
				WaitForVsync();
		}
		if(!(*KEYS & KEY_A)||!(*KEYS & KEY_B))                	//if the A or B key is pressed
		{
			count=0;
				//found=0;
				//selected = ?
			if (y==68+sy)
			{
				if (x==136+sx)
				{
					found=0;
					selected=ChooseNextShipRand(list);
				}
				else
				{
					int t=10+((x-sx)/34);
					if (list[t].active==1)
					{
						found=0;
						selected=t;
					}
				}
			}
			else if (y==34+sx)
			{
				int t=5+((x-sx)/34);

				if (list[t].active==1)
				{
					found=0;
					selected=t;
				}
			}
			else
			{
				int t=((x-sx)/34);

				if (list[t].active==1)
				{
						found=0;
						selected=t;
				}
			}

		}
		WaitForVsync();
		CopyOAM();
		count++;

		if (count==500)
		{
			found=0;
			selected=ChooseNextShipRand(list);
		}

	}
	while (found);
	InitializeSprites();
	WaitForVsync();
	CopyOAM();
	return selected;
}
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
extern u8 *pSaveMemory;

void DisplayShipDetail(int ship,int x,int y,int s);
int SelectNextShip(s32 x,s32 y,s32 s,s32 d=GUARDIAN);

int Available(s16 selected)
{
	if (selected==FURY||selected==DREADNAUGHT||
		selected==AVENGER||selected==TERMINATOR||
		selected==PENETRATOR||selected==INTRUDER||
		selected==DRONE||selected==ELUDER||
		selected==GUARDIAN||selected==PODSHIP||
		selected==CRUISER||selected==SCOUT||
		selected==TRANSFORMER||selected==BROODHOME||
		selected==SKIFF)
		return 1;

	return 0;
}


void ChooseShips(pPlayer pl, pPlrList list)
{
	//chooses team of 14 ships
	for (int i=0;i<14;i++)
		list[i].active=EMPTY;



	s8 o = (pl->plr==1)?4:19;
	s8 mem = pSaveMemory[o];

	s32 totalscore=0;


	if (mem!=0)
	{


		for (int i=0;i<14;i++)
		{
			mem=pSaveMemory[o+i+1];
			if (mem>0)
			{
				list[i].active=ACTIVE;
				list[i].ship=mem-1;
			}

		}
	}

	s16 x=2,y=4,s=13,ax=(34*3)+x,ay=y+78;

	 DrawLetter('p',s,x,y);
    DrawLetter('l',s+1,x+10,y);
    DrawLetter('a',s+2,x+20,y);
    DrawLetter('y',s+3,x+30,y);
    DrawLetter('e',s+4,x+40,y);
    DrawLetter('r',s+5,x+50,y);

    if (pl->plr==1)
    {
		DrawLetter('o',s+6,x+70,y);
	    DrawLetter('n',s+7,x+80,y);
    	DrawLetter('e',s+8,x+90,y);
	}
	else
	{
		DrawLetter('t',s+6,x+70,y);
	    DrawLetter('w',s+7,x+80,y);
    	DrawLetter('o',s+8,x+90,y);
	}

	DrawLetter('s',s+9,x+110,y);
    DrawLetter('e',s+10,x+120,y);
    DrawLetter('l',s+11,x+130,y);
    DrawLetter('e',s+12,x+140,y);
    DrawLetter('c',s+13,x+150,y);
	DrawLetter('t',s+14,x+160,y);
    DrawLetter('s',s+15,x+180,y);
    DrawLetter('h',s+16,x+190,y);
    DrawLetter('i',s+17,x+200,y);
    DrawLetter('p',s+18,x+210,y);
	DrawLetter('s',s+19,x+220,y);

	for (int i=0;i<7;i++)
	{


	   	sprites[20+i+s].attribute0 = COLOR_256 | SQUARE | y+10;
		sprites[20+i+s].attribute1 = SIZE_32 | (i*34)+x;
		sprites[20+i+s].attribute2 = SpriteAllShips+896 | PRIORITY(1);
		sprites[27+i+s].attribute0 = COLOR_256 | SQUARE | y+44;
		sprites[27+i+s].attribute1 = SIZE_32 | (i*34)+x;
		sprites[27+i+s].attribute2 = SpriteAllShips+896 | PRIORITY(1);


		sprites[34+i+s].attribute0 = COLOR_256 | SQUARE | y+10;
	   	sprites[34+i+s].attribute1 = SIZE_32 | (i*34)+x;
		if (list[i].active!=EMPTY)
		{
			totalscore+=score(list[i].ship);
			sprites[34+i+s].attribute2 = SpriteAllShips+(list[i].ship*32) | PRIORITY(3);
		}
		else
			sprites[34+i+s].attribute2 = SpriteAllShips+896 | PRIORITY(3);

	   	sprites[41+i+s].attribute0 = COLOR_256 | SQUARE | y+44;
	   	sprites[41+i+s].attribute1 = SIZE_32 | (i*34)+x;
		if (list[i+7].active!=EMPTY)
		{
			totalscore+=score(list[i+7].ship);
			sprites[41+i+s].attribute2 = SpriteAllShips+(list[i+7].ship*32) | PRIORITY(3);
		}
		else
			sprites[41+i+s].attribute2 = SpriteAllShips+896 | PRIORITY(3);
	}
	LoadAllShips(SpriteAllShips*16);
	LoadBattle();
	LoadPal();

	//battle
	sprites[48+s].attribute0 = COLOR_256 | SQUARE | y+78;
	sprites[48+s].attribute1 = SIZE_32 | (3*34)+x;
	sprites[48+s].attribute2 = SpriteAllShips+832 | PRIORITY(2);

	//box
	sprites[49+s].attribute0 = COLOR_256 | SQUARE | ay;
	sprites[49+s].attribute1 = SIZE_32 | ax;
	sprites[49+s].attribute2 = SpriteAllShips+800 | PRIORITY(0);

	//battle outline
	sprites[50+s].attribute0 = COLOR_256 | SQUARE | y+78;
	sprites[50+s].attribute1 = SIZE_32 | (3*34)+x;
	sprites[50+s].attribute2 = SpriteAllShips+896 | PRIORITY(1);

	DrawLargeNumber(totalscore,s+51,x+208,y+ 78,3);




	WaitForVsync();
	CopyOAM();
	int count=0,notfinished=1,ship,tmp;

   	do
	{

		if(!(*KEYS & KEY_UP)&&ay>y+10)                   //if the UP key is pressed
		{
			count=0;
			ay=ay-34;
			MoveSprite(&sprites[49+s],ax,ay);

		}
		if(!(*KEYS & KEY_DOWN)&&ay<78+y)                 //if the DOWN key is pressed
		{
			count=0;
			ay=ay+34;
			if (ay==78+y)
				ax=(3*34+x);
			MoveSprite(&sprites[49+s],ax,ay);


		}
		if(!(*KEYS & KEY_LEFT)&&ax>x&&ay<=y+44)                 //if the LEFT key is pressed
		{
			count=0;
		    ax=ax-34;
			MoveSprite(&sprites[49+s],ax,ay);

		}
		if(!(*KEYS & KEY_RIGHT)&&ax<x+(6*34)&&ay<=y+44)                //if the RIGHT key is pressed
		{
			count=0;
			ax=ax+34;
			MoveSprite(&sprites[49+s],ax,ay);

		}
		if(!(*KEYS & KEY_A))
		{
			count=0;
			if (ay==y+78)
			{
				if (totalscore>0)
					notfinished=0;
			}
			else
			{
				if (ay==y+10)
					tmp=(ax-x)/34;
				else
					tmp=7+((ax-x)/34);

				if (list[tmp].active==ACTIVE)
					totalscore-=score(list[tmp].ship);

				list[tmp].active=ACTIVE;
				MoveSprite(&sprites[48+s],240,160);
				ship=SelectNextShip(x+(3*34),y+78,s+54,list[tmp].ship);
				MoveSprite(&sprites[48+s],x+(3*34),y+78);
				sprites[34+tmp+s].attribute2 = SpriteAllShips+(ship*32) | PRIORITY(3);
				list[tmp].ship=ship;
				totalscore+=score(list[tmp].ship);
				DrawLargeNumber(totalscore,s+51,x+208,y+ 78,3);

				//move right
				ax=ax+34;

				if (ax>x+(6*34))
				{
					ay+=34;
					ax=x;			;
					if (ay==y+78)
						ax=x+(3*34);

				}


			}//end else select

		}//end if a


		if(!(*KEYS & KEY_B))
		{
			count=0;

			if (ay==y+78)
			{
				if (totalscore>0)
					notfinished=0;
			}
			else
			{
				if (ay==y+10)
					tmp=(ax-x)/34;
				else
					tmp=7+((ax-x)/34);

				if (list[tmp].active==ACTIVE)
					totalscore-=score(list[tmp].ship);

				list[tmp].active=EMPTY;
				sprites[34+tmp+s].attribute2 = SpriteAllShips+896 | PRIORITY(3);
				DrawLargeNumber(totalscore,s+51,x+208,y+ 78,3);

				//move left
				ax=ax-34;

				if (ax<x)
				{
					if (ay==y+44)
						ax=(6*34)+x;
					else
						ax=x;
					ay=y+10;
				}
			}//end if not done


		}//end if B



		if (count==0)
		{
			MoveSprite(&sprites[49+s],ax,ay);
			if (ay==78+y)
				ship=-2;
			else
			{
				if (ay==y+10)
					tmp=(ax-x)/34;
				else
					tmp=7+((ax-x)/34);

				if (list[tmp].active==EMPTY)
					ship=-1;
				else
					ship=list[tmp].ship;

			}
			DisplayShipDetail(ship,x,y+112,54+s);
    		WaitForVsync();
			CopyOAM();
			for (int i=0;i<100;i++)
				WaitForVsync();
		}
		WaitForVsync();
		CopyOAM();
		count++;

		if (count==50000)
		{
			notfinished=0;
		}

	}
	while (notfinished);
	InitializeSprites();
	WaitForVsync();
	CopyOAM();



	//save to memory

	pSaveMemory[o]=1;

	for (int i=0;i<14;i++)
	{
		if (list[i].active==ACTIVE)
		{
			pSaveMemory[o+i+1]=list[i].ship+1;
		}
		else
			pSaveMemory[o+i+1]=0;

	}



}

int SelectNextShip(s32 x,s32 y,s32 s,s32 d)
{


	s32 selected=d;
	int i;
	if (selected<GUARDIAN||selected>STINGER)
		selected=GUARDIAN;

	//cross
	 sprites[0].attribute0 = COLOR_256 | SQUARE | 160;
	 sprites[0].attribute1 = SIZE_32 | 240;
	 sprites[0].attribute2 = SpriteAllShips+864 | PRIORITY(0);

	 //square
	 sprites[1].attribute0 = COLOR_256 | SQUARE | y;
	 sprites[1].attribute1 = SIZE_32 | x;
	 sprites[1].attribute2 = SpriteAllShips+800 | PRIORITY(0);

	 //selected
	 sprites[2].attribute0 = COLOR_256 | SQUARE | y;
	 sprites[2].attribute1 = SIZE_32 | x;
	 sprites[2].attribute2 = SpriteAllShips+(selected*32)| PRIORITY(1);

	 //left
	 i=selected-1;
	 if (i<GUARDIAN)
		 i+=STINGER;
	 sprites[3].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[3].attribute1 = SIZE_32 |  ROTDATA(3) | x-28;
	 sprites[3].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(3, 0, 384, 384);

	 i--;
	 if (i<GUARDIAN)
		 i=STINGER;
	 sprites[4].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[4].attribute1 = SIZE_32 |  ROTDATA(4) | x-47;
	 sprites[4].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(4, 0, 512, 512);

	 i--;
	 if (i<GUARDIAN)
		 i=STINGER;
	 sprites[5].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[5].attribute1 = SIZE_32 |  ROTDATA(5) | x-62;
	 sprites[5].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(5, 0, 768, 768);

	  i--;
	 if (i<GUARDIAN)
		 i=STINGER;
	 sprites[6].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[6].attribute1 = SIZE_32 |  ROTDATA(6) | x-72;
	 sprites[6].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(6, 0, 1024, 1024);

	 i--;
	 if (i<GUARDIAN)
		 i=STINGER;
	 sprites[7].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[7].attribute1 = SIZE_32 |  ROTDATA(7) | x-80;
	 sprites[7].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(7, 0, 1536, 1536);




	 //right
	 i=selected+1;
	 if (i<STINGER)
		 i+=GUARDIAN;
	 sprites[8].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[8].attribute1 = SIZE_32 |  ROTDATA(8) | x+27;
	 sprites[8].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(8, 0, 384, 384);

	 i++;
	 if (i<STINGER)
		 i+=GUARDIAN;
	 sprites[9].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[9].attribute1 = SIZE_32 |  ROTDATA(9) | x+47;
	 sprites[9].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(9, 0, 512, 512);

	 i++;
	 if (i<STINGER)
		 i+=GUARDIAN;
	 sprites[10].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[10].attribute1 = SIZE_32 |  ROTDATA(10) | x+61;//19
	 sprites[10].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(10, 0, 768, 768);

	  i++;
	 if (i<STINGER)
		 i+=GUARDIAN;
	 sprites[11].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[11].attribute1 = SIZE_32 |  ROTDATA(11) | x+72;//15
	 sprites[11].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(11, 0, 1024, 1024);

	 i++;
	 if (i<STINGER)
		 i+=GUARDIAN;
	 sprites[12].attribute0 = COLOR_256 | SQUARE| ROTATION_FLAG | y;
	 sprites[12].attribute1 = SIZE_32 |  ROTDATA(12) | x+79;//10
	 sprites[12].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
	 RotateSprite(12, 0, 1536, 1536);





	WaitForVsync();
	CopyOAM();
	for (int i=0;i<100;i++)
			WaitForVsync();
	s32 notfinished=1,count=0;
	do
	{
		if(!(*KEYS & KEY_LEFT))                 //if the LEFT key is pressed
		{
		   count=0;
		   selected--;
		   if (selected<GUARDIAN)
				selected=STINGER;
		}
		if(!(*KEYS & KEY_RIGHT))                //if the RIGHT key is pressed
		{
			count=0;
			selected++;
			if (selected>STINGER)
				selected=GUARDIAN;
		}

		if(!(*KEYS & KEY_A)||!(*KEYS & KEY_B))                	//if the A or B key is pressed
		{
			count=0;

			if (Available(selected))
				notfinished=0;
		}


		if (count==0)
		{
			sprites[2].attribute2 = SpriteAllShips+(selected*32)| PRIORITY(1);
			DisplayShipDetail(selected,x-(3*34),y+34,s);

			i=selected;


		for(int j=3;j<8;j++)
		{
				i--;
			if (i<GUARDIAN)
				 i=STINGER;
			sprites[j].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
		}
		i=selected;

		for(int j=3;j<8;j++)
		{
			i++;
			if (i>STINGER)
				 i=GUARDIAN;
			sprites[j+5].attribute2 = SpriteAllShips+(i*32)| PRIORITY(1);
		}

		if (Available(selected))
			MoveSprite(&sprites[0],240,160);
		else
			MoveSprite(&sprites[0],x,y);








			WaitForVsync();
			CopyOAM();
			for (int i=0;i<100;i++)
				WaitForVsync();
		}
		count++;
	}


	 while(notfinished);

	 for ( i=0;i<13;i++)
		MoveSprite(&sprites[i],240,160);

	 for ( i=s;i<128;i++);
		MoveSprite(&sprites[i],240,160);





	return selected;

}

int ChooseNextShipRand(pPlrList list)
{
	int ret=1;

	int choose=ran(0,13);
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

void DisplayShipDetail(int ship,int x,int y,int s)
{
	s32 st;
	for (int i=s;i<128;i++)
		MoveSprite(&sprites[i],240,160);
	if (ship==-2)
	{
		DrawLetter('d',s,x+98,y);
		DrawLetter('o',s+1,x+108,y);
		DrawLetter('n',s+2,x+118,y);
		DrawLetter('e',s+3,x+128,y);

	}
	else if (ship>=0)
	{
		DrawLetter('c',s,x+83,y+10);
		DrawLetter('r',s+1,x+93,y+10);
		DrawLetter('e',s+2,x+103,y+10);
		DrawLetter('w',s+3,x+113,y+10);
		DrawLetter('b',s+4,x+83,y+20);
		DrawLetter('a',s+5,x+93,y+20);
		DrawLetter('t',s+6,x+103,y+20);
		DrawLetter('t',s+7,x+113,y+20);
		DrawLetter('c',s+8,x+83,y+30);
		DrawLetter('o',s+9,x+93,y+30);
		DrawLetter('s',s+10,x+103,y+30);
		DrawLetter('t',s+11,x+113,y+30);
		DrawLargeNumber(score(ship),s+12,x+133,y+30,2);
		if (ship==FURY)
		{
			st=68;
			//10 chars
			DrawLargeNumber(8,s+14,x+133,y+10,2);
			DrawLargeNumber(12,s+16,x+133,y+20,2);
			DrawLetter('p',s+18,x+st,y);
			DrawLetter('k',s+19,x+st+10,y);
			DrawLetter('u',s+20,x+st+20,y);
			DrawLetter('n',s+21,x+st+30,y);
			DrawLetter('k',s+22,x+st+40,y);
			DrawLetter('f',s+23,x+st+60,y);
			DrawLetter('u',s+24,x+st+70,y);
			DrawLetter('r',s+25,x+st+80,y);
			DrawLetter('y',s+26,x+st+90,y);

		}
		else if (ship==DREADNAUGHT)
		{
			st=28;
			//19 chars
			DrawLargeNumber(42,s+14,x+133,y+10,2);
			DrawLargeNumber(42,s+16,x+133,y+20,2);
			DrawLetter('u',s+18,x+st,y);
			DrawLetter('r',s+19,x+st+10,y);
			DrawLetter('q',s+20,x+st+20,y);
			DrawLetter('u',s+21,x+st+30,y);
			DrawLetter('a',s+22,x+st+40,y);
			DrawLetter('n',s+23,x+st+50,y);
			DrawLetter('d',s+24,x+70+st,y);
			DrawLetter('r',s+25,x+80+st,y);
			DrawLetter('e',s+26,x+90+st,y);
			DrawLetter('a',s+27,x+100+st,y);
			DrawLetter('d',s+28,x+110+st,y);
			DrawLetter('n',s+29,x+120+st,y);
			DrawLetter('a',s+30,x+130+st,y);
			DrawLetter('u',s+31,x+140+st,y);
			DrawLetter('g',s+32,x+150+st,y);
			DrawLetter('h',s+33,x+160+st,y);
			DrawLetter('t',s+34,x+170+st,y);

		}
		else if (ship==TERMINATOR)
		{
			st=38;
			//17 CHARS
			DrawLargeNumber(20,s+14,x+133,y+10,2);
			DrawLargeNumber(10,s+16,x+133,y+20,2);
			DrawLetter('y',s+18,x+st,y);
			DrawLetter('e',s+19,x+10+st,y);
			DrawLetter('h',s+20,x+20+st,y);
			DrawLetter('a',s+21,x+30+st,y);
			DrawLetter('t',s+22,x+40+st,y);
			DrawLetter('t',s+23,x+60+st,y);
			DrawLetter('e',s+24,x+70+st,y);
			DrawLetter('r',s+25,x+80+st,y);
			DrawLetter('m',s+26,x+90+st,y);
			DrawLetter('i',s+27,x+100+st,y);
			DrawLetter('n',s+28,x+110+st,y);
			DrawLetter('a',s+29,x+120+st,y);
			DrawLetter('t',s+30,x+130+st,y);
			DrawLetter('o',s+31,x+140+st,y);
			DrawLetter('r',s+32,x+150+st,y);


		}
		else if (ship==AVENGER)
		{
			st=43;
			//16 chars
			DrawLargeNumber(22,s+14,x+133,y+10,2);
			DrawLargeNumber(16,s+16,x+133,y+20,2);
			DrawLetter('i',s+18,x+st,y);
			DrawLetter('l',s+19,x+10+st,y);
			DrawLetter('w',s+20,x+20+st,y);
			DrawLetter('r',s+21,x+30+st,y);
			DrawLetter('a',s+22,x+40+st,y);
			DrawLetter('t',s+23,x+50+st,y);
			DrawLetter('h',s+24,x+60+st,y);
			DrawLetter('a',s+25,x+80+st,y);
			DrawLetter('v',s+26,x+90+st,y);
			DrawLetter('e',s+27,x+100+st,y);
			DrawLetter('n',s+28,x+110+st,y);
			DrawLetter('g',s+29,x+120+st,y);
			DrawLetter('e',s+30,x+130+st,y);
			DrawLetter('r',s+31,x+140+st,y);

		}
		else if (ship==INTRUDER)
		{
			st=58;
			//11 chars
			DrawLargeNumber(20,s+14,x+133,y+10,2);
			DrawLargeNumber(40,s+16,x+133,y+20,2);
			DrawLetter('v',s+18,x+st,y);
			DrawLetter('u',s+19,x+10+st,y);
			DrawLetter('x',s+20,x+20+st,y);
			DrawLetter('i',s+21,x+40+st,y);
			DrawLetter('n',s+22,x+50+st,y);
			DrawLetter('t',s+23,x+60+st,y);
			DrawLetter('r',s+24,x+70+st,y);
			DrawLetter('u',s+25,x+80+st,y);
			DrawLetter('d',s+26,x+90+st,y);
			DrawLetter('e',s+27,x+100+st,y);
			DrawLetter('r',s+28,x+110+st,y);

		}
		else if (ship==PENETRATOR)
		{
			st=33;
			//18 chars
			DrawLargeNumber(12,s+14,x+133,y+10,2);
			DrawLargeNumber(16,s+16,x+133,y+20,2);
			DrawLetter('s',s+18,x+st,y);
			DrawLetter('y',s+19,x+10+st,y);
			DrawLetter('r',s+20,x+20+st,y);
			DrawLetter('e',s+21,x+30+st,y);
			DrawLetter('e',s+22,x+40+st,y);
			DrawLetter('n',s+23,x+50+st,y);
			DrawLetter('p',s+24,x+70+st,y);
			DrawLetter('e',s+25,x+80+st,y);
			DrawLetter('n',s+26,x+90+st,y);
			DrawLetter('e',s+27,x+100+st,y);
			DrawLetter('t',s+28,x+110+st,y);
			DrawLetter('r',s+29,x+120+st,y);
			DrawLetter('a',s+30,x+130+st,y);
			DrawLetter('t',s+31,x+140+st,y);
			DrawLetter('o',s+32,x+150+st,y);
			DrawLetter('r',s+33,x+160+st,y);

			DrawLargeNumber(42,s+35,x+168,y+10,2);
		}

			if (ship==GUARDIAN)
		{
			st=23;
			//22 chars
			DrawLargeNumber(20,s+14,x+133,y+10,2);
			DrawLargeNumber(24,s+16,x+133,y+20,2);
			DrawLetter('a',s+18,x+st,y);
			DrawLetter('n',s+19,x+st+10,y);
			DrawLetter('d',s+20,x+st+20,y);
			DrawLetter('r',s+21,x+st+30,y);
			DrawLetter('o',s+22,x+st+40,y);
			DrawLetter('s',s+23,x+st+50,y);
			DrawLetter('y',s+24,x+60+st,y);
			DrawLetter('n',s+25,x+70+st,y);
			DrawLetter('t',s+26,x+80+st,y);
			DrawLetter('h',s+27,x+90+st,y);
			DrawLetter('g',s+28,x+110+st,y);
			DrawLetter('u',s+29,x+120+st,y);
			DrawLetter('a',s+30,x+130+st,y);
			DrawLetter('r',s+31,x+140+st,y);
			DrawLetter('d',s+32,x+150+st,y);
			DrawLetter('i',s+33,x+160+st,y);
			DrawLetter('a',s+34,x+170+st,y);
			DrawLetter('n',s+35,x+180+st,y);

		}


	if (ship==SKIFF)
		{
			st=13;
			//22 chars
			DrawLargeNumber(6,s+14,x+133,y+10,2);
			DrawLargeNumber(20,s+16,x+133,y+20,2);
			DrawLetter('a',s+18,x+st,y);
			DrawLetter('r',s+19,x+st+10,y);
			DrawLetter('i',s+20,x+st+20,y);
			DrawLetter('l',s+21,x+st+30,y);
			DrawLetter('o',s+22,x+st+40,y);
			DrawLetter('u',s+23,x+st+50,y);
			DrawLetter('l',s+24,x+60+st,y);
			DrawLetter('a',s+25,x+70+st,y);
			DrawLetter('l',s+26,x+80+st,y);
			DrawLetter('l',s+27,x+90+st,y);
			DrawLetter('e',s+28,x+100+st,y);
			DrawLetter('e',s+29,x+110+st,y);
			DrawLetter('l',s+30,x+120+st,y);
			DrawLetter('a',s+31,x+130+st,y);
			DrawLetter('y',s+32,x+140+st,y);
			DrawLetter('s',s+33,x+160+st,y);
			DrawLetter('i',s+34,x+170+st,y);
			DrawLetter('f',s+35,x+180+st,y);
			DrawLetter('f',s+36,x+190+st,y);
			DrawLetter('f',s+37,x+200+st,y);

		}


	if (ship==MARAUDER)
		{
			st=43;
			//15 chars
			DrawLargeNumber(42,s+14,x+133,y+10,2);
			DrawLargeNumber(42,s+16,x+133,y+20,2);
			DrawLetter('k',s+18,x+st,y);
			DrawLetter('o',s+19,x+st+10,y);
			DrawLetter('h',s+20,x+st+20,y);
			DrawLetter('r',s+21,x+st+30,y);
			DrawLetter('a',s+22,x+st+40,y);
			DrawLetter('h',s+23,x+st+50,y);
			DrawLetter('m',s+24,x+70+st,y);
			DrawLetter('a',s+25,x+80+st,y);
			DrawLetter('r',s+26,x+90+st,y);
			DrawLetter('a',s+27,x+100+st,y);
			DrawLetter('u',s+28,x+110+st,y);
			DrawLetter('d',s+29,x+120+st,y);
			DrawLetter('e',s+30,x+130+st,y);
			DrawLetter('r',s+31,x+140+st,y);


		}


	if (ship==BROODHOME)
		{
			st=28;
			//19 chars
			DrawLargeNumber(36,s+14,x+133,y+10,2);
			DrawLargeNumber(30,s+16,x+133,y+20,2);
			DrawLetter('c',s+18,x+st,y);
			DrawLetter('h',s+19,x+st+10,y);
			DrawLetter('e',s+20,x+st+20,y);
			DrawLetter('n',s+21,x+st+30,y);
			DrawLetter('j',s+22,x+st+40,y);
			DrawLetter('e',s+23,x+st+50,y);
			DrawLetter('s',s+24,x+60+st,y);
			DrawLetter('u',s+25,x+70+st,y);
			DrawLetter('b',s+26,x+90+st,y);
			DrawLetter('r',s+27,x+100+st,y);
			DrawLetter('o',s+28,x+110+st,y);
			DrawLetter('o',s+29,x+120+st,y);
			DrawLetter('d',s+30,x+130+st,y);
			DrawLetter('h',s+31,x+140+st,y);
			DrawLetter('o',s+32,x+150+st,y);
			DrawLetter('m',s+33,x+160+st,y);
			DrawLetter('e',s+34,x+170+st,y);

		}


	if (ship==AVATAR)
		{
			st=58;
			//13 chars
			DrawLargeNumber(42,s+14,x+133,y+10,2);
			DrawLargeNumber(42,s+16,x+133,y+20,2);
			DrawLetter('c',s+18,x+st,y);
			DrawLetter('h',s+19,x+st+10,y);
			DrawLetter('m',s+20,x+st+20,y);
			DrawLetter('m',s+21,x+st+30,y);
			DrawLetter('r',s+22,x+st+40,y);
			DrawLetter('a',s+23,x+st+60,y);
			DrawLetter('v',s+24,x+70+st,y);
			DrawLetter('a',s+25,x+80+st,y);
			DrawLetter('t',s+26,x+90+st,y);
			DrawLetter('a',s+27,x+100+st,y);
			DrawLetter('r',s+28,x+110+st,y);


		}


	if (ship==BLADE)
		{
			st=63;
			//12 chars
			DrawLargeNumber(12,s+14,x+133,y+10,2);
			DrawLargeNumber(16,s+16,x+133,y+20,2);
			DrawLetter('s',s+18,x+st,y);
			DrawLetter('u',s+19,x+st+10,y);
			DrawLetter('p',s+20,x+st+20,y);
			DrawLetter('o',s+21,x+st+30,y);
			DrawLetter('x',s+22,x+st+40,y);
			DrawLetter('b',s+23,x+st+60,y);
			DrawLetter('l',s+24,x+70+st,y);
			DrawLetter('a',s+25,x+80+st,y);
			DrawLetter('d',s+26,x+90+st,y);
			DrawLetter('e',s+27,x+100+st,y);


		}


	if (ship==CRUISER)
				{
			st=33;
			//18 chars
			DrawLargeNumber(18,s+14,x+133,y+10,2);
			DrawLargeNumber(18,s+16,x+133,y+20,2);
			DrawLetter('e',s+18,x+st,y);
			DrawLetter('a',s+19,x+st+10,y);
			DrawLetter('r',s+20,x+st+20,y);
			DrawLetter('t',s+21,x+st+30,y);
			DrawLetter('h',s+22,x+st+40,y);
			DrawLetter('l',s+23,x+st+50,y);
			DrawLetter('i',s+24,x+60+st,y);
			DrawLetter('n',s+25,x+70+st,y);
			DrawLetter('g',s+26,x+80+st,y);
			DrawLetter('c',s+27,x+100+st,y);
			DrawLetter('r',s+28,x+110+st,y);
			DrawLetter('u',s+29,x+120+st,y);
			DrawLetter('i',s+30,x+130+st,y);
			DrawLetter('s',s+31,x+140+st,y);
			DrawLetter('e',s+32,x+150+st,y);
			DrawLetter('r',s+33,x+160+st,y);

		}


	if (ship==DRONE)
		{
			st=63;
			//12 chars
			DrawLargeNumber(10,s+14,x+133,y+10,2);
			DrawLargeNumber(30,s+16,x+133,y+20,2);
			DrawLetter('u',s+18,x+st,y);
			DrawLetter('m',s+19,x+st+10,y);
			DrawLetter('g',s+20,x+st+20,y);
			DrawLetter('a',s+21,x+st+30,y);
			DrawLetter('h',s+22,x+st+40,y);
			DrawLetter('d',s+23,x+st+60,y);
			DrawLetter('r',s+24,x+70+st,y);
			DrawLetter('o',s+25,x+80+st,y);
			DrawLetter('n',s+26,x+90+st,y);
			DrawLetter('e',s+27,x+100+st,y);


		}


	if (ship==ELUDER)
		{
			st=53;
			//14 chars
			DrawLargeNumber(30,s+14,x+133,y+10,2);
			DrawLargeNumber(10,s+16,x+133,y+20,2);
			DrawLetter('s',s+18,x+st,y);
			DrawLetter('p',s+19,x+st+10,y);
			DrawLetter('a',s+20,x+st+20,y);
			DrawLetter('t',s+21,x+st+30,y);
			DrawLetter('h',s+22,x+st+40,y);
			DrawLetter('i',s+23,x+st+50,y);
			DrawLetter('e',s+24,x+70+st,y);
			DrawLetter('l',s+25,x+80+st,y);
			DrawLetter('u',s+26,x+90+st,y);
			DrawLetter('d',s+27,x+100+st,y);
			DrawLetter('e',s+28,x+110+st,y);
			DrawLetter('r',s+29,x+120+st,y);


		}


	if (ship==JUGGER)
		{
			st=58;
			//13 chars
			DrawLargeNumber(20,s+14,x+133,y+10,2);
			DrawLargeNumber(20,s+16,x+133,y+20,2);
			DrawLetter('u',s+18,x+st,y);
			DrawLetter('t',s+19,x+st+10,y);
			DrawLetter('w',s+20,x+st+20,y);
			DrawLetter('i',s+21,x+st+30,y);
			DrawLetter('g',s+22,x+st+40,y);
			DrawLetter('j',s+23,x+st+60,y);
			DrawLetter('u',s+24,x+70+st,y);
			DrawLetter('g',s+25,x+80+st,y);
			DrawLetter('g',s+26,x+90+st,y);
			DrawLetter('e',s+27,x+100+st,y);
			DrawLetter('r',s+28,x+110+st,y);

		}


	if (ship==MAULER)
		{
			st=48;
			//15 chars
			DrawLargeNumber(14,s+14,x+133,y+10,2);
			DrawLargeNumber(32,s+16,x+133,y+20,2);
			DrawLetter('d',s+18,x+st,y);
			DrawLetter('r',s+19,x+st+10,y);
			DrawLetter('u',s+20,x+st+20,y);
			DrawLetter('u',s+21,x+st+30,y);
			DrawLetter('d',s+22,x+st+40,y);
			DrawLetter('g',s+23,x+st+50,y);
			DrawLetter('e',s+24,x+60+st,y);
			DrawLetter('m',s+25,x+80+st,y);
			DrawLetter('a',s+26,x+90+st,y);
			DrawLetter('u',s+27,x+100+st,y);
			DrawLetter('l',s+28,x+110+st,y);
			DrawLetter('e',s+29,x+120+st,y);
			DrawLetter('r',s+30,x+130+st,y);

		}


	if (ship==NEMESIS)
		{
			st=63;
			//11 chars
			DrawLargeNumber(16,s+14,x+133,y+10,2);
			DrawLargeNumber(20,s+16,x+133,y+20,2);
			DrawLetter('o',s+18,x+st,y);
			DrawLetter('r',s+19,x+st+10,y);
			DrawLetter('z',s+20,x+st+20,y);
			DrawLetter('n',s+21,x+st+40,y);
			DrawLetter('e',s+22,x+st+50,y);
			DrawLetter('m',s+23,x+st+60,y);
			DrawLetter('e',s+24,x+70+st,y);
			DrawLetter('s',s+25,x+80+st,y);
			DrawLetter('i',s+26,x+90+st,y);
			DrawLetter('s',s+27,x+100+st,y);


		}


	if (ship==PODSHIP)
		{
			st=53;
			//14 chars
			DrawLargeNumber(20,s+14,x+133,y+10,2);
			DrawLargeNumber(40,s+16,x+133,y+20,2);
			DrawLetter('m',s+18,x+st,y);
			DrawLetter('y',s+19,x+st+10,y);
			DrawLetter('c',s+20,x+st+20,y);
			DrawLetter('o',s+21,x+st+30,y);
			DrawLetter('n',s+22,x+st+40,y);
			DrawLetter('p',s+23,x+st+60,y);
			DrawLetter('o',s+24,x+70+st,y);
			DrawLetter('d',s+25,x+80+st,y);
			DrawLetter('s',s+26,x+90+st,y);
			DrawLetter('h',s+27,x+100+st,y);
			DrawLetter('i',s+28,x+110+st,y);
			DrawLetter('p',s+29,x+120+st,y);

		}


	if (ship==PROBE)
		{
			st=43;
			//19 chars
			DrawLargeNumber(12,s+14,x+133,y+10,2);
			DrawLargeNumber(20,s+16,x+133,y+20,2);
			DrawLetter('s',s+18,x+st,y);
			DrawLetter('l',s+19,x+st+10,y);
			DrawLetter('y',s+20,x+st+20,y);
			DrawLetter('l',s+21,x+st+30,y);
			DrawLetter('a',s+22,x+st+40,y);
			DrawLetter('n',s+23,x+st+50,y);
			DrawLetter('d',s+24,x+60+st,y);
			DrawLetter('r',s+25,x+70+st,y);
			DrawLetter('o',s+26,x+80+st,y);
			DrawLetter('p',s+27,x+100+st,y);
			DrawLetter('r',s+28,x+110+st,y);
			DrawLetter('o',s+29,x+120+st,y);
			DrawLetter('b',s+30,x+130+st,y);
			DrawLetter('e',s+31,x+140+st,y);


		}


	if (ship==SCOUT)
		{
			st=48;
			//15 chars
			DrawLargeNumber(6,s+14,x+133,y+10,2);
			DrawLargeNumber(4,s+16,x+133,y+20,2);
			DrawLetter('s',s+18,x+st,y);
			DrawLetter('h',s+19,x+st+10,y);
			DrawLetter('o',s+20,x+st+20,y);
			DrawLetter('f',s+21,x+st+30,y);
			DrawLetter('i',s+22,x+st+40,y);
			DrawLetter('x',s+23,x+st+50,y);
			DrawLetter('t',s+24,x+60+st,y);
			DrawLetter('i',s+25,x+70+st,y);
			DrawLetter('s',s+26,x+90+st,y);
			DrawLetter('c',s+27,x+100+st,y);
			DrawLetter('o',s+28,x+110+st,y);
			DrawLetter('u',s+29,x+120+st,y);
			DrawLetter('t',s+30,x+130+st,y);


		}


	if (ship==STINGER)
		{
			st=23;
			//20 chars
			DrawLargeNumber(10,s+14,x+133,y+10,2);
			DrawLargeNumber(10,s+16,x+133,y+20,2);
			DrawLetter('z',s+18,x+st,y);
			DrawLetter('o',s+19,x+st+10,y);
			DrawLetter('q',s+20,x+st+20,y);
			DrawLetter('f',s+21,x+st+40,y);
			DrawLetter('o',s+22,x+st+50,y);
			DrawLetter('t',s+23,x+st+60,y);
			DrawLetter('p',s+24,x+80+st,y);
			DrawLetter('i',s+25,x+90+st,y);
			DrawLetter('k',s+26,x+100+st,y);
			DrawLetter('s',s+27,x+120+st,y);
			DrawLetter('t',s+28,x+130+st,y);
			DrawLetter('i',s+29,x+140+st,y);
			DrawLetter('n',s+30,x+150+st,y);
			DrawLetter('g',s+31,x+160+st,y);
			DrawLetter('e',s+32,x+170+st,y);
			DrawLetter('r',s+33,x+180+st,y);

		}


	if (ship==TORCH)
		{
			st=43;
			//16 chars
			DrawLargeNumber(8,s+14,x+133,y+10,2);
			DrawLargeNumber(24,s+16,x+133,y+20,2);
			DrawLetter('t',s+18,x+st,y);
			DrawLetter('h',s+19,x+st+10,y);
			DrawLetter('a',s+20,x+st+20,y);
			DrawLetter('r',s+21,x+st+30,y);
			DrawLetter('d',s+22,x+st+40,y);
			DrawLetter('d',s+23,x+st+50,y);
			DrawLetter('a',s+24,x+60+st,y);
			DrawLetter('s',s+25,x+70+st,y);
			DrawLetter('h',s+26,x+80+st,y);
			DrawLetter('t',s+27,x+100+st,y);
			DrawLetter('o',s+28,x+110+st,y);
			DrawLetter('r',s+29,x+120+st,y);
			DrawLetter('c',s+30,x+130+st,y);
			DrawLetter('h',s+31,x+140+st,y);


		}


	if (ship==TRADER)
		{
			st=43;
			//16 chars
			DrawLargeNumber(20,s+14,x+133,y+10,2);
			DrawLargeNumber(42,s+16,x+133,y+20,2);
			DrawLetter('m',s+18,x+st,y);
			DrawLetter('e',s+19,x+st+10,y);
			DrawLetter('l',s+20,x+st+20,y);
			DrawLetter('n',s+21,x+st+30,y);
			DrawLetter('o',s+22,x+st+40,y);
			DrawLetter('r',s+23,x+st+50,y);
			DrawLetter('m',s+24,x+60+st,y);
			DrawLetter('e',s+25,x+70+st,y);
			DrawLetter('t',s+26,x+90+st,y);
			DrawLetter('r',s+27,x+100+st,y);
			DrawLetter('a',s+28,x+110+st,y);
			DrawLetter('d',s+29,x+120+st,y);
			DrawLetter('e',s+30,x+130+st,y);
			DrawLetter('r',s+31,x+140+st,y);


		}


	if (ship==TRANSFORMER)
		{
			st=18;
			//21 chars
			DrawLargeNumber(20,s+14,x+133,y+10,2);
			DrawLargeNumber(10,s+16,x+133,y+20,2);
			DrawLetter('m',s+18,x+st,y);
			DrawLetter('m',s+19,x+st+10,y);
			DrawLetter('r',s+20,x+st+20,y);
			DrawLetter('n',s+21,x+st+30,y);
			DrawLetter('m',s+22,x+st+40,y);
			DrawLetter('h',s+23,x+st+50,y);
			DrawLetter('r',s+24,x+60+st,y);
			DrawLetter('m',s+25,x+70+st,y);
			DrawLetter('t',s+26,x+90+st,y);
			DrawLetter('r',s+27,x+100+st,y);
			DrawLetter('a',s+28,x+110+st,y);
			DrawLetter('n',s+29,x+120+st,y);
			DrawLetter('s',s+30,x+130+st,y);
			DrawLetter('f',s+31,x+140+st,y);
			DrawLetter('o',s+32,x+150+st,y);
			DrawLetter('r',s+33,x+160+st,y);
			DrawLetter('m',s+34,x+170+st,y);
			DrawLetter('e',s+35,x+180+st,y);
			DrawLetter('r',s+36,x+190+st,y);

		}




	}
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

	//d
	if (ship==GUARDIAN)
		return 15;

	if (ship==SKIFF)
		return 16;

	if (ship==MARAUDER)
		return 30;

	if (ship==BROODHOME)
		return 28;

	if (ship==AVATAR)
		return 30 ;

	if (ship==BLADE)
		return 16;

	if (ship==CRUISER)
		return 11;

	if (ship==DRONE)
		return 7;

	if (ship==ELUDER)
		return 18;

	if (ship==JUGGER)
		return 22;

	if (ship==MAULER)
		return 17;

	if (ship==NEMESIS)
		return 23;

	if (ship==PODSHIP)
		return 21;

	if (ship==PROBE)
		return 17;

	if (ship==SCOUT)
		return 5;

	if (ship==STINGER)
		return 6;

	if (ship==TORCH)
		return 10 ;

	if (ship==TRADER)
		return 18;

	if (ship==TRANSFORMER)
		return 19;



	return 0;
}

int DrawRemainingShips(pPlayer pl,pPlrList list,int sx,int sy)
{
	int i;
	int found=0;
	InitializeSprites();
	int tscore=0;

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
				tscore+=score(list[i].ship);
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
	return tscore; // maybe return value of remaining ships!!
}

s16 DrawEndShips(pPlrList list, s8 s,s8 x,s8 y)
{
	s16 totalscore=0;
	for (int i=0;i<7;i++)
	{

		//empty boxes
	   	sprites[i+s].attribute0 = COLOR_256 | SQUARE | y;
		sprites[i+s].attribute1 = SIZE_32 | (i*34)+x;
		sprites[i+s].attribute2 = SpriteAllShips+896 | PRIORITY(1);
		sprites[7+i+s].attribute0 = COLOR_256 | SQUARE | y+34;
		sprites[7+i+s].attribute1 = SIZE_32 | (i*34)+x;
		sprites[7+i+s].attribute2 = SpriteAllShips+896 | PRIORITY(1);

		//crosses
		sprites[28+i+s].attribute0 = COLOR_256 | SQUARE | 160;
		sprites[28+i+s].attribute1 = SIZE_32 | 240;
		sprites[28+i+s].attribute2 = SpriteAllShips+864 | PRIORITY(0);
		sprites[35+i+s].attribute0 = COLOR_256 | SQUARE | 160;
		sprites[35+i+s].attribute1 = SIZE_32 | 240;
		sprites[35+i+s].attribute2 = SpriteAllShips+864 | PRIORITY(0);



		sprites[14+i+s].attribute0 = COLOR_256 | SQUARE | y;
	   	sprites[14+i+s].attribute1 = SIZE_32 | (i*34)+x;
		if (list[i].active!=EMPTY)
		{
			if (list[i].active==ACTIVE)
				totalscore+=score(list[i].ship);
			else
				MoveSprite(&sprites[28+i+s],(i*34)+x,y);
			sprites[14+i+s].attribute2 = SpriteAllShips+(list[i].ship*32) | PRIORITY(3);

		}
		else
			sprites[14+i+s].attribute2 = SpriteAllShips+896 | PRIORITY(3);

	   	sprites[21+i+s].attribute0 = COLOR_256 | SQUARE | y+34;
	   	sprites[21+i+s].attribute1 = SIZE_32 | (i*34)+x;
		if (list[i+7].active!=EMPTY)
		{
			if (list[i+7].active==ACTIVE)
				totalscore+=score(list[i+7].ship);
			else
				MoveSprite(&sprites[35+i+s],(i*34)+x,y+34);
			sprites[21+i+s].attribute2 = SpriteAllShips+(list[i+7].ship*32) | PRIORITY(3);

		}
		else
			sprites[21+i+s].attribute2 = SpriteAllShips+896 | PRIORITY(3);
	}
	return totalscore;
}

int DrawWinner(pPlrList list1, pPlrList list2)
{
	int sx=45,sy=71;
	InitializeSprites();
	int score1=DrawEndShips(list1,0,2,3);
	int score2=DrawEndShips(list2,42,2,92);
	LoadPal();
	LoadAllShips(SpriteAllShips*16);
	LoadLetters(SpriteLettersStart);


	if (score1==score2)
	{
		DrawLetter('d',84,sx+55,sy);
		DrawLetter('r',85,sx+65,sy);
		DrawLetter('a',86,sx+75,sy);
		DrawLetter('w',87,sx+85,sy);

	}
	else
	{
		DrawLetter('p',84,sx,sy);
		DrawLetter('l',85,sx+10,sy);
		DrawLetter('a',86,sx+20,sy);
		DrawLetter('y',87,sx+30,sy);
		DrawLetter('e',88,sx+40,sy);
		DrawLetter('r',89,sx+50,sy);

		if (score1>score2)
		{
		DrawLetter('o',90,sx+70,sy);
		DrawLetter('n',91,sx+80,sy);
		DrawLetter('e',92,sx+90,sy);
		DrawLargeNumber(score1,97,sx+25,sy+10,3);
		}
		else
		{
		DrawLetter('t',90,sx+70,sy);
		DrawLetter('w',91,sx+80,sy);
		DrawLetter('o',92,sx+90,sy);
		DrawLargeNumber(score2,97,sx+25,sy+10,3);
		}

		DrawLetter('w',93,sx+110,sy);
		DrawLetter('i',94,sx+120,sy);
		DrawLetter('n',95,sx+130,sy);
		DrawLetter('s',96,sx+140,sy);

		DrawLetter('p',100,sx+65,sy+10);
		DrawLetter('o',101,sx+75,sy+10);
		DrawLetter('i',102,sx+85,sy+10);
		DrawLetter('n',103,sx+95,sy+10);
		DrawLetter('t',104,sx+105,sy+10);
		DrawLetter('s',105,sx+115,sy+10);



	}

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

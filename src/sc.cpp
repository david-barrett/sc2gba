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
//#include "interrupts.h"


#include "letters.h"

//#include "sc2title.h"
#include "sc2titles.h"

//sound
#include "sfx.h"
#include "urquan_sfx.h"
//#include "pkunk_sfx.h"
//extern /*"C"*/ const PCMSOUND pkunk_stupid;

typedef void (*fnptr)(void);
#define REG_INTMAIN *(fnptr*)(0x03fffffc)

#include<math.h>
#include<stdlib.h>

//create an OAM variable and make it point to the address of OAM
u16* OAM = (u16*)0x7000000;
//create the array of sprites (128 is the maximum)
pOAMEntry sprites;

s8 pilot;

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

void LoadLetters(s16 spriteStart)
{

	s32 OAMStart = spriteStart*16;
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
  	{
       		OAMData[loop] = aData[loop-OAMStart];
       		OAMData[loop+32] = bData[loop-OAMStart];
       		OAMData[loop+64] = cData[loop-OAMStart];
       		OAMData[loop+96] = dData[loop-OAMStart];
       		OAMData[loop+128] = eData[loop-OAMStart];
       		OAMData[loop+160] = fData[loop-OAMStart];
       		OAMData[loop+192] = gData[loop-OAMStart];
			OAMData[loop+224] = hData[loop-OAMStart];
			OAMData[loop+256] = iData[loop-OAMStart];
			OAMData[loop+288] = jData[loop-OAMStart];
			OAMData[loop+320] = kData[loop-OAMStart];
       		OAMData[loop+352] = lData[loop-OAMStart];
       		OAMData[loop+384] = mData[loop-OAMStart];
			OAMData[loop+416] = nData[loop-OAMStart];
			OAMData[loop+448] = oData[loop-OAMStart];
			OAMData[loop+480] = pData[loop-OAMStart];
			OAMData[loop+512] = qData[loop-OAMStart];
       		OAMData[loop+544] = rData[loop-OAMStart];
       		OAMData[loop+576] = sData[loop-OAMStart];
			OAMData[loop+608] = tData[loop-OAMStart];
		    OAMData[loop+640] = uData[loop-OAMStart];
			OAMData[loop+672] = vData[loop-OAMStart];
			OAMData[loop+704] = wData[loop-OAMStart];
       		OAMData[loop+736] = xData[loop-OAMStart];
       		OAMData[loop+768] = yData[loop-OAMStart];
       		OAMData[loop+800] = zData[loop-OAMStart];

       		OAMData[loop+832] = arrowData[loop-OAMStart];

       		OAMData[loop+864] = n0Data[loop-OAMStart];
       		OAMData[loop+896] = n1Data[loop-OAMStart];
       		OAMData[loop+928] = n2Data[loop-OAMStart];
       		OAMData[loop+960] = n3Data[loop-OAMStart];
       		OAMData[loop+992] = n4Data[loop-OAMStart];
       		OAMData[loop+1024] = n5Data[loop-OAMStart];
       		OAMData[loop+1056] = n6Data[loop-OAMStart];
       		OAMData[loop+1088] = n7Data[loop-OAMStart];
       		OAMData[loop+1120] = n8Data[loop-OAMStart];
       		OAMData[loop+1152] = n9Data[loop-OAMStart];



   	}
}

void printLetter(char letter,s16 sprite,  s16 x, s16 y)
{
//	print("letter p");
//	print((int)'p');
	//a=97 z=122 p = 112
	int off=(int)letter-97;
	sprites[sprite].attribute0 = COLOR_256 | SQUARE | y;
	sprites[sprite].attribute1 = SIZE_8 | x;
	sprites[sprite].attribute2 = SpriteLettersStart+off*2 | PRIORITY(1);
}

void printNumber(int num,s16 sprite,  s16 x, s16 y)
{
	if (num>9)
		num=0;
	int off=(num+27)*2;
	sprites[sprite].attribute0 = COLOR_256 | SQUARE | y;
	sprites[sprite].attribute1 = SIZE_8 | x;
	sprites[sprite].attribute2 = SpriteLettersStart+off | PRIORITY(1);

}

void printLargeNumber(int num,s16 sprite, s16 x, s16 y,s16 digits)
{
	print("number=");
	print(num);
	while(digits>1)
	{
		int off=(int)pow(double(10),double(digits-1));//(10^(digits-1));
		print("off =");
		print(off);
		if (num>off)
		{
			int tmp=num/off;
			print("tmp=");
			print(tmp);
			print("tmp*off");
			print(tmp*off);
			printNumber(tmp,sprite,x,y);
			num=num-(tmp*off);
		}
		else
			printNumber(0,sprite,x,y);
		x+=10;
		sprite++;
		digits--;
		print("new num=");
		print(num);

	}
	//1 digit number
	print(num);
	printNumber(num,sprite,x,y);

}

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
	for (int i=0;i<4;i++)
		list[i].active=1;
	for (int i=4;i<14;i++)
		list[i].active=0;
	list[0].ship=FURY;
	list[1].ship=TERMINATOR;
	list[2].ship=DREADNAUGHT;
	list[3].ship=AVENGER;




}

int ChooseNextShipRand(pPlrList list)
{
	int ret=1;

	int choose=ran(0,13);
	do
	{
		if (list[choose].active==1)
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

	return 0;
}

int DrawRemainingShips(pPlayer pl,pPlrList list,int sx,int sy)
{
	int i;
	int found=0;
	InitializeSprites();
	int score=0;

	for (i =0;i<14;i++)
	{	if (list[i].active==1)
		{
			found=1;
			sprites[42+i].attribute2 = SpriteAllShips+(list[i].ship*32) | PRIORITY(1);
			score+=list[i].ship;
		}
		else if (list[i].active==0)
		{
			sprites[42+i].attribute2 = SpriteAllShips+864 | PRIORITY(1);
		}
	}

	sprites[56].attribute2 = SpriteAllShips+832 | PRIORITY(1);
	if (found==0)
		return -1;

	LoadPal();
	LoadAllShips(SpriteAllShips*16);
	LoadLetters(SpriteLettersStart);
	for (i=0;i<5;i++)
	{
		sprites[42+i].attribute0 = COLOR_256 | SQUARE | sy;
    	sprites[42+i].attribute1 = SIZE_32 | (i*34)+sx;
    	sprites[47+i].attribute0 = COLOR_256 | SQUARE | 34+sy;
    	sprites[47+i].attribute1 = SIZE_32 | (i*34)+sx;
    	sprites[52+i].attribute0 = COLOR_256 | SQUARE | 68+sy;
    	sprites[52+i].attribute1 = SIZE_32 | (i*34)+sx;
	}

    s16 text=sx-10;//20;
    printLetter('p',31,text+10,15);
    printLetter('l',32,text+20,15);
    printLetter('a',33,text+30,15);
    printLetter('y',34,text+40,15);
    printLetter('e',35,text+50,15);
    printLetter('r',36,text+60,15);

    if (pl->plr==1)
    {
		printLetter('o',37,text+80,15);
	    printLetter('n',38,text+90,15);
    	printLetter('e',39,text+100,15);
	}
	else
	{
		printLetter('t',37,text+80,15);
	    printLetter('w',38,text+90,15);
    	printLetter('o',39,text+100,15);
	}
	return score; // maybe return value of remaining ships!!
}

int DrawWinner(pPlayer pl, pPlrList list)
{
	int sx=30,sy=30;
	int score=DrawRemainingShips(pl,list,sx,sy);
	printLetter('w',60,sx+110,15);
	printLetter('i',61,sx+120,15);
	printLetter('n',62,sx+130,15);
	printLetter('s',63,sx+140,15);

	printLargeNumber(score,64,sx,140,3);
	print("score=");
	print(score);
	//printLetter('o',64,sx,140);
	//printLetter('o',65,sx+10,140);
	//printLetter('o',66,sx+20,140);

	printLetter('p',67,sx+40,140);
	printLetter('o',68,sx+50,140);
	printLetter('i',69,sx+60,140);
	printLetter('n',70,sx+70,140);
	printLetter('t',71,sx+80,140);
	if (score!=1)
		printLetter('s',72,sx+90,140);

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
	print("draw ships");
	if (DrawRemainingShips(pl,list,sx,sy)==-1)
	{
		InitializeSprites();
		WaitForVsync();
		CopyOAM();
		return -1;
	}

	 printLetter('s',60,sx,140);
	 printLetter('e',61,sx+10,140);
	 printLetter('l',62,sx+20,140);
	 printLetter('e',63,sx+30,140);
	 printLetter('c',64,sx+40,140);
	 printLetter('t',65,sx+50,140);
	 printLetter('n',66,sx+70,140);
	 printLetter('e',67,sx+80,140);
	 printLetter('x',68,sx+90,140);
	 printLetter('t',69,sx+100,140);
	 printLetter('s',70,sx+120,140);
	 printLetter('h',71,sx+130,140);
	 printLetter('i',72,sx+140,140);
	 printLetter('p',73,sx+150,140);


print("drawn ok");

	sprites[57].attribute2 = SpriteAllShips+800 | PRIORITY(0);
	sprites[57].attribute0 = COLOR_256 | SQUARE | MODE_TRANSPARENT |y;
    sprites[57].attribute1 = SIZE_32 | x;

    WaitForVsync();
	CopyOAM();



	// TAKEN OUT TEMPORARILY SO I CAN CHOOSE OPP
/*
	if (pl->ai>PLAYER2)
	{

		for(i=0;i<500;i++)
			WaitForVsync();
		InitializeSprites();
		WaitForVsync();
		CopyOAM();
		return ChooseNextShipRand(list);
	}
*/

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
			print("button pressed");
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
				int t=((x-sx)/34);
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


//tests
p1->xpos = 1638;	//variables to hold position of sprite on screen
p1->ypos = 1623;
p2->xpos = 1562;	//variables to hold position of sprite on screen
p2->ypos = 1577;

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


	//	printLargeNumber(25,1,11,1,2);



		do//main  loop
		{
		for(loop = 0; loop < 256; loop++)          //load the sprite palette into memory
			OBJPaletteMem[loop] = sc2titlesPalette[loop];


		for(loop = SpriteTitleStart*16; loop < (SpriteTitleStart*16)+1024; loop++)               //load sprite image data
		{
			OAMData[loop] = sc2titlesData[loop-(SpriteTitleStart*16)];
		}
		LoadLetters(SpriteLettersStart);

	/*	for(loop = 0; loop < 7472; loop++)               //load sprite image data
				{
					OAMData[loop] = gfx_data[loop];
		}*/

		sprites[30].attribute0 = COLOR_256 | TALL | ROTATION_FLAG | SIZE_DOUBLE | MODE_TRANSPARENT | 20;	//setup sprite info, 256 colour, shape and y-coord
		sprites[30].attribute1 = SIZE_64 | ROTDATA(30) | 50;            //size 32x32 and x-coord
		sprites[30].attribute2 = SpriteTitleStart | PRIORITY(1); //pointer to tile where sprite starts
		//sprites[30].attribute2 = SC2TITLE_START | PRIORITY(1); //pointer to tile where sprite starts
		RotateSprite(30, 0, 128,128);
		printLetter('p',31,60,110);
		printLetter('r',32,70,110);
		printLetter('e',33,80,110);
		printLetter('s',34,90,110);
		printLetter('s',35,100,110);
		printLetter('s',36,115,110);
		printLetter('t',37,125,110);
		printLetter('a',38,135,110);
		printLetter('r',39,145,110);
		printLetter('t',40,155,110);
		WaitForVsync();
		CopyOAM();

		 //test sound
		init_sfx_system();

		play_sfx(&urquan_ditty,1);
		//play_sfx(&pkunk_stupid,0);



		while (*KEYS & KEY_START);

		int x=80,y=100,ay=y;
		printLetter('w',31,x,y);
		printLetter('e',32,x+10,y);
		printLetter('a',33,x+20,y);
		printLetter('k',34,x+30,y);

		printLetter('g',35,x,y+10);
		printLetter('o',36,x+10,y+10);
		printLetter('o',37,x+20,y+10);
		printLetter('d',38,x+30,y+10);

		printLetter('a',39,x,y+20);
		printLetter('w',40,x+10,y+20);
		printLetter('e',41,x+20,y+20);
		printLetter('s',42,x+30,y+20);
		printLetter('o',43,x+40,y+20);
		printLetter('m',44,x+50,y+20);
		printLetter('e',45,x+60,y+20);

		printLetter('d',56,x,y+30);
		printLetter('i',57,x+10,y+30);
		printLetter('s',58,x+20,y+30);
		printLetter('a',59,x+30,y+30);
		printLetter('b',60,x+40,y+30);
		printLetter('l',61,x+50,y+30);
		printLetter('e',62,x+60,y+30);
		printLetter('d',63,x+70,y+30);

		sprites[52].attribute0 = COLOR_256 | SQUARE | y;
		sprites[52].attribute1 = SIZE_8 | x-10;
		sprites[52].attribute2 = SpriteLettersStart+26*2 | PRIORITY(1);

		WaitForVsync();
		CopyOAM();
		int count=0;
		do
		{
			if(!(*KEYS & KEY_UP)&&ay>y)                   //if the UP key is pressed
			{

				count==0;
				ay=ay-10;
				MoveSprite(&sprites[52],x-10,ay);
				WaitForVsync();
				CopyOAM();
				//while(*KEYS & KEY_UP);
				for (int i=0;i<100;i++)
					WaitForVsync();
			}
			if(!(*KEYS & KEY_DOWN)&&ay<30+y)                 //if the DOWN key is pressed
			{
				count==0;
				ay=ay+10;
				MoveSprite(&sprites[52],x-10,ay);
				WaitForVsync();
				CopyOAM();
				for (int i=0;i<100;i++)
					WaitForVsync();
			}
		}while ((*KEYS & KEY_A)&&(*KEYS & KEY_B));
		//while(!((*KEYS & KEY_A)&&(*KEYS & KEY_B)));
		for (int i=0;i<80;i++)
			WaitForVsync();
		p1->ai=PLAYER1;
		if (ay==y+30)
			p2->ai=DISABLED;
		else if (ay==y+20)
			p2->ai=AWESOME;
		else if (ay==y+10)
			p2->ai=GOOD;
		else
			p2->ai=STANDARD;

		//for (int i=0;i<10;i++);


		print("\n p2 ai=");
		print(p2->ai);


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

		ChooseShips(p1,plist1);
		ChooseShips(p2,plist2);

		print("\n first p1 ships is ");
		print(plist1[0].ship);

		//int p1c=0;
		//int p2c=0;
		s16 nextp1=ChooseNextShip(p1,plist1);
		p1->ship=plist1[nextp1].ship;
		s16 nextp2=ChooseNextShip(p2,plist2);
		print("\n  choosen p2");
		p2->ship=plist2[nextp2].ship;
		print("\n  assigned p2");
		SetShip(p1);
		print("\n  set p1");
		SetShip(p2);
		print("\n  set p2");
		SetNew(p1);
		print("\n  set p1 new");
		SetNew(p2);
		print("\n  done settings");
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
       		if (p1->crew<1)
       		{
				plist1[nextp1].active=0;
				nextp1 = ChooseNextShip(p1,plist1);

				if (nextp1!=-1)
				{	p1->ship=plist1[nextp1].ship;
       				SetShip(p1);
       				SetNew(p1);
       				//LoadShip(p1);
       				p1->loadfunc(p1->SpriteStart);


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
				    //LoadShip(p2);
				    p2->loadfunc(p2->SpriteStart);
				}
			}

		}
		while (nextp1!=-1&&nextp2!=-1);
		//game over!!!
		if (nextp1==-1)
			DrawWinner(p2,plist2);
		else
			DrawWinner(p1,plist1);

	}//end loop - got back to titles;
	while(1);



}



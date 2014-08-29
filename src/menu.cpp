#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"


#include "letters.h"



//title
#include "sc2titles.h"


//typedef void (*fnptr)(void);
//#define REG_INTMAIN *(fnptr*)(0x03fffffc)

#include<math.h>
#include<stdlib.h>

extern u16* OAM;
extern pOAMEntry sprites;
extern pRotData rotData;
extern int* KEYS;

extern s8 pilot;
extern skill aiskill;
extern s8 sound;
extern u8 *pSaveMemory;

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

void DrawLetter(char letter,s16 sprite,  s16 x, s16 y)
{
//	print("letter p");
//	print((int)'p');
	//a=97 z=122 p = 112
	int off=(int)letter-97;
	sprites[sprite].attribute0 = COLOR_256 | SQUARE | y;
	sprites[sprite].attribute1 = SIZE_8 | x;
	sprites[sprite].attribute2 = SpriteLettersStart+off*2 | PRIORITY(1);
}

void DrawNumber(int num,s16 sprite,  s16 x, s16 y)
{
	if (num>9)
		num=0;
	int off=(num+27)*2;
	sprites[sprite].attribute0 = COLOR_256 | SQUARE | y;
	sprites[sprite].attribute1 = SIZE_8 | x;
	sprites[sprite].attribute2 = SpriteLettersStart+off | PRIORITY(1);

}

void DrawLargeNumber(int num,s16 sprite, s16 x, s16 y,s16 digits)
{

	while(digits>1)
	{
		int off=(int)pow(double(10),double(digits-1));//(10^(digits-1));

		if (num>off)
		{
			int tmp=num/off;

			DrawNumber(tmp,sprite,x,y);
			num=num-(tmp*off);
		}
		else
			DrawNumber(0,sprite,x,y);
		x+=10;
		sprite++;
		digits--;


	}
	//1 digit number

	DrawNumber(num,sprite,x,y);

}


void pilotoption(int x,int y)
{
if (pilot)
	{
		DrawLetter('o',37,x+70,y);
		DrawLetter('n',38,x+80,y);
		DrawLetter('e',39,240,160);
	}
	else
	{
		DrawLetter('o',37,x+70,y);
		DrawLetter('f',38,x+80,y);
		DrawLetter('f',39,x+90,y);
	}
}

void soundoption(int x,int y)
{
if (sound)
	{
		DrawLetter('o',60,x,y);
		DrawLetter('n',61,x+10,y);
		DrawLetter('e',62,240,160);
	}
	else
	{
		DrawLetter('o',63,x,y);
		DrawLetter('f',64,x+10,y);
		DrawLetter('f',65,x+20,y);
	}
}

void skilloption(int x,int y)
{
	if (aiskill==WEAK)
	{
		DrawLetter('w',47,x,y);
		DrawLetter('e',48,x+10,y);
		DrawLetter('a',49,x+20,y);
		DrawLetter('k',50,x+30,y);
		DrawLetter('b',51,240,160);
		DrawLetter('l',52,240,160);
		DrawLetter('e',53,240,160);
		DrawLetter('d',54,240,160);
	}
	else if (aiskill==GOOD)
	{
		DrawLetter('g',47,x,y);
		DrawLetter('o',48,x+10,y);
		DrawLetter('o',49,x+20,y);
		DrawLetter('d',50,x+30,y);
		DrawLetter('b',51,240,160);
		DrawLetter('l',52,240,160);
		DrawLetter('e',53,240,160);
		DrawLetter('d',54,240,160);
	}
	else if (aiskill==AWESOME)
	{
		DrawLetter('a',47,x,y);
		DrawLetter('w',48,x+10,y);
		DrawLetter('e',49,x+20,y);
		DrawLetter('s',50,x+30,y);
		DrawLetter('o',51,x+40,y);
		DrawLetter('m',52,x+50,y);
		DrawLetter('e',53,x+60,y);
		DrawLetter('d',54,240,160);
	}
	else if (aiskill=DISABLED)
	{
		DrawLetter('d',47,x,y);
		DrawLetter('i',48,x+10,y);
		DrawLetter('s',49,x+20,y);
		DrawLetter('a',50,x+30,y);
		DrawLetter('b',51,x+40,y);
		DrawLetter('l',52,x+50,y);
		DrawLetter('e',53,x+60,y);
		DrawLetter('d',54,x+70,y);
	}
}

void options()
{
	int x=70,y=100,ay=y,exit=y+36;
	DrawLetter('p',31,x,y);
	DrawLetter('i',32,x+10,y);
	DrawLetter('l',33,x+20,y);
	DrawLetter('o',34,x+30,y);
	DrawLetter('t',35,x+40,y);
	DrawLetter('s',36,x+50,y);
	pilotoption(x,y);

	DrawLetter('a',45,x,y+12);
	DrawLetter('i',46,x+10,y+12);
	skilloption(x+35,y+12);

	DrawLetter('s',55,x,y+24);
	DrawLetter('o',56,x+10,y+24);
	DrawLetter('u',57,x+20,y+24);
	DrawLetter('n',58,x+30,y+24);
	DrawLetter('d',59,x+40,y+24);
	soundoption(x+55,y+24);

	DrawLetter('e',40,x,exit);
	DrawLetter('x',41,x+10,exit);
	DrawLetter('i',42,x+20,exit);
	DrawLetter('t',43,x+30,exit);

	sprites[44].attribute0 = COLOR_256 | SQUARE | y;
	sprites[44].attribute1 = SIZE_8 | x-10;
	sprites[44].attribute2 = SpriteLettersStart+26*2 | PRIORITY(1);

	WaitForVsync();
	CopyOAM();
	int count=0;
	int exitcode=1;
	do
	{
		if(!(*KEYS & KEY_UP)&&ay>y)                   //if the UP key is pressed
		{
				count==0;
				ay=ay-12;
				MoveSprite(&sprites[44],x-10,ay);
				WaitForVsync();
				CopyOAM();
				//while(*KEYS & KEY_UP);
				for (int i=0;i<100;i++)
						WaitForVsync();
				}
				if(!(*KEYS & KEY_DOWN)&&ay<exit)                 //if the DOWN key is pressed
				{
					count==0;
					ay=ay+12;
					MoveSprite(&sprites[44],x-10,ay);
					WaitForVsync();
					CopyOAM();

				}
				if ((!(*KEYS & KEY_A))||(!(*KEYS & KEY_B)))
				{
					if (ay==y)
					{
						pilot=!pilot;
						pilotoption(x,y);
						for (int i=0;i<100;i++)
							WaitForVsync();
					}
					else if (ay==y+12)
					{
						if (aiskill==WEAK)
							aiskill=GOOD;
						else if (aiskill==GOOD)
							aiskill=AWESOME;
						else if (aiskill==AWESOME)
							aiskill=DISABLED;
						else if (aiskill==DISABLED)
							aiskill=WEAK;
						skilloption(x+35,y+12);
					}
					else if (ay==y+24)
					{
						sound=!sound;
						soundoption(x+55,y+24);
						for (int i=0;i<100;i++)
							WaitForVsync();
					}

					else if (ay==exit)
					{
						exitcode=0;
					}

					WaitForVsync();
					CopyOAM();
					for (int i=0;i<200;i++)
						WaitForVsync();
				}
			}while (exitcode);

			if (aiskill==WEAK)
				pSaveMemory[0]=1;
			else if (aiskill==AWESOME)
				pSaveMemory[0]=2;
			else if (aiskill==DISABLED)
				pSaveMemory[0]=3;
			else
				pSaveMemory[0]=0; //GOOD

			if (pilot==1)
				pSaveMemory[1]=0;
			else
				pSaveMemory[1]=1;

			if (sound==1)
				pSaveMemory[2]=0;
			else
				pSaveMemory[2]=1;



			for (int i=31;i<66;i++)
				MoveSprite(&sprites[i],240,160);

			for (int i=0;i<80;i++)
				WaitForVsync();



}




int drawstartmenu()
{
	int x=65,y=100,ay=y;
	DrawLetter('s',31,x,y);
	DrawLetter('t',32,x+10,y);
	DrawLetter('a',33,x+20,y);
	DrawLetter('r',34,x+30,y);
	DrawLetter('t',35,x+40,y);
	DrawLetter('g',36,x+60,y);
	DrawLetter('a',37,x+70,y);
	DrawLetter('m',38,x+80,y);
	DrawLetter('e',39,x+90,y);

	DrawLetter('o',40,x,y+20);
	DrawLetter('p',41,x+10,y+20);
	DrawLetter('t',42,x+20,y+20);
	DrawLetter('i',43,x+30,y+20);
	DrawLetter('o',44,x+40,y+20);
	DrawLetter('n',45,x+50,y+20);
	DrawLetter('s',46,x+60,y+20);

	sprites[54].attribute0 = COLOR_256 | SQUARE | y;
	sprites[54].attribute1 = SIZE_8 | x-10;
	sprites[54].attribute2 = SpriteLettersStart+26*2 | PRIORITY(1);

	WaitForVsync();
	CopyOAM();
	int count=0;
	do
	{
		if(!(*KEYS & KEY_UP)&&ay>y)                   //if the UP key is pressed
		{
				count==0;
				ay=ay-20;
				MoveSprite(&sprites[54],x-10,ay);
				WaitForVsync();
				CopyOAM();
				//while(*KEYS & KEY_UP);
				for (int i=0;i<100;i++)
						WaitForVsync();
				}
				if(!(*KEYS & KEY_DOWN)&&ay<20+y)                 //if the DOWN key is pressed
				{
					count==0;
					ay=ay+20;
					MoveSprite(&sprites[54],x-10,ay);
					WaitForVsync();
					CopyOAM();
					for (int i=0;i<100;i++)
						WaitForVsync();
				}
			}while ((*KEYS & KEY_A)&&(*KEYS & KEY_B));

			for (int i=0;i<150;i++)
				WaitForVsync();

			for (int i=31;i<55;i++)
				MoveSprite(&sprites[i],240,160);

			if (ay==y)
				return 0;
			else
				return 1;

}

void startmenu()
{
	int x =1;
	do
	{
		x=drawstartmenu();
		if (x==1)
			options();
	}
	while (x==1);

}
/* OLD CODE
skill choose_ai()
{
	int x=80,y=100,ay=y;
				DrawLetter('w',31,x,y);
				DrawLetter('e',32,x+10,y);
				DrawLetter('a',33,x+20,y);
				DrawLetter('k',34,x+30,y);

				DrawLetter('g',35,x,y+10);
				DrawLetter('o',36,x+10,y+10);
				DrawLetter('o',37,x+20,y+10);
				DrawLetter('d',38,x+30,y+10);

				DrawLetter('a',39,x,y+20);
				DrawLetter('w',40,x+10,y+20);
				DrawLetter('e',41,x+20,y+20);
				DrawLetter('s',42,x+30,y+20);
				DrawLetter('o',43,x+40,y+20);
				DrawLetter('m',44,x+50,y+20);
				DrawLetter('e',45,x+60,y+20);

				DrawLetter('d',46,x,y+30);
				DrawLetter('i',47,x+10,y+30);
				DrawLetter('s',48,x+20,y+30);
				DrawLetter('a',49,x+30,y+30);
				DrawLetter('b',50,x+40,y+30);
				DrawLetter('l',51,x+50,y+30);
				DrawLetter('e',52,x+60,y+30);
				DrawLetter('d',53,x+70,y+30);

				sprites[54].attribute0 = COLOR_256 | SQUARE | y;
				sprites[54].attribute1 = SIZE_8 | x-10;
				sprites[54].attribute2 = SpriteLettersStart+26*2 | PRIORITY(1);

				WaitForVsync();
				CopyOAM();
				int count=0;
				do
				{
					if(!(*KEYS & KEY_UP)&&ay>y)                   //if the UP key is pressed
					{

						count==0;
						ay=ay-10;
						MoveSprite(&sprites[54],x-10,ay);
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
						MoveSprite(&sprites[54],x-10,ay);
						WaitForVsync();
						CopyOAM();
						for (int i=0;i<100;i++)
							WaitForVsync();
					}
				}while ((*KEYS & KEY_A)&&(*KEYS & KEY_B));

			for (int i=0;i<80;i++)
					WaitForVsync();

			for (int i=31;i<55;i++)
				MoveSprite(&sprites[i],240,160);

				if (ay==y+30)
					return DISABLED;
				else if (ay==y+20)
					return AWESOME;
				else if (ay==y+10)
					return GOOD;
				else
			return WEAK;
}
*/
void menu(pPlayer p1, pPlayer p2)
{
	startmenu();
	//p1->ai=PLAYER1;
	//p2->ai=choose_ai();
}

void DrawTitle()
{
	s32 loop;
	InitializeSprites();
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
			DrawLetter('p',31,60,110);
			DrawLetter('r',32,70,110);
			DrawLetter('e',33,80,110);
			DrawLetter('s',34,90,110);
			DrawLetter('s',35,100,110);
			DrawLetter('s',36,115,110);
			DrawLetter('t',37,125,110);
			DrawLetter('a',38,135,110);
			DrawLetter('r',39,145,110);
			DrawLetter('t',40,155,110);
			WaitForVsync();
			CopyOAM();


}
#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "gfx/urquan.h"  //urqan
#include "gfx/pkunk.h" //punk punk(sic)

//bullets
#include "gfx/urquan_fire.h"
#include "gfx/urquan_fighters.h"
#include "gfx/pkunk_fire.h"

//exp
#include "gfx/exp1.h"
#include "gfx/exp2.h"
#include "gfx/exp3.h"
#include "gfx/exp4.h"
#include "gfx/exp5.h"

//#include "gfx/ships.h"

//extern u16* OAMData;
extern u16* OAM;
//extern OAMEntry sprites[128];
extern pRotData rotData;

extern Player p1,p2;

void LoadExp(s16 OAMStart, s16 SpriteStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
  	{
       		OAMData[loop] = exp1Data[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
	{
	       		OAMData[loop+32] = exp2Data[loop-OAMStart];
	       		OAMData[loop+32+128] = exp3Data[loop-OAMStart];
	       		OAMData[loop+32+256] = exp4Data[loop-OAMStart];
	       		OAMData[loop+32+128+256] = exp5Data[loop-OAMStart];
   	}
}

void LoadDreadnaught(s16 OAMStart, s16 SpriteStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = urquanData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+512] = urquan_fireData[loop-OAMStart]; //loads some garb
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+512+128] = urquan_fightersData[loop-OAMStart]; //loads some garb
   	}

}

void LoadFury(s16 OAMStart, s16 SpriteStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
    {
       		OAMData[loop] = pkunkData[loop-OAMStart];
	}
    for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
	{
			OAMData[loop+512] = pkunk_fireData[loop-OAMStart]; //loads some garbv
    }

}


void LoadShip(s16 ship,s16 OAMStart, s16 SpriteStart)
{
	switch (ship)
	{
	case DREADNAUGHT:
		LoadDreadnaught(OAMStart,SpriteStart);
		break;
	case FURY:
		LoadFury(OAMStart,SpriteStart);
		break;
	default:;
	}
}


void LoadShips()
{
	s16 loop;
	for(loop = 0; loop < 256; loop++)          //load the sprite palette into memory
		OBJPaletteMem[loop] = pkunkPalette[loop];
	LoadShip(p1.ship,p1.OAMStart,p1.SpriteStart);
	LoadShip(p2.ship,p2.OAMStart,p2.SpriteStart);
	SetShip(&p1);
}



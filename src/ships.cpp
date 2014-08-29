#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "gfx/urquan.h"  //urqan
#include "gfx/urquan_out.h"  //urqan
#include "gfx/pkunk.h" //punk punk(sic)
#include "gfx/pkunk_out.h" //punk punk(sic)

//bullets
#include "gfx/urquan_fire.h"
#include "gfx/urquan_fighters.h"
#include "gfx/urquan_fighters_fire.h"
#include "gfx/pkunk_fire.h"

//exp
#include "gfx/exp1.h"
#include "gfx/exp2.h"
#include "gfx/exp3.h"
#include "gfx/exp4.h"
#include "gfx/exp5.h"

#include "gfx/trail.h"

//extern u16* OAMData;
extern u16* OAM;
//extern OAMEntry sprites[128];
extern pRotData rotData;

//extern Player p1,p2;

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

void LoadTrail(s16 OAMStart)
{

	s16 loop;
	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
  	{
       		OAMData[loop] = trailData[loop-OAMStart];
   	}
}

void LoadDreadnaught(s16 OAMStart, s16 SpriteStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = urquanData[loop-OAMStart];
       		OAMData[loop+512] = urquan_outData[loop-OAMStart];

   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
   	{
		OAMData[loop+512+512] = urquan_fireData[loop-OAMStart]; //loads some garb
   	}
   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+512+128+512] = urquan_fightersData[loop-OAMStart]; //loads some garb
   	}

   	for(loop = OAMStart; loop < OAMStart+32; loop++)               //load sprite image data
	{
		OAMData[loop+512+128+32+512] = urquan_fighters_fireData[loop-OAMStart]; //loads some garb
   	}

}

void LoadFury(s16 OAMStart, s16 SpriteStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
    {
       	OAMData[loop] = pkunkData[loop-OAMStart];
       	OAMData[loop+512] = pkunk_outData[loop-OAMStart];
	}
    for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
	{
		OAMData[loop+1024] = pkunk_fireData[loop-OAMStart]; //loads some garbv
    }

}


void ReallyLoadShip(s16 ship,s16 OAMStart, s16 SpriteStart)
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

void LoadPal()
{
	s16 loop;
	for(loop = 0; loop < 256; loop++)          //load the sprite palette into memory
		OBJPaletteMem[loop] = pkunkPalette[loop];

}

void LoadShip(pPlayer pl)
{
	ReallyLoadShip(pl->ship,pl->OAMStart,pl->SpriteStart);
}




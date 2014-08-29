#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"


//exp
#include "gfx/exp1.h"
#include "gfx/exp2.h"
#include "gfx/exp3.h"
#include "gfx/exp4.h"
#include "gfx/exp5.h"

#include "gfx/trail.h"

#include "gfx/planet.h"
#include "gfx/asteroid.h"

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
       	OAMData[loop+32] = exp2Data[loop-OAMStart];
   	}
   	for(loop = OAMStart; loop < OAMStart+128; loop++)               //load sprite image data
	{
	    OAMData[loop+32+128] = exp3Data[loop-OAMStart];
	   	OAMData[loop+32+256] = exp4Data[loop-OAMStart];
		OAMData[loop+32+128+256] = exp5Data[loop-OAMStart];
   	}
}

void LoadPlanet(s16 OAMStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+2048; loop++)               //load sprite image data
	{
		OAMData[loop] = planetData[loop-OAMStart];
   	}
}

void LoadAsteroid(s16 OAMStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
	{
		OAMData[loop] = asteroidData[loop-OAMStart];
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


void ReallyLoadShip(s16 ship,s16 OAMStart, s16 SpriteStart)
{
	switch (ship)
	{
	case DREADNAUGHT:
		LoadDreadnaught(OAMStart,SpriteStart);
		break;
	case FURY:
	//LoadDreadnaught(OAMStart,SpriteStart);
		LoadFury(OAMStart,SpriteStart);
		break;
	case TERMINATOR:
		LoadYehat(OAMStart,SpriteStart);
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

void LoadAllShips(s16 OAMStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
       		OAMData[loop] = androsynData[loop-OAMStart];
       		OAMData[loop+512] = arilouData[loop-OAMStart];
       		OAMData[loop+1024] = blackurqData[loop-OAMStart];
       		OAMData[loop+1536] = chenjesuData[loop-OAMStart];
       		OAMData[loop+2048] = chmmrData[loop-OAMStart];
       		OAMData[loop+2560] = druudgeData[loop-OAMStart];
       		OAMData[loop+3072] = humanData[loop-OAMStart];
			OAMData[loop+3584] = ilwrathData[loop-OAMStart];
			OAMData[loop+4096] = melnormeData[loop-OAMStart];
			OAMData[loop+4608] = mmrnmhrmData[loop-OAMStart];
			OAMData[loop+5120] = myconData[loop-OAMStart];
       		OAMData[loop+5632] = orzData[loop-OAMStart];
       		OAMData[loop+6144] = pkunkData[loop-OAMStart];
			OAMData[loop+6656] = probeData[loop-OAMStart];
			OAMData[loop+7168] = shofixtiData[loop-OAMStart];
			OAMData[loop+7680] = spathiData[loop-OAMStart];
			OAMData[loop+8192] = supoxData[loop-OAMStart];
       		OAMData[loop+8704] = syreenData[loop-OAMStart];
       		OAMData[loop+9216] = thraddData[loop-OAMStart];
			OAMData[loop+9728] = umgahData[loop-OAMStart];
			OAMData[loop+10240] = urquanData[loop-OAMStart];
			OAMData[loop+10752] = utwigData[loop-OAMStart];
			OAMData[loop+11264] = vuxData[loop-OAMStart];
			OAMData[loop+11776] = yehatData[loop-OAMStart];
			OAMData[loop+12288] = zoqfotData[loop-OAMStart];
			OAMData[loop+12800] = outlineData[loop-OAMStart];
			OAMData[loop+13312] = questionData[loop-OAMStart];
			OAMData[loop+13824] = crossData[loop-OAMStart];

	}
}




/*
#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "ur3.h"  //urqan
#include "punk.h" //punk punk(sic)

//bullets
#include "dread_b.h"
#include "fury_b.h"

//extern u16* OAMData;
extern u16* OAM;
//extern OAMEntry sprites[128];
extern pRotData rotData;

extern Player p1,p2;
/*
void LoadAllShips(s16 OAMStart)
{
	s16 loop;
	for(loop = OAMStart; loop < OAMStart+512; loop++)               //load sprite image data
  	{
			OAMData[loop] = avatar[loop-OAMStart];
			OAMData[loop+512] = avenger[loop-OAMStart];
			OAMData[loop] = blade[loop-OAMStart];
			OAMData[loop+512] = broodhome[loop-OAMStart];
			OAMData[loop] = cruiser[loop-OAMStart];
			OAMData[loop] = ur3Data[loop-OAMStart];
			OAMData[loop+512] = drone[loop-OAMStart];
       		OAMData[loop] = eluder[loop-OAMStart];
			OAMData[loop+512] = fury[loop-OAMStart];
			OAMData[loop] = guardian[loop-OAMStart];
			OAMData[loop+512] = intruder[loop-OAMStart];
			OAMData[loop] = jugger[loop-OAMStart];
			OAMData[loop+512] = marauder[loop-OAMStart];
			OAMData[loop] = nemeis[loop-OAMStart];
			OAMData[loop+512] = penetrator[loop-OAMStart];
			OAMData[loop] = podship[loop-OAMStart];
			OAMData[loop+512] = probe[loop-OAMStart];
			OAMData[loop] = scout[loop-OAMStart];
			OAMData[loop+512] = skiff[loop-OAMStart];
			OAMData[loop] = stinger[loop-OAMStart];
			OAMData[loop+512] = terminator[loop-OAMStart];
			OAMData[loop] = torch[loop-OAMStart];
			OAMData[loop+512] = trader[loop-OAMStart];
			OAMData[loop] = transformer[loop-OAMStart];
   	}

}
*/



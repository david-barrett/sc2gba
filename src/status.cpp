
#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sprite.h" //my generic sprite header file

#include "gfx/crew1.h"
#include "gfx/crew2.h"
#include "gfx/crew3.h"
#include "gfx/crew4.h"
#include "gfx/crew5.h"
#include "gfx/crew6.h"
#include "gfx/crew7.h"
#include "gfx/crew8.h"

#include "gfx/batt1.h"
#include "gfx/batt2.h"
#include "gfx/batt3.h"
#include "gfx/batt4.h"
#include "gfx/batt5.h"
#include "gfx/batt6.h"
#include "gfx/batt7.h"
#include "gfx/batt8.h"

#include "gfx/blank.h"

#include "gfx/line.h"

#include "gfx/endr1.h"
#include "gfx/endr2.h"
#include "gfx/endr3.h"
#include "gfx/endr4.h"
#include "gfx/endl1.h"
#include "gfx/endl2.h"
#include "gfx/endl3.h"
#include "gfx/endl4.h"

extern pOAMEntry sprites;
//extern pPlayer p1;
//extern pPlayer p2;
const int start=71;
void drawStatus(pPlayer p1,pPlayer p2);
const s16 crew1=SpriteStatusStart;
const s16 crew2=SpriteStatusStart+4;
const s16 crew3=SpriteStatusStart+8;
const s16 crew4=SpriteStatusStart+12;
const s16 crew5=SpriteStatusStart+16;
const s16 crew6=SpriteStatusStart+20;
const s16 crew7=SpriteStatusStart+24;
const s16 crew8=SpriteStatusStart+28;

const s16 blank = SpriteStatusStart+68;

const s16 batt1=SpriteStatusStart+32;
const s16 batt2=SpriteStatusStart+36;
const s16 batt3=SpriteStatusStart+40;
const s16 batt4=SpriteStatusStart+48;//jumps weird!
const s16 batt5=SpriteStatusStart+52;
const s16 batt6=SpriteStatusStart+56;
const s16 batt7=SpriteStatusStart+60;
const s16 batt8=SpriteStatusStart+64;

void SetupStatus(pPlayer p1,pPlayer p2)
{
	u16 loop;

	for (int i=start;i<start+48;i++)
	{
		sprites[i].attribute0 = COLOR_256 | SQUARE |50;
	    sprites[i].attribute1 = SIZE_8 | 240;
		sprites[i].attribute2 = 64 | PRIORITY(2);


	}




		for(loop = 0; loop < 64; loop++)               //load sprite image data
       	{
       		OAMData[loop+OAMStatusStart] = crew1Data[loop];
			OAMData[loop+OAMStatusStart+64] = crew2Data[loop];
			OAMData[loop+OAMStatusStart+128] = crew3Data[loop];
			OAMData[loop+OAMStatusStart+192] = crew4Data[loop];
			OAMData[loop+OAMStatusStart+256] = crew5Data[loop];
			OAMData[loop+OAMStatusStart+320] = crew6Data[loop];
			OAMData[loop+OAMStatusStart+384] = crew7Data[loop];
			OAMData[loop+OAMStatusStart+448] = crew8Data[loop];

			OAMData[loop+OAMStatusStart+512] = batt1Data[loop];
			OAMData[loop+OAMStatusStart+576] = batt2Data[loop];
			OAMData[loop+OAMStatusStart+640] = batt3Data[loop];
			OAMData[loop+OAMStatusStart+768] = batt4Data[loop];
			OAMData[loop+OAMStatusStart+832] = batt5Data[loop];
			OAMData[loop+OAMStatusStart+896] = batt6Data[loop];
			OAMData[loop+OAMStatusStart+960] = batt7Data[loop];
			OAMData[loop+OAMStatusStart+1024] = batt8Data[loop];

			OAMData[loop+OAMStatusStart+1088] = blankData[loop];
			OAMData[loop+OAMStatusStart+1152] = lineData[loop];

			OAMData[loop+OAMStatusStart+1216] = endl1Data[loop];
			OAMData[loop+OAMStatusStart+1280] = endl2Data[loop];
			OAMData[loop+OAMStatusStart+1344] = endl3Data[loop];
			OAMData[loop+OAMStatusStart+1408] = endl4Data[loop];

			OAMData[loop+OAMStatusStart+1472] = endr1Data[loop];
			OAMData[loop+OAMStatusStart+1536] = endr2Data[loop];
			OAMData[loop+OAMStatusStart+1600] = endr3Data[loop];
			OAMData[loop+OAMStatusStart+1664] = endr4Data[loop];



       	}
       	drawStatus(p1,p2);
       	UpdateCrew(p1);
		UpdateBatt(p1);
		UpdateCrew(p2);
		UpdateBatt(p2);
}



s16 statusCrew(s16 data,s16 minus)
{


	s16 d = data - minus;
	if (d<0)
		return(blank);
	switch (d)
	{
	case 0:
		return(blank);
	case 1:
		return (crew1);
		break;
	case 2:
		return (crew2);
		break;
	case 3:
		return (crew3);
		break;
	case 4:
		return (crew4);
		break;
	case 5:
		return (crew5);
		break;
	case 6:
		return (crew6);
		break;
	case 7:
		return (crew7);
		break;
	case 8:
	default:
		return (crew8);
	}
}

s16 statusCrewS(s16 data,s16 minus)
{
	s16 endl1=SpriteStatusStart+76;
	s16 endl2=SpriteStatusStart+80;
	s16 endl3=SpriteStatusStart+84;
	s16 endl4=SpriteStatusStart+88;
	s16 blank = SpriteStatusStart+68;
	s16 line = SpriteStatusStart+72;

	s16 d = data - minus;
	if (d<0)
		return(blank);
	switch (d)
	{
	case 0:
		return(endl1);
	case 1:
	case 2:
		return (endl2);
		break;
	case 3:
	case 4:
		return (endl3);
		break;
	case 5:
	case 6:
		return (endl4);
		break;
	case 7:
	case 8:
	default:
		return (line);
	}

}

s16 statusBattS(s16 data,s16 minus)
{
	s16 endr1=SpriteStatusStart+92;
		s16 endr2=SpriteStatusStart+96;
		s16 endr3=SpriteStatusStart+100;
		s16 endr4=SpriteStatusStart+104;
		s16 blank = SpriteStatusStart+68;
		s16 line = SpriteStatusStart+72;

		s16 d = data - minus;
		if (d<0)
			return(blank);
		switch (d)
		{
		case 0:
			return(endr1);
		case 1:
		case 2:
			return (endr2);
			break;
		case 3:
		case 4:
			return (endr3);
			break;
		case 5:
		case 6:
			return (endr4);
			break;
		case 7:
		case 8:
		default:
			return (line);
		}

}

s16 statusBatt(s16 data,s16 minus)
{


	s16 d = data - minus;
	if (d<0)
		return(blank);
	switch (d)
	{
	case 0:
		return(blank);
	case 1:
		return (batt1);
		break;
	case 2:
		return (batt2);
		break;
	case 3:
		return (batt3);
		break;
	case 4:
		return (batt4);
		break;
	case 5:
		return (batt5);
		break;
	case 6:
		return (batt6);
		break;
	case 7:
		return (batt7);
		break;
	case 8:
	default:
		return (batt8);
	}
}

void drawStatus(pPlayer p1,pPlayer p2)
{
	//max crew/batt =42

			sprites[start+24].attribute2 = statusCrewS(p1->maxcrew,40) | PRIORITY(3);
			sprites[start+25].attribute2 = statusCrewS(p1->maxcrew,32) | PRIORITY(3);
			sprites[start+26].attribute2 = statusCrewS(p1->maxcrew,24) | PRIORITY(3);
			sprites[start+27].attribute2 = statusCrewS(p1->maxcrew,16) | PRIORITY(3);
			sprites[start+28].attribute2 = statusCrewS(p1->maxcrew,8) | PRIORITY(3);
			sprites[start+29].attribute2 = statusCrewS(p1->maxcrew,0) | PRIORITY(3);

			sprites[start+30].attribute2 = statusBattS(p1->maxbatt,0) | PRIORITY(3);
			sprites[start+31].attribute2 = statusBattS(p1->maxbatt,8) | PRIORITY(3);
			sprites[start+32].attribute2 = statusBattS(p1->maxbatt,16) | PRIORITY(3);
			sprites[start+33].attribute2 = statusBattS(p1->maxbatt,32) | PRIORITY(3);
			sprites[start+34].attribute2 = statusBattS(p1->maxbatt,24) | PRIORITY(3);
			sprites[start+35].attribute2 = statusBattS(p1->maxbatt,40) | PRIORITY(3);

			sprites[start+36].attribute2 = statusCrewS(p2->maxcrew,40) | PRIORITY(3);
			sprites[start+37].attribute2 = statusCrewS(p2->maxcrew,32) | PRIORITY(3);
			sprites[start+38].attribute2 = statusCrewS(p2->maxcrew,24) | PRIORITY(3);
			sprites[start+39].attribute2 = statusCrewS(p2->maxcrew,16) | PRIORITY(3);
			sprites[start+40].attribute2 = statusCrewS(p2->maxcrew,8) | PRIORITY(3);
			sprites[start+41].attribute2 = statusCrewS(p2->maxcrew,0) | PRIORITY(3);

			sprites[start+42].attribute2 = statusBattS(p2->maxbatt,0) | PRIORITY(3);
			sprites[start+43].attribute2 = statusBattS(p2->maxbatt,8) | PRIORITY(3);
			sprites[start+44].attribute2 = statusBattS(p2->maxbatt,16) | PRIORITY(3);
			sprites[start+45].attribute2 = statusBattS(p2->maxbatt,24) | PRIORITY(3);
			sprites[start+46].attribute2 = statusBattS(p2->maxbatt,32) | PRIORITY(3);
			sprites[start+47].attribute2 = statusBattS(p2->maxbatt,40) | PRIORITY(3);



		   	    //p1 crew
		   		MoveSprite(&sprites[start+24], 8, 152);
		   		MoveSprite(&sprites[start+25], 16, 152);
		   		MoveSprite(&sprites[start+26], 24, 152);
		   		MoveSprite(&sprites[start+27], 32, 152);
		   		MoveSprite(&sprites[start+28], 40, 152);
		   		MoveSprite(&sprites[start+29], 48, 152);
		   		//p1 batt
		   		MoveSprite(&sprites[start+30], 56, 152);
		   		MoveSprite(&sprites[start+31], 64, 152);
		   		MoveSprite(&sprites[start+32], 72, 152);
		   		MoveSprite(&sprites[start+33], 80, 152);
		   		MoveSprite(&sprites[start+34], 88, 152);
		   		MoveSprite(&sprites[start+35], 96, 152);

		   		//p1 crew
		   		MoveSprite(&sprites[start+36], 136, 152);
		   		MoveSprite(&sprites[start+37], 144, 152);
		   		MoveSprite(&sprites[start+38], 152, 152);
		   		MoveSprite(&sprites[start+39], 160, 152);
		   		MoveSprite(&sprites[start+40], 168, 152);
		   		MoveSprite(&sprites[start+41], 176, 152);
		   		//p1 batt
		   		MoveSprite(&sprites[start+42], 184, 152);
		   		MoveSprite(&sprites[start+43], 192, 152);
		   		MoveSprite(&sprites[start+44], 200, 152);
		   		MoveSprite(&sprites[start+45], 208, 152);
		   		MoveSprite(&sprites[start+46], 216, 152);
				MoveSprite(&sprites[start+47], 224, 152);

			//p1 crew
					MoveSprite(&sprites[start], 8, 152);
					MoveSprite(&sprites[start+1], 16, 152);
					MoveSprite(&sprites[start+2], 24, 152);
					MoveSprite(&sprites[start+3], 32, 152);
					MoveSprite(&sprites[start+4], 40, 152);
					MoveSprite(&sprites[start+5], 48, 152);
					//p1 batt
					MoveSprite(&sprites[start+6], 56, 152);
					MoveSprite(&sprites[start+7], 64, 152);
					MoveSprite(&sprites[start+8], 72, 152);
					MoveSprite(&sprites[start+9], 80, 152);
					MoveSprite(&sprites[start+10], 88, 152);
					MoveSprite(&sprites[start+11], 96, 152);

					//p2 crew
					MoveSprite(&sprites[start+12], 136, 152);
					MoveSprite(&sprites[start+13], 144, 152);
					MoveSprite(&sprites[start+14], 152, 152);
					MoveSprite(&sprites[start+15], 160, 152);
					MoveSprite(&sprites[start+16], 168, 152);
					MoveSprite(&sprites[start+17], 176, 152);
					//p2 batt
					MoveSprite(&sprites[start+18], 184, 152);
					MoveSprite(&sprites[start+19], 192, 152);
					MoveSprite(&sprites[start+20], 200, 152);
					MoveSprite(&sprites[start+21], 208, 152);
					MoveSprite(&sprites[start+22], 216, 152);
		MoveSprite(&sprites[start+23], 224, 152);

}
/*
void UpdateStatus()
{
		//max crew/batt =42

		sprites[start].attribute2 = statusCrew(p1->crew,40) | PRIORITY(2);
		sprites[start+1].attribute2 = statusCrew(p1->crew,32) | PRIORITY(2);
		sprites[start+2].attribute2 = statusCrew(p1->crew,24) | PRIORITY(2);
		sprites[start+3].attribute2 = statusCrew(p1->crew,16) | PRIORITY(2);
		sprites[start+4].attribute2 = statusCrew(p1->crew,8) | PRIORITY(2);
		sprites[start+5].attribute2 = statusCrew(p1->crew,0) | PRIORITY(2);

		sprites[start+6].attribute2 = statusBatt(p1->batt,0) | PRIORITY(2);
		sprites[start+7].attribute2 = statusBatt(p1->batt,8) | PRIORITY(2);
		sprites[start+8].attribute2 = statusBatt(p1->batt,16) | PRIORITY(2);
		sprites[start+9].attribute2 = statusBatt(p1->batt,24) | PRIORITY(2);
		sprites[start+10].attribute2 = statusBatt(p1->batt,32) | PRIORITY(2);
		sprites[start+11].attribute2 = statusBatt(p1->batt,40) | PRIORITY(2);

		sprites[start+12].attribute2 = statusCrew(p2->crew,40) | PRIORITY(2);
		sprites[start+13].attribute2 = statusCrew(p2->crew,32) | PRIORITY(2);
		sprites[start+14].attribute2 = statusCrew(p2->crew,24) | PRIORITY(2);
		sprites[start+15].attribute2 = statusCrew(p2->crew,16) | PRIORITY(2);
		sprites[start+16].attribute2 = statusCrew(p2->crew,8) | PRIORITY(2);
		sprites[start+17].attribute2 = statusCrew(p2->crew,0) | PRIORITY(2);

		sprites[start+18].attribute2 = statusBatt(p2->batt,0) | PRIORITY(2);
		sprites[start+19].attribute2 = statusBatt(p2->batt,8) | PRIORITY(2);
		sprites[start+20].attribute2 = statusBatt(p2->batt,16) | PRIORITY(2);
		sprites[start+21].attribute2 = statusBatt(p2->batt,24) | PRIORITY(2);
		sprites[start+22].attribute2 = statusBatt(p2->batt,32) | PRIORITY(2);
		sprites[start+23].attribute2 = statusBatt(p2->batt,40) | PRIORITY(2);


/* dont need to do this again?
	    //p1 crew
		MoveSprite(&sprites[start], 8, 152);
		MoveSprite(&sprites[start+1], 16, 152);
		MoveSprite(&sprites[start+2], 24, 152);
		MoveSprite(&sprites[start+3], 32, 152);
		MoveSprite(&sprites[start+4], 40, 152);
		MoveSprite(&sprites[start+5], 48, 152);
		//p1 batt
		MoveSprite(&sprites[start+6], 56, 152);
		MoveSprite(&sprites[start+7], 64, 152);
		MoveSprite(&sprites[start+8], 72, 152);
		MoveSprite(&sprites[start+9], 80, 152);
		MoveSprite(&sprites[start+10], 88, 152);
		MoveSprite(&sprites[start+11], 96, 152);

		//p2 crew
		MoveSprite(&sprites[start+12], 136, 152);
		MoveSprite(&sprites[start+13], 144, 152);
		MoveSprite(&sprites[start+14], 152, 152);
		MoveSprite(&sprites[start+15], 160, 152);
		MoveSprite(&sprites[start+16], 168, 152);
		MoveSprite(&sprites[start+17], 176, 152);
		//p2 batt
		MoveSprite(&sprites[start+18], 184, 152);
		MoveSprite(&sprites[start+19], 192, 152);
		MoveSprite(&sprites[start+20], 200, 152);
		MoveSprite(&sprites[start+21], 208, 152);
		MoveSprite(&sprites[start+22], 216, 152);
		MoveSprite(&sprites[start+23], 224, 152);



}
*/

void UpdateCrew(pPlayer pl)
{
	int o=pl->plr==1?0:12;
	sprites[start+o].attribute2 = statusCrew(pl->crew,40) | PRIORITY(2);
	sprites[start+1+o].attribute2 = statusCrew(pl->crew,32) | PRIORITY(2);
	sprites[start+2+o].attribute2 = statusCrew(pl->crew,24) | PRIORITY(2);
	sprites[start+3+o].attribute2 = statusCrew(pl->crew,16) | PRIORITY(2);
	sprites[start+4+o].attribute2 = statusCrew(pl->crew,8) | PRIORITY(2);
	sprites[start+5+o].attribute2 = statusCrew(pl->crew,0) | PRIORITY(2);
}

void UpdateBatt(pPlayer pl)
{
	int o=pl->plr==1?0:12;
	sprites[start+6+o].attribute2 = statusBatt(pl->batt,0) | PRIORITY(2);
	sprites[start+7+o].attribute2 = statusBatt(pl->batt,8) | PRIORITY(2);
	sprites[start+8+o].attribute2 = statusBatt(pl->batt,16) | PRIORITY(2);
	sprites[start+9+o].attribute2 = statusBatt(pl->batt,24) | PRIORITY(2);
	sprites[start+10+o].attribute2 = statusBatt(pl->batt,32) | PRIORITY(2);
	sprites[start+11+o].attribute2 = statusBatt(pl->batt,40) | PRIORITY(2);
}



void ModifyCrew(pPlayer p, int i,int ignoreshield)
{
	if (!p->shield&&!ignoreshield)
	{
	p->crew+=i;
	if (p->crew>p->maxcrew)
		p->crew=p->maxcrew;

	UpdateCrew(p);
	}
}

void ModifyBatt(pPlayer p, int i)
{
	p->batt+=i;
	if (p->batt>p->maxbatt)
		p->batt=p->maxbatt;
	if (p->batt<0)
		p->batt=0;

	UpdateBatt(p);
}


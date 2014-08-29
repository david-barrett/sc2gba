
#include "gba.h"	//GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"
#include "sprite.h" //my generic sprite header file

#include "crew1.h"
#include "crew2.h"
#include "crew3.h"
#include "crew4.h"
#include "crew5.h"
#include "crew6.h"
#include "crew7.h"
#include "crew8.h"

#include "batt1.h"
#include "batt2.h"
#include "batt3.h"
#include "batt4.h"
#include "batt5.h"
#include "batt6.h"
#include "batt7.h"
#include "batt8.h"

#include "blank.h"

#include "line.h"

#include "endr1.h"
#include "endr2.h"
#include "endr3.h"
#include "endr4.h"
#include "endl1.h"
#include "endl2.h"
#include "endl3.h"
#include "endl4.h"

#include "status_pod1.h"
#include "status_pod2.h"

//#include "status_spaceman1.h"
//#include "status_spaceman2.h"

extern pOAMEntry sprites;
//extern pPlayer p1;
//extern pPlayer p2;
const int start=65;
void drawStatus(pPlayer p1,pPlayer p2);
const s16 crew1=SpriteStatusStart;
const s16 crew2=SpriteStatusStart+2;
const s16 crew3=SpriteStatusStart+4;
const s16 crew4=SpriteStatusStart+6;
const s16 crew5=SpriteStatusStart+8;
const s16 crew6=SpriteStatusStart+10;
const s16 crew7=SpriteStatusStart+12;
const s16 crew8=SpriteStatusStart+14;

const s16 batt1=SpriteStatusStart+16;
const s16 batt2=SpriteStatusStart+18;
const s16 batt3=SpriteStatusStart+20;
const s16 batt4=SpriteStatusStart+22;
const s16 batt5=SpriteStatusStart+24;
const s16 batt6=SpriteStatusStart+26;
const s16 batt7=SpriteStatusStart+28;
const s16 batt8=SpriteStatusStart+30;

const s16 blank = SpriteStatusStart+32;
const s16 line = SpriteStatusStart+34;

const s16 endl1=SpriteStatusStart+36;
const s16 endl2=SpriteStatusStart+38;
const s16 endl3=SpriteStatusStart+40;
const s16 endl4=SpriteStatusStart+42;
const s16 endr1=SpriteStatusStart+44;
const s16 endr2=SpriteStatusStart+46;
const s16 endr3=SpriteStatusStart+48;
const s16 endr4=SpriteStatusStart+50;

const s16 pod1=SpriteStatusStart+52;
const s16 pod2=SpriteStatusStart+54;


void SetupStatus(pPlayer p1,pPlayer p2)
{
	u32 loop;
	s32 OAMStatusStart = 16*SpriteStatusStart;

	for (int i=start;i<start+56;i++)
	{
		sprites[i].attribute0 = COLOR_256 | SQUARE |50;
	    sprites[i].attribute1 = SIZE_8 | 240;
		sprites[i].attribute2 = 64 | PRIORITY(2);
	}




		for(loop = 0; loop < 64; loop++)               //load sprite image data
       	{
       		OAMData[loop+OAMStatusStart] = crew1Data[loop];
			OAMData[loop+OAMStatusStart+32] = crew2Data[loop];
			OAMData[loop+OAMStatusStart+64] = crew3Data[loop];
			OAMData[loop+OAMStatusStart+96] = crew4Data[loop];
			OAMData[loop+OAMStatusStart+128] = crew5Data[loop];
			OAMData[loop+OAMStatusStart+160] = crew6Data[loop];
			OAMData[loop+OAMStatusStart+192] = crew7Data[loop];
			OAMData[loop+OAMStatusStart+224] = crew8Data[loop];

			OAMData[loop+OAMStatusStart+256] = batt1Data[loop];
			OAMData[loop+OAMStatusStart+288] = batt2Data[loop];
			OAMData[loop+OAMStatusStart+320] = batt3Data[loop];
			OAMData[loop+OAMStatusStart+352] = batt4Data[loop];
			OAMData[loop+OAMStatusStart+384] = batt5Data[loop];
			OAMData[loop+OAMStatusStart+416] = batt6Data[loop];
			OAMData[loop+OAMStatusStart+448] = batt7Data[loop];
			OAMData[loop+OAMStatusStart+480] = batt8Data[loop];
//I really dont understand - not including these 2 and yet they appear...

			OAMData[loop+OAMStatusStart+512] = blankData[loop];
		//	OAMData[loop+OAMStatusStart+544] = lineData[loop];

			OAMData[loop+OAMStatusStart+576] = endl1Data[loop];
			OAMData[loop+OAMStatusStart+608] = endl2Data[loop];
			OAMData[loop+OAMStatusStart+640] = endl3Data[loop];
			//OAMData[loop+OAMStatusStart+672] = endl4Data[loop];

			OAMData[loop+OAMStatusStart+704] = endr1Data[loop];
			OAMData[loop+OAMStatusStart+736] = endr2Data[loop];
			OAMData[loop+OAMStatusStart+768] = endr3Data[loop];
		//	OAMData[loop+OAMStatusStart+800] = endr4Data[loop];

			OAMData[loop+OAMStatusStart+832] = status_pod1Data[loop];
			OAMData[loop+OAMStatusStart+864] = status_pod2Data[loop];
			//OAMData[loop+OAMStatusStart+896] = status_spaceman1Data[loop];
			//OAMData[loop+OAMStatusStart+928] = status_spaceman2Data[loop];

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

s16 statusLimpet(s16 data,s16 minus)
{


	s16 d = data - minus;
	if (d<0)
		return(blank);
	switch (d)
	{
	case 0:
		return(blank);
	case 1:
		return (pod1);
		break;
	case 2:
	default:
		return (pod2);
		break;
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
			sprites[start+33].attribute2 = statusBattS(p1->maxbatt,24) | PRIORITY(3);
			sprites[start+34].attribute2 = statusBattS(p1->maxbatt,32) | PRIORITY(3);
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

			sprites[start+48].attribute2 = statusLimpet(p1->limpets,0) | PRIORITY(2);
			sprites[start+49].attribute2 = statusLimpet(p1->limpets,2) | PRIORITY(2);
			sprites[start+50].attribute2 = statusLimpet(p1->limpets,4) | PRIORITY(2);
			sprites[start+51].attribute2 = statusLimpet(p1->limpets,6) | PRIORITY(3);


			sprites[start+52].attribute2 = statusLimpet(p1->limpets,0) | PRIORITY(2);
			sprites[start+53].attribute2 = statusLimpet(p1->limpets,2) | PRIORITY(2);
			sprites[start+54].attribute2 = statusLimpet(p1->limpets,4) | PRIORITY(2);
			sprites[start+55].attribute2 = statusLimpet(p1->limpets,6) | PRIORITY(2);



		   	    //p1 crew border
		   		MoveSprite(&sprites[start+24], 8, 152);
		   		MoveSprite(&sprites[start+25], 16, 152);
		   		MoveSprite(&sprites[start+26], 24, 152);
		   		MoveSprite(&sprites[start+27], 32, 152);
		   		MoveSprite(&sprites[start+28], 40, 152);
		   		MoveSprite(&sprites[start+29], 48, 152);
		   		//p1 batt border
		   		MoveSprite(&sprites[start+30], 56, 152);
		   		MoveSprite(&sprites[start+31], 64, 152);
		   		MoveSprite(&sprites[start+32], 72, 152);
		   		MoveSprite(&sprites[start+33], 80, 152);
		   		MoveSprite(&sprites[start+34], 88, 152);
		   		MoveSprite(&sprites[start+35], 96, 152);

		   		//p2 crew border
		   		MoveSprite(&sprites[start+36], 136, 152);
		   		MoveSprite(&sprites[start+37], 144, 152);
		   		MoveSprite(&sprites[start+38], 152, 152);
		   		MoveSprite(&sprites[start+39], 160, 152);
		   		MoveSprite(&sprites[start+40], 168, 152);
		   		MoveSprite(&sprites[start+41], 176, 152);
		   		//p2 batt border
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


		int x=58+(p1->maxbatt);
		//p1 limpet
		MoveSprite(&sprites[start+48], x, 151);
		MoveSprite(&sprites[start+49], x+4, 151);
		MoveSprite(&sprites[start+50], x+8, 151);
		MoveSprite(&sprites[start+51], x+12, 151);

		x=178-(p2->maxcrew);
		//p2 limpet
		MoveSprite(&sprites[start+52], x, 151);
		MoveSprite(&sprites[start+53], x-4, 151);
		MoveSprite(&sprites[start+54], x-8, 151);
		MoveSprite(&sprites[start+55], x-12, 151);

}

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

void UpdateLimpet(pPlayer pl)
{
	int o=pl->plr==1?0:4;
	sprites[start+48+o].attribute2 = statusLimpet(pl->limpets,0) | PRIORITY(2);
	sprites[start+49+o].attribute2 = statusLimpet(pl->limpets,2) | PRIORITY(2);
	sprites[start+50+o].attribute2 = statusLimpet(pl->limpets,4) | PRIORITY(2);
	sprites[start+51+o].attribute2 = statusLimpet(pl->limpets,6) | PRIORITY(2);
}



void ModifyCrew(pPlayer p, int i,int ignoreshield)
{
	if (!p->shield||ignoreshield)
	{
	p->crew+=i;
	if (p->crew>p->maxcrew)
		p->crew=p->maxcrew;
	if(p->crew<0)
		p->crew=0;

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

void AddLimpet(pPlayer p)
{
	if (p->limpets<8)
		p->limpets++;
	UpdateLimpet(p);
}



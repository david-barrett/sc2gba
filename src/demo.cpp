#include "gba.h"	//GBA register definitions

#include "dispcnt.h"    //REG_DISPCNT register #defines
#include "sc.h"       //my generic sprite header file
#include "sprite.h"
#include "sincosrad.h"
#include "bg.h"

extern s8 pilot;
extern skill aiskill;
extern s8 demo;
extern s8 planet;

int Available(s16 selected);

void Demo(pPlayer p1,pPlayer p2,pBg bg0,pBg bg1)
{

		s8 tmppilots=pilot;
				pilot=1;
				InitializeSprites();
				WaitForVsync();
				CopyOAM();
				if (aiskill==DISABLED)
				{
					p1->ai=GOOD;
					p2->ai=GOOD;
				}
				else
				{
					p1->ai=aiskill;
					p2->ai=aiskill;
				}
/*
				if (demo==1)
				{
					p1->ship=INTRUDER;
				 	p2->ship=PENETRATOR;
				}
				else if (demo==2)
				{
					p1->ship=DREADNAUGHT;
				 	p2->ship=TERMINATOR;
				}
				else if (demo==3)
				{
					p1->ship=FURY;
				 	p2->ship=AVENGER;
				}
				else if (demo==4)
				{
					p1->ship=INTRUDER;
				 	p2->ship=TERMINATOR;
				}
				else if (demo==5)
				{
					p1->ship=DREADNAUGHT;
					p2->ship=FURY;
				}
				else if (demo==6)
				{
					p1->ship=PENETRATOR;
					p2->ship=AVENGER;
				}

				if (demo==7)
				{
					p1->ship=GUARDIAN;
					p2->ship=ELUDER;
				}

				if (demo==8)
				{
					p1->ship=DRONE;
					p2->ship=AVENGER;
				}

				if (demo==9)
				{
					p1->ship=PODSHIP;
					p2->ship=CRUISER;
				}


				demo++;
				if (demo==10)
					demo=1;
*/
				do
				{
					p1->ship=ran(0,24);
					if (!Available(p1->ship))
						p1->ship=-1;
				}
				while(p1->ship==-1);

				do
				{
					p2->ship=ran(0,24);
					if (!Available(p2->ship))
						p2->ship=-1;
				}
				while(p2->ship==-1);

				SetNew(p1);
				SetNew(p2);
				SetShip(p1);
				SetShip(p2);
				planet++;
				Melee(p1,p2,bg0,bg1);
				pilot=tmppilots;


}


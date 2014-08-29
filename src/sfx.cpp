///////////////////////////////////////////////////////////////////////////////
// sfx.c
///////////////////////////////////////////////////////////////////////////////

#include "gba.h"
#include "sfx.h"
//#include "sound/sound.h"
#include "TektronicWave.h"

#define INT_TIMER0       0x0008
#define INT_TIMER1       0x0010

u32 iNextSampleA=0;
//u32 SampleSizeA=0;
//s8 *soundA;
u32 iNextSampleB=0;
//u32 SampleSizeB=0;
//s8 *soundB;
const PCMSOUND *pcmsoundA;
const PCMSOUND *pcmsoundB;

extern s8 sound;


//void InterruptProcess(void) __attribute__((section(".iwram")));

void InterruptProcess(void)
{
	if (REG_IF&INT_TIMER0)
	{
		if(!(iNextSampleA&3))				// use associate variable name
			REG_SGFIFOA=(*(((u32 *)(pcmsoundA->pName))+(iNextSampleA>>2)));
		iNextSampleA++;
		if(iNextSampleA>pcmsoundA->nLength) { 	REG_TM0CNT=0;}

		REG_IF |=INT_TIMER0;
	}
	//else T1
	if (REG_IF&INT_TIMER1)
	{/*
		if (!(iNextSampleB&3))
			REG_SGFIFOB=soundB[iNextSampleB<<2];

		iNextSampleB++;

		if (iNextSampleB>(SampleSizeB/4))
		{
			REG_TM1CNT=0;
		//	REG_DMA1CNT = 0;
		}
		*/
		if(!(iNextSampleB&3))				// use associate variable name
				REG_SGFIFOB=(*(((u32 *)(pcmsoundB->pName))+(iNextSampleB>>2)));
		iNextSampleB++;
		if(iNextSampleB>pcmsoundB->nLength) { 	REG_TM1CNT=0;}
		REG_IF |=INT_TIMER1;
	}



	//REG_IF |= REG_IF;
}

// init_sfx_system - sets the registers for enabling the sound hardware and
//                   Direct Sound
// PARAMETERS:  none
// RETURNS:     none
void init_sfx_system(void)
{
    // turn on the sound chip
    REG_SOUNDCNT_X = SND_ENABLED;

    // make sure sound channels 1-4 are turned off
    REG_SOUNDCNT_L = 0;

    // set the direct sound output control register
    REG_SOUNDCNT_H = SND_OUTPUT_RATIO_100 | // 100% sound output
                     DSA_OUTPUT_RATIO_100 | // 100% direct sound A output
                     DSA_OUTPUT_TO_BOTH |   // output Direct Sound A to both right and left speakers
                     DSA_TIMER0 |           // use timer 0 to determine the playback frequency of Direct Sound A
                     DSA_FIFO_RESET |       // reset the FIFO for Direct Sound A
                     DSB_OUTPUT_RATIO_100 | // 100% direct sound B output
					 DSB_OUTPUT_TO_BOTH |   // output Direct Sound B to both right and left speakers
					 DSB_TIMER1 |           // use timer 1 to determine the playback frequency of Direct Sound A
                     DSB_FIFO_RESET;    // reset the FIFO for Direct Sound B
}

// play_sfx - starts the DMA of a sample and waits for it to complete
// PARAMETERS:  pSample - a pointer to the sample we want to play
// RETURNS:     none
void play_sfx(const PCMSOUND *pSample,short channel)
{
	if (sound)
	{
	if (channel==0)
	{

		pcmsoundA=pSample;
		iNextSampleA=0;
		REG_TM0D=0xffff-(unsigned int)16772216/pSample->nSamplingRate; // aproximated
		REG_TM0CNT=0x00C0;

	}
	else
	{
		pcmsoundB=pSample;
    	iNextSampleB=0;
		REG_TM1D=0xffff-(unsigned int)16772216/pSample->nSamplingRate; // aproximated
		REG_TM1CNT=0x00C0;
	}
	}
}


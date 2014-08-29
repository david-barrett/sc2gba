///////////////////////////////////////////////////////////////////////////////
// sfx.c
///////////////////////////////////////////////////////////////////////////////

#include "gba.h"
#include "sfx.h"

#define INT_TIMER0       0x0008
#define INT_TIMER1       0x0010

//#include "sound/urquan-ditty.h"



int iNextSampleA=0;
int SampleSizeA=0;
s8 *soundA;
int iNextSampleB=0;
int SampleSizeB=0;
s8 *soundB;


//void InterruptProcess(void) __attribute__((section(".iwram")));

void InterruptProcess(void)
{
	if (REG_IF&INT_TIMER0)
	{
	if (!(iNextSampleA&3))
		REG_SGFIF0A=soundA[iNextSampleA<<2];

	iNextSampleA++;

	if (iNextSampleA>(SampleSizeA/4))
	{
		REG_TM0CNT=0;
	//	REG_DMA1CNT = 0;
	}
	REG_IF |=INT_TIMER0;
	}
	//else T1
	if (REG_IF&INT_TIMER1)
	{
		if (!(iNextSampleB&3))
			REG_SGFIFOB=soundB[iNextSampleB<<2];

		iNextSampleB++;

		if (iNextSampleB>(SampleSizeB/4))
		{
			REG_TM1CNT=0;
		//	REG_DMA1CNT = 0;
		}
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
void play_sfx(const SAMPLE *pSample,short channel)
{
	if (channel==0)
	{

	iNextSampleA=0;
	SampleSizeA=pSample->length;//16hz
	soundA=pSample->pData;

	//REG_SOUNDCNT_H=0x0B0F;

    //start timer 0
   	REG_TM0D   = 0xffff;
    REG_TM0CNT = 0x00C3;

	}
	else
	{
		iNextSampleB=0;
		SampleSizeB=pSample->length;//16hz
		soundB=pSample->pData;

		//REG_SOUNDCNT_H=0x0B0F;

		//start timer 0
		REG_TM1D   = 0xffff;
    	REG_TM1CNT = 0x00C3;
	}
}

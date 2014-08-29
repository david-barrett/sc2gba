//////////////////////////////////////////////////////////////////////////
// File: 	imagedisp.cpp                                           //
// Description: A program that displays an image on the GBA		//
// Author:	gbajunkie (with lots of code from dovoto)	  	//
// Date: 	15th March 2002                                      	//
//////////////////////////////////////////////////////////////////////////
#include "gba.h"        //GBA register definitions
#include "dispcnt.h"    //REG_DISPCNT register definitions
#include "UR2.h"	//holds the image information in an array

void PlotPixel(int x,int y, unsigned short int c)
{
	VideoBuffer[(y) * 120 + (x)] = (c);
}

int main()
{
	int loop;
	int x, y;

        SetMode(MODE_4 | BG2_ENABLE); //set mode 4 and enable background 2

	for(loop = 0; loop < 256; loop++)                 //256 entries allowed
		BGPaletteMem[loop] = UR2Palette[loop]; //load the palette into palette memory

	while(1)
	{
		for(y = 0; y < 160; y++)                  //screen height
		{
			for(x = 0; x < 120; x++)          //screen width
			{
				PlotPixel(x,y, UR2Data[y*120+x]);	//load image data into
			}						//memory pixel by pixel
		}
	}
}



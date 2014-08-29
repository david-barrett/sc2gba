#define BG_C

#include "gba.h"
#include "bg.h"
#include "dispcnt.h"
#include "sincosrad.h"
#include "starfield0.c"
#include "starfield1.c"
#include "stars.h"


extern FIXED COS2[360];
extern FIXED SIN2[360];


extern Bg bg0;
extern Bg bg1;


void EnableBackground(Bg* bg)
{
	u16 temp;

	bg->tileData = (u16*)CharBaseBlock(bg->charBaseBlock);
	bg->mapData = (u16*)ScreenBaseBlock(bg->screenBaseBlock);
	temp = bg->size | (bg->charBaseBlock<<CHAR_SHIFT) | (bg->screenBaseBlock<<SCREEN_SHIFT)
		| bg->colorMode | bg->mosaic | bg->wraparound;

	switch(bg->number)
	{
	case 0:
		{
			REG_BG0CNT = temp;
			REG_DISPCNT |= BG0_ENABLE;
		}break;
	case 1:
		{
			REG_BG1CNT = temp;
			REG_DISPCNT |= BG1_ENABLE;
		}break;
	case 2:
		{
			REG_BG2CNT = temp;
			REG_DISPCNT |= BG2_ENABLE;
		}break;
	case 3:
		{
			REG_BG3CNT = temp;
			REG_DISPCNT |= BG3_ENABLE;
		}break;

	default:break;

	}
}

void UpdateBackground(Bg* bg)
{
	switch(bg->number)
	{
	case 0:
		REG_BG0HOFS = bg->x_scroll;
		REG_BG0VOFS = bg->y_scroll;
		break;
	case 1:
		REG_BG1HOFS = bg->x_scroll;
		REG_BG1VOFS = bg->y_scroll;
		break;
	case 2:
		if(!(REG_DISPCNT & MODE_0))//it is a rot background
		{
			REG_BG2X = bg->DX;
			REG_BG2Y = bg->DY;

			REG_BG2PA = bg->PA;
			REG_BG2PB = bg->PB;
			REG_BG2PC = bg->PC;
			REG_BG2PD = bg->PD;
		}
		else  //it is a text background
		{
			REG_BG2HOFS = bg->x_scroll;
			REG_BG2VOFS = bg->y_scroll;
		}
		break;
	case 3:
		if(!(REG_DISPCNT & MODE_0))//it is a rot background
		{
			REG_BG3X = bg->DX;
			REG_BG3Y = bg->DY;

			REG_BG3PA = bg->PA;
			REG_BG3PB = bg->PB;
			REG_BG3PC = bg->PC;
			REG_BG3PD = bg->PD;
		}
		else //it is a text background
		{
			REG_BG3HOFS = bg->x_scroll;
			REG_BG3VOFS = bg->y_scroll;
		}
		break;
	default: break;
	}
}

void RotateBackground(Bg* bg, int angle,int center_x, int center_y, FIXED zoom)
{

	center_y = (center_y * zoom)>>8;
	center_x = (center_x * zoom)>>8;

	bg->DX = ((bg->x_scroll<<8)-center_y*SIN2[angle]-center_x*COS2[angle]);
	bg->DY = ((bg->y_scroll<<8)-center_y*COS2[angle]+center_x*SIN2[angle]);

	bg->PA = (COS2[angle]*zoom)>>8;  //cos&sin are LUTs that are .8 fixed numbers
	bg->PB = (SIN2[angle]*zoom)>>8;  //zoom is also fixed
	bg->PC = (-SIN2[angle]*zoom)>>8;
	bg->PD = (COS2[angle]*zoom)>>8;
}

void SetupBackground()
{
	//background
	s16 loop;
	u16* temp;

	bg0.number = 0;				//background number 0-3
	bg0.charBaseBlock = 0;                  //tile data position (right at the start of the available memory on 16Kb boundary)
	bg0.screenBaseBlock = 28;		//map data position on 2Kb boundary
	bg0.colorMode = BG_COLOR_256;           //256-colour background
	bg0.size = ROTBG_SIZE_512x512;          //size of map
	bg0.mosaic = 0;                         //not enabled
	bg0.x_scroll = 120;			//scrolling variables
	bg0.y_scroll = 80;
	bg0.wraparound = WRAPAROUND;

	EnableBackground(&bg0);

	bg1.number = 1;				//background number 0-3
	bg1.charBaseBlock = 0;                  //tile data position (right at the start of the available memory on 16Kb boundary)
	bg1.screenBaseBlock = 28;		//map data position on 2Kb boundary
	bg1.colorMode = BG_COLOR_256;           //256-colour background
	bg1.size = ROTBG_SIZE_512x512;          //size of map
	bg1.mosaic = 0;                         //not enabled
	bg1.x_scroll = 120;			//scrolling variables
	bg1.y_scroll = 80;
	bg1.wraparound = WRAPAROUND;

	EnableBackground(&bg1);


    for(loop = 0; loop < 256; loop++)
		BGPaletteMem[loop] = starsPalette[loop];     //load the background palette into memory

	for(loop = 0; loop < 128 * 128 /2; loop++)  //load tile image data
	{
		bg1.tileData[loop] = starsData[loop];
		bg0.tileData[loop] = starsData[loop];
	}

	//load the map image data
	temp = (u16*)starfield0;
	for(loop = 0; loop < 128*128/2; loop++) //128x128 tiles /2 because 16 bit copy
		bg0.mapData[loop] = temp[loop];

	temp = (u16*)starfield1;
	for(loop = 0; loop < 64*64/2; loop++) //64x64 tiles /2 because 16 bit copy
		bg1.mapData[loop] = temp[loop];

}

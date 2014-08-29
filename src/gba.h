//////////////////////////////////////////////////////////////////////////
// File: 	gba.h                                             	//
// Description: Contains all the GBA register definitions	 	//
// Author:	Eloist						  	//
// Date: 	9th January 2002                                      	//
//////////////////////////////////////////////////////////////////////////
#ifndef GBA_HEADER
#define GBA_HEADER

typedef unsigned char 	u8;
typedef unsigned short 	u16;
typedef unsigned long 	u32;

typedef signed char 	s8;
typedef signed short 	s16;
typedef signed long 	s32;

typedef unsigned char 	byte;
typedef unsigned short 	hword;
typedef unsigned long 	word;

#define OAMmem  		((u32*)0x7000000)
#define VideoBuffer 		((u16*)0x6000000)
#define OAMData			((u16*)0x6010000)
#define BGPaletteMem 		((u16*)0x5000000)
#define OBJPaletteMem 		((u16*)0x5000200)
#define FIXED s32
#define PI 3.14159
#define RADIAN(n) 		(((float)n)/(float)180*PI)


#define REG_INTERUPT   *(u32*)0x3007FFC
#define REG_DISPCNT    *(u32*)0x4000000
#define REG_DISPCNT_L  *(u16*)0x4000000
#define REG_DISPCNT_H  *(u16*)0x4000002
#define REG_DISPSTAT   *(u16*)0x4000004
#define REG_VCOUNT     *(u16*)0x4000006
#define REG_BG0CNT     *(u16*)0x4000008
#define REG_BG1CNT     *(u16*)0x400000A
#define REG_BG2CNT     *(u16*)0x400000C
#define REG_BG3CNT     *(u16*)0x400000E
#define REG_BG0HOFS    *(u16*)0x4000010
#define REG_BG0VOFS    *(u16*)0x4000012
#define REG_BG1HOFS    *(u16*)0x4000014
#define REG_BG1VOFS    *(u16*)0x4000016
#define REG_BG2HOFS    *(u16*)0x4000018
#define REG_BG2VOFS    *(u16*)0x400001A
#define REG_BG3HOFS    *(u16*)0x400001C
#define REG_BG3VOFS    *(u16*)0x400001E
#define REG_BG2PA      *(u16*)0x4000020
#define REG_BG2PB      *(u16*)0x4000022
#define REG_BG2PC      *(u16*)0x4000024
#define REG_BG2PD      *(u16*)0x4000026
#define REG_BG2X       *(u32*)0x4000028
#define REG_BG2X_L     *(u16*)0x4000028
#define REG_BG2X_H     *(u16*)0x400002A
#define REG_BG2Y       *(u32*)0x400002C
#define REG_BG2Y_L     *(u16*)0x400002C
#define REG_BG2Y_H     *(u16*)0x400002E
#define REG_BG3PA      *(u16*)0x4000030
#define REG_BG3PB      *(u16*)0x4000032
#define REG_BG3PC      *(u16*)0x4000034
#define REG_BG3PD      *(u16*)0x4000036
#define REG_BG3X       *(u32*)0x4000038
#define REG_BG3X_L     *(u16*)0x4000038
#define REG_BG3X_H     *(u16*)0x400003A
#define REG_BG3Y       *(u32*)0x400003C
#define REG_BG3Y_L     *(u16*)0x400003C
#define REG_BG3Y_H     *(u16*)0x400003E
#define REG_WIN0H      *(u16*)0x4000040
#define REG_WIN1H      *(u16*)0x4000042
#define REG_WIN0V      *(u16*)0x4000044
#define REG_WIN1V      *(u16*)0x4000046
#define REG_WININ      *(u16*)0x4000048
#define REG_WINOUT     *(u16*)0x400004A
#define REG_MOSAIC     *(u32*)0x400004C
#define REG_MOSAIC_L   *(u32*)0x400004C
#define REG_MOSAIC_H   *(u32*)0x400004E
#define REG_BLDMOD     *(u16*)0x4000050
#define REG_COLEV      *(u16*)0x4000052
#define REG_COLEY      *(u16*)0x4000054
#define REG_SG10       *(u32*)0x4000060
#define REG_SG10_L     *(u16*)0x4000060
#define REG_SG10_H     *(u16*)0x4000062
#define REG_SG11       *(u16*)0x4000064
#define REG_SG20       *(u16*)0x4000068
#define REG_SG21       *(u16*)0x400006C
#define REG_SG30       *(u32*)0x4000070
#define REG_SG30_L     *(u16*)0x4000070
#define REG_SG30_H     *(u16*)0x4000072
#define REG_SG31       *(u16*)0x4000074
#define REG_SG40       *(u16*)0x4000078
#define REG_SG41       *(u16*)0x400007C
#define REG_SGCNT0     *(u32*)0x4000080
#define REG_SGCNT0_L   *(u16*)0x4000080
#define REG_SGCNT0_H   *(u16*)0x4000082
#define REG_SGCNT1     *(u16*)0x4000084
#define REG_SGBIAS     *(u16*)0x4000088
#define REG_SGWR0      *(u32*)0x4000090
#define REG_SGWR0_L    *(u16*)0x4000090
#define REG_SGWR0_H    *(u16*)0x4000092
#define REG_SGWR1      *(u32*)0x4000094
#define REG_SGWR1_L    *(u16*)0x4000094
#define REG_SGWR1_H    *(u16*)0x4000096
#define REG_SGWR2      *(u32*)0x4000098
#define REG_SGWR2_L    *(u16*)0x4000098
#define REG_SGWR2_H    *(u16*)0x400009A
#define REG_SGWR3      *(u32*)0x400009C
#define REG_SGWR3_L    *(u16*)0x400009C
#define REG_SGWR3_H    *(u16*)0x400009E
#define REG_SGFIFOA    *(u32*)0x40000A0
#define REG_SGFIFOA_L  *(u16*)0x40000A0
#define REG_SGFIFOA_H  *(u16*)0x40000A2
#define REG_SGFIFOB    *(u32*)0x40000A4
#define REG_SGFIFOB_L  *(u16*)0x40000A4
#define REG_SGFIFOB_H  *(u16*)0x40000A6
#define REG_DM0SAD     *(u32*)0x40000B0
#define REG_DM0SAD_L   *(u16*)0x40000B0
#define REG_DM0SAD_H   *(u16*)0x40000B2
#define REG_DM0DAD     *(u32*)0x40000B4
#define REG_DM0DAD_L   *(u16*)0x40000B4
#define REG_DM0DAD_H   *(u16*)0x40000B6
#define REG_DM0CNT     *(u32*)0x40000B8
#define REG_DM0CNT_L   *(u16*)0x40000B8
#define REG_DM0CNT_H   *(u16*)0x40000BA
#define REG_DM1SAD     *(u32*)0x40000BC
#define REG_DM1SAD_L   *(u16*)0x40000BC
#define REG_DM1SAD_H   *(u16*)0x40000BE
#define REG_DM1DAD     *(u32*)0x40000C0
#define REG_DM1DAD_L   *(u16*)0x40000C0
#define REG_DM1DAD_H   *(u16*)0x40000C2
#define REG_DM1CNT     *(u32*)0x40000C4
#define REG_DM1CNT_L   *(u16*)0x40000C4
#define REG_DM1CNT_H   *(u16*)0x40000C6
#define REG_DM2SAD     *(u32*)0x40000C8
#define REG_DM2SAD_L   *(u16*)0x40000C8
#define REG_DM2SAD_H   *(u16*)0x40000CA
#define REG_DM2DAD     *(u32*)0x40000CC
#define REG_DM2DAD_L   *(u16*)0x40000CC
#define REG_DM2DAD_H   *(u16*)0x40000CE
#define REG_DM2CNT     *(u32*)0x40000D0
#define REG_DM2CNT_L   *(u16*)0x40000D0
#define REG_DM2CNT_H   *(u16*)0x40000D2
#define REG_DM3SAD     *(u32*)0x40000D4
#define REG_DM3SAD_L   *(u16*)0x40000D4
#define REG_DM3SAD_H   *(u16*)0x40000D6
#define REG_DM3DAD     *(u32*)0x40000D8
#define REG_DM3DAD_L   *(u16*)0x40000D8
#define REG_DM3DAD_H   *(u16*)0x40000DA
#define REG_DM3CNT     *(u32*)0x40000DC
#define REG_DM3CNT_L   *(u16*)0x40000DC
#define REG_DM3CNT_H   *(u16*)0x40000DE
#define REG_TM0D       *(u16*)0x4000100
#define REG_TM0CNT     *(u16*)0x4000102
#define REG_TM1D       *(u16*)0x4000104
#define REG_TM1CNT     *(u16*)0x4000106
#define REG_TM2D       *(u16*)0x4000108
#define REG_TM2CNT     *(u16*)0x400010A
#define REG_TM3D       *(u16*)0x400010C
#define REG_TM3CNT     *(u16*)0x400010E
#define REG_SCD0       *(u16*)0x4000120
#define REG_SCD1       *(u16*)0x4000122
#define REG_SCD2       *(u16*)0x4000124
#define REG_SCD3       *(u16*)0x4000126
#define REG_SCCNT      *(u32*)0x4000128
#define REG_SCCNT_L    *(u16*)0x4000128
#define REG_SCCNT_H    *(u16*)0x400012A
#define REG_P1         *(u16*)0x4000130
#define REG_P1CNT      *(u16*)0x4000132
#define REG_R          *(u16*)0x4000134
#define REG_HS_CTRL    *(u16*)0x4000140
#define REG_JOYRE      *(u32*)0x4000150
#define REG_JOYRE_L    *(u16*)0x4000150
#define REG_JOYRE_H    *(u16*)0x4000152
#define REG_JOYTR      *(u32*)0x4000154
#define REG_JOYTR_L    *(u16*)0x4000154
#define REG_JOYTR_H    *(u16*)0x4000156
#define REG_JSTAT      *(u32*)0x4000158
#define REG_JSTAT_L    *(u16*)0x4000158
#define REG_JSTAT_H    *(u16*)0x400015A
#define REG_IE         *(u16*)0x4000200
#define REG_IF         *(u16*)0x4000202
#define REG_WSCNT      *(u16*)0x4000204
#define REG_IME        *(u16*)0x4000208
#define REG_PAUSE      *(u16*)0x4000300

#define REG_COLV		*(u16*)0x4000052

/***************************************
NEW added for sound
**************************************/
// sound hardware defines
#define REG_SOUND1CNT_L *(u16*)0x04000060       // sound 1 sweep control
#define REG_SOUND1CNT_H *(u16*)0x04000062       // sound 1 length, wave duty and envelope control
#define REG_SOUND1CNT_X *(u16*)0x04000064       // sound 1 frequency, reset and loop control
#define REG_SOUND2CNT_L *(u16*)0x04000068       // sound 2 length, wave duty and envelope control
#define REG_SOUND2CNT_H *(u16*)0x0400006C       // sound 2 frequency, reset and loop control
#define REG_SOUND3CNT_L *(u16*)0x04000070       // sound 3 enable and wave ram bank control
#define REG_SOUND3CNT_H *(u16*)0x04000072       // sound 3 sound length and output level control
#define REG_SOUND3CNT_X *(u16*)0x04000074       // sound 3 frequency, reset and loop control
#define REG_SOUND4CNT_L *(u16*)0x04000078       // sound 4 length, output level and envelope control
#define REG_SOUND4CNT_H *(u16*)0x0400007C       // sound 4 noise parameters, reset and loop control
#define REG_SOUNDCNT_L  *(u16*)0x04000080       // sound 1-4 output level and stereo control
#define REG_SOUNDCNT_H  *(u16*)0x04000082       // direct sound control and sound 1-4 output ratio
#define REG_SOUNDCNT_X  *(u32*)0x04000084       // master sound enable and sound 1-4 play status
#define REG_SOUNDBIAS   *(u32*)0x04000088       // sound bias and amplitude resolution control
#define REG_WAVE_RAM0_L *(u16*)0x04000090       // sound 3 samples 0-3
#define REG_WAVE_RAM0_H *(u16*)0x04000092       // sound 3 samples 4-7
#define REG_WAVE_RAM1_L *(u16*)0x04000094       // sound 3 samples 8-11
#define REG_WAVE_RAM1_H *(u16*)0x04000096       // sound 3 samples 12-15
#define REG_WAVE_RAM2_L *(u16*)0x04000098       // sound 3 samples 16-19
#define REG_WAVE_RAM2_H *(u16*)0x0400009A       // sound 3 samples 20-23
#define REG_WAVE_RAM3_L *(u16*)0x0400009C       // sound 3 samples 23-27
#define REG_WAVE_RAM3_H *(u16*)0x0400009E       // sound 3 samples 28-31
#define REG_FIFO_A      0x040000A0              // direct sound channel A samples 0-3
#define REG_FIFO_A_L    0x040000A0              // direct sound channel A samples 0-1
#define REG_FIFO_A_H    0x040000A2              // direct sound channel A samples 2-3
#define REG_FIFO_B      0x040000A4              // direct sound channel B samples 0-3
#define REG_FIFO_B_L    0x040000A4              // direct sound channel B samples 0-1
#define REG_FIFO_B_H    0x040000A6              // direct sound channel B samples 2-3

// timer defines
#define REG_TM0D        *(u16*)0x4000100
#define REG_TM0CNT      *(u16*)0x4000102
#define REG_TM1D        *(u16*)0x4000104
#define REG_TM1CNT      *(u16*)0x4000106
#define REG_TM2D        *(u16*)0x4000108
#define REG_TM2CNT      *(u16*)0x400010A
#define REG_TM3D        *(u16*)0x400010C
#define REG_TM3CNT      *(u16*)0x400010E
#define TIMER_ENABLED   0x0080

// DMA defines
#define REG_DMA0SAD          *(u32*)0x40000B0
#define REG_DMA0DAD          *(u32*)0x40000B4
#define REG_DMA0CNT          *(u32*)0x40000B8
#define REG_DMA1SAD          *(u32*)0x40000BC
#define REG_DMA1DAD          *(u32*)0x40000C0
#define REG_DMA1CNT          *(u32*)0x40000C4
#define REG_DMA2SAD          *(u32*)0x40000C8
#define REG_DMA2DAD          *(u32*)0x40000CC
#define REG_DMA2CNT          *(u32*)0x40000D0
#define REG_DMA3SAD          *(u32*)0x40000D4
#define REG_DMA3DAD          *(u32*)0x40000D8
#define REG_DMA3CNT          *(u32*)0x40000DC
#define WORD_DMA             0x04000000
#define HALF_WORD_DMA        0x00000000
#define ENABLE_DMA           0x80000000
#define START_ON_FIFO_EMPTY  0x30000000
#define DMA_REPEAT           0x02000000
#define DEST_REG_SAME        0x00400000
#define DMA_CHANNEL_0        0x0
#define DMA_CHANNEL_1        0x1
#define DMA_CHANNEL_2        0x2
#define DMA_CHANNEL_3        0x3
#define DMA_CHANNEL_DEFAULT  0x3

// background defines
#define REG_BG0CNT     *(u16*)0x4000008
#define REG_BG1CNT     *(u16*)0x400000A
#define REG_BG2CNT     *(u16*)0x400000C
#define REG_BG3CNT     *(u16*)0x400000E
#define REG_BG0HOFS    *(u16*)0x4000010
#define REG_BG0VOFS    *(u16*)0x4000012
#define REG_BG1HOFS    *(u16*)0x4000014
#define REG_BG1VOFS    *(u16*)0x4000016
#define REG_BG2HOFS    *(u16*)0x4000018
#define REG_BG2VOFS    *(u16*)0x400001A
#define REG_BG3HOFS    *(u16*)0x400001C
#define REG_BG3VOFS    *(u16*)0x400001E
#define REG_BG2PA      *(u16*)0x4000020
#define REG_BG2PB      *(u16*)0x4000022
#define REG_BG2PC      *(u16*)0x4000024
#define REG_BG2PD      *(u16*)0x4000026
#define REG_BG2X       *(u32*)0x4000028
#define REG_BG2Y       *(u32*)0x400002C
#define REG_BG3PA      *(u16*)0x4000030
#define REG_BG3PB      *(u16*)0x4000032
#define REG_BG3PC      *(u16*)0x4000034
#define REG_BG3PD      *(u16*)0x4000036
#define REG_BG3X       *(u32*)0x4000038
#define REG_BG3Y       *(u32*)0x400003C

// input defines
#define REG_KEYS       *(volatile u16*)0x4000130

// mosaic defines
#define REG_MOSAIC     *(u32*)0x400004C
#define REG_MOSAIC_L   *(u32*)0x400004C
#define REG_MOSAIC_H   *(u32*)0x400004E

#define SCREEN_HEIGHT        160
#define SCANLINE_COUNTER           (*(volatile u16*)0x4000006)

#define DMA_16NOW ENABLE_DMA | HALF_WORD_DMA


#endif

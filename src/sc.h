////////////////////////////////////////////////////////////////////////
// File: 	ch4.h                                              //
// Description: contains all the definitions and structures necessary //
//		for displaying sprites on the GBA		      //
// Author:	dovoto (modified by gbajunkie)	                      //
// Date: 	7th February 2002                                     //
////////////////////////////////////////////////////////////////////////
#ifndef SC2_H
#define SC2_H
#include "bg.h"

//define the screen width and height values to be used
#define SCREEN_WIDTH	240
#define SCREEN_HEIGHT   160

//colour convertion (converts a RGB colour to a 15-bit BGR value used by the GBA)
#define RGB16_BGR(r, g, b) ((r)+(g<<5)+(b<<10))

//attribute0 #defines
#define ROTATION_FLAG		0x100
#define SIZE_DOUBLE		0x200
#define MODE_NORMAL		0x0
#define MODE_TRANSPARENT	0x400
#define MODE_WINDOWED		0x800
#define MOSAIC			0x1000
#define COLOR_16		0x0000
#define COLOR_256		0x2000
#define SQUARE			0x0
#define TALL			0x4000
#define WIDE			0x8000

//attribute1 #defines
#define ROTDATA(n)		((n)<<9)
#define HORIZONTAL_FLIP		0x1000
#define VERTICAL_FLIP		0x2000
#define SIZE_8			0x0
#define SIZE_16			0x4000
#define SIZE_32			0x8000
#define SIZE_64			0xC000

//atrribute2 #defines
#define PRIORITY(n)	        ((n)<<10)
#define PALETTE(n)		((n)<<12)

#define SPEED_REDUCT  3

//sprite structure definitions
typedef struct tagOAMEntry
{
	u16 attribute0;
	u16 attribute1;
	u16 attribute2;
	u16 attribute3;
}OAMEntry, *pOAMEntry;

//sprite rotation information (don't worry about this for now)
typedef struct tagRotData
{
	u16 filler1[3];
	u16 pa;
	u16 filler2[3];
	u16 pb;
	u16 filler3[3];
	u16 pc;
	u16 filler4[3];
	u16 pd;
}RotData, *pRotData;

typedef struct Weapon
{
s32 xpos ;
s32 ypos ;
s16 xscreen;
s16 yscreen;
s32 angle;
s32 xspeed;
s32 yspeed;
s32 turn;
s32 speed;

s16 sprite;
s16 life;

s16 damage;
s16 damageparent;
s16 type;
s16 who; //0 = both 1=pl1,2=pl2

s16 size;

s16 status;//ie fighter mission; podship weapon size;trader weapon etc

void* parent;
void* target;

}Weapon, *pWeapon;


typedef struct Player
{
s32 xpos ;
s32 ypos ;
s16 xscreen;
s16 yscreen;
s32 angle;
s32 thrustangle;

s32 xspeed;
s32 yspeed;

s16 ship;

s16 crew;
s16 maxcrew;
s16 batt;
s16 maxbatt;

s16 accesst;
s16 speed;
s16 maxspeed;

s16 accel_inc;
s16 firebatt;
s16 specbatt;
s16 plr;

s16 batt_wait;
s16 turn_wait;
s16 thrust_wait;
s16 weapon_wait;
s16 special_wait;

s16 batt_turn;
s16 turn_turn;
s16 thrust_turn;
s16 weapon_turn;
s16 special_turn;


Weapon weapon[15];
s16 ffiresprite;
s16 lfiresprite;
s16 fspecsprite;
s16 lspecsprite;

s16 offset;


s16 SpriteStart;
s16 OAMStart;

//void* opp;

//pPlayer opp;
}Player, *pPlayer;


//crew/fuel/fuel regen/accel/turn rate/max vel/weapon range/weapon damage - max 9
const s16 AVATAR =0; //chmmr avatar 9/9/9/3/5/7/4/9 - lazer + tractor beam
const s16 AVENGER =1; //ilwarth avenger 5/3/5/8/7/4/3/4 flame + cloak
const s16 BLADE =2; //supox blade 3/4/5/8/8/8/5/1 gun + reverse
const s16 BROODHOME =3; //chenjesu broodhome 8/6/6/3/3/5/9/7 cyrstal range var + recover?
const s16 CRUISER =4; //earthling cruiser 5/5/3/2/9/4/9/4 homing missing+lazer defense
const s16 DREADNAUGHT =5; //ur-quan dreadnaught 9/9/8/4/4/5/9/9 fusion cannon + homing fighters
const s16 DRONE =6; // umgah drone 2/6/6/5/5/1/2/5 anti matter + reverse
const s16 ELUDER =7; // spathi eluder 6/2/1/8/8/9/5/2 gun + homing missile rear
const s16 FURY =8; // pkunk fury - 1/3/2/8/9/9/3/3 - 3 way gun + curse (resurection)
const s16 GUARDIAN =9;//androsyth guardian 4/5/3/6-9,4-9,3-9,5,4 bubble + blazer form
const s16 INTRUDER =10; //VUX INTRUDER 6/9/6/2/1/1/4/8 lazer + limpit cocoons
const s16 JUGGER =11; //UTWIG JUGGER 4/2-4/0-4/4/8/8/5/6 + lazer + shield (inc energy)
const s16 MARAUDER =12;//KOHR-AH MARAUDER 9/9/8/4/4/5/9/4 mines (var range) + fire defense
const s16 MAULER =13; // druuge mauler 4/6/1/1/4/1/9/6 high reoil cannon + furnice (kill crew for en)
const s16 NEMESIS =14; // orz nemesis 4/4/5/4/8/6/7/3 cannon (rotate) + marines
const s16 PENETRATOR =15; //syreen penetrator 3-9/3/4/8/6/6/5/2 + gun + call emeny crew
const s16 PODSHIP =16; //mycon podship 4/8/7/4/2/5/8/1-9 -homing plasmoid + crew growth
const s16 PROBE =17;//slylandro probe 3/4/0-2/9/9/9/4/4 - lightnig + asteroid consumption
const s16 SCOUT =18; // shofixti scout 1/2/4/8/7/7/5/1 - gun + self destruct
const s16 SKIFF =19 ;//ariloulalleelay skiff 1/4/5/9/9/8/2/1 auto target lazer + hyperspace
const s16 STINGER =20;//zoq-fot-pik stinger 2/2/6/7/8/8/3/2 spary + tongue attack
const s16 TERMINATOR =21; //yehat terminator 4/4/7/5/7/6/5/7 gun + shield
const s16 TORCH =22; //tharddash torch 1/5/5/5/8/5/6/1  missile (home?) + flame trail
const s16 TRADER =23; //melnorme trader 4/9/6/4/4/6/9/2-9 weapon var range + power + disable?
const s16 TRANSFORMER =24;//mmrnmhrm transformer 4/2/8/6/7/1/1/6 + 4/2/5/9/1/9/9/2 lazer/missile + transform

const s16 SIMPLE=0;
const s16 UR_FIGHTERS=1;
const s16 EXP=2;



//OAM memory sprite = oam/16
const s16 P1_OAMStart =0;
const s16 P1_SpriteStart =0;

const s16 P2_OAMStart =2048;
const s16 P2_SpriteStart =128;

const s16 OAMFireSprite1=4096;
const s16 FireSprite1 = 256;

const s16 OAMStatusStart = 5120;//rand no
const s16 SpriteStatusStart = 320;

void Thrust(pPlayer plr);
void LoadShips();
void SetupBackground();
void MoveSprite(OAMEntry* sp, int x, int y);
void UpdateStatus();
void SetupStatus();
void Fire(pPlayer pl);
void Special(pPlayer pl);
void drawOnScreen(s16* x,s16* y,s32 xpos,s32 ypos, s16 screenx, s16 yscreen,s16 size);
//int can(s16 rate);
void SetShip(pPlayer pl);

void CopyOAM();
void InitializeSprites();
void WaitForVsync();

void Melee();
void SetNew(pPlayer pl);
void LoadExp(s16 OAMStart, s16 SpriteStart);
#endif

/*128 sprites first 32 rotate
0=p1
1-12 = p1 weapons/special/divided as nec 12 + explos
13=p2
14-25 p2 weapons/special/divided as nec + expl
26-28 = 3 asteroids
29=planet
30,31 ??

71-82=p1 batt/crew 6 each
83-94=p2 batt/crew
95-106=p1 status box
107-118=p2 status box
119-120=p1 spacemen
121-122=p1 vux dots
123-124=p2 spacemen
125-126=p2 vux dots

127 spare
*/





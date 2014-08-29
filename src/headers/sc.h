////////////////////////////////////////////////////////////////////////
// File: 	ch4.h                                              //
// Description: contains all the definitions and structures necessary //
//		for displaying sprites on the GBA		      //
// Author:	dovoto (modified by gbajunkie)	                      //
// Date: 	7th February 2002                                     //
////////////////////////////////////////////////////////////////////////
#ifndef SC2_H
#define SC2_H
#include "gba.h"
#include "bg.h"
#include "keypad.h"     //button registers
#include "shipgfx.h"
#include "sfx.h"
#include "TektronicWave.h"

//#define DEBUG 1

//#define MISSILE_START 1


//#include "gfx/gfx.h"
//#include "gfx/gfx_symbols.h"

#define ACTIVE 1
#define DESTROYED 2
#define EMPTY 3

#define SHIP_SPEED(x) ((x)>>1) //try



typedef signed short 	COUNT;
/* heat of battle specific flags */
#define LEFT                   (1 << 0)
#define RIGHT                  (1 << 1)
#define THRUST                 (1 << 2)
#define WEAPON                 (1 << 3)
#define SPECIAL                (1 << 4)
#define LOW_ON_ENERGY          (1 << 5)
#define SHIP_BEYOND_MAX_SPEED  (1 << 6)
#define SHIP_AT_MAX_SPEED      (1 << 7)
#define SHIP_IN_GRAVITY_WELL   (1 << 8)
#define PLAY_VICTORY_DITTY     (1 << 9)

#define SEEKING_WEAPON    (1 << 2)
#define SEEKING_SPECIAL   (1 << 3)
#define POINT_DEFENSE     (1 << 4)
#define IMMEDIATE_WEAPON  (1 << 5)
#define CREW_IMMUNE       (1 << 6)
#define FIRES_FORE        (1 << 7)
#define FIRES_RIGHT       (1 << 8)
#define FIRES_AFT         (1 << 9)
#define FIRES_LEFT        (1 << 10)
#define SHIELD_DEFENSE    (1 << 11)
#define DONT_CHASE        (1 << 12)

#define FAST_SHIP 150
#define MEDIUM_SHIP 45
#define SLOW_SHIP 25
#define CLOSE_RANGE_WEAPON 50
#define LONG_RANGE_WEAPON 1000

typedef enum
{
	PURSUE = 0,
	AVOID,
	ENTICE,
	NO_MOVEMENT
} MOVEMENT_STATE;



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

#define RADIAN(n) 		(((float)n)/(float)180*PI)

#define DEFY_PHYSICS (1 << 9)
#define COLLISION (1 << 7)


#define COLLISION_TURN_WAIT 1
#define COLLISION_THRUST_WAIT 3

#define MAX_SHIP_MASS 10
#define GRAVITY_MASS(m) ((m) > MAX_SHIP_MASS * 10)



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

typedef struct PlrList
{
	s16 ship;
	s16 active;
}PlrList, *pPlrList;

typedef struct Object
{
s32 xpos ;
s32 ypos ;
s16 xscreen;
s16 yscreen;
s32 angle;
s8 actualangle;

s32 lastxpos;
s32 lastypos;

s32 xspeed;
s32 yspeed;

s16 type;
void* parent;

s16 which_turn;
s16 MoveState;

s16 life;
s16 size;

s8 ignorecollision;
s8 state_flags;

s8 mass_points;

}Object,*pObject;

typedef struct Weapon
{
Object object;

s32 turn;
s32 speed;

s16 sprite;

s16 damage;
s16 damageparent;
s16 type;
s16 who; //0 = both 1=pl1,2=pl2

s16 size;

s16 status;//ie fighter mission; podship weapon size;trader weapon etc

void* parent;
void* target;

s8 turn_wait;
s8 fire_wait;

void (*movefunc)(Weapon*);
void (*hitfunc)(Weapon*,void*);

}Weapon, *pWeapon;

typedef struct Trail
{
Object object;

s16 sprite;
s16 life;

}Trail, *pTrail;

typedef struct Asteroid
{
Object object;


s16 sprite;



}Asteroid, *pAsteroid;


enum skill
{
	PLAYER1=1,
	PLAYER2=2,
	AWESOME=3,
	GOOD=4,
	WEAK=5,
	DISABLED=6
};




typedef struct Pilots
{
	s8 x;
	s8 y;
}Pilots,pPilots;


typedef struct Player
{
Object object;

s32 thrustangle;

s16 ship;

s16 crew;
s16 maxcrew;
s16 batt;
s16 maxbatt;

s16 accesst;
s16 speed;
s16 maxspeed;
s16 thrustspeed;

s16 accel_inc;
s16 firebatt;
s16 specbatt;
s16 plr;
s16 batt_regen;

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

s8 warp;

s16 SpriteStart;
//s16 OAMStart;

void* opp;

skill ai;
s8 aiturn;
s8 aispecial;

s16 ManeuverabilityIndex;
s8 ship_input_state;
s8 ship_flags;



s16 range;

s8 limpets;

//s16 fireangle;

short reinc;//pkunk
short shield;//yehat/utwig
short cloak;//ilwarth
short scrambled;//trader weapon hit
short blaze;//andro
short destruct;//shofixti
short form; //mmrnhrm
short charging; //chenjesu/trader
short drawangle;//probe
short currentweapon;//blackurq
s8 turretangle;//orz
s8 marines;//num of marines attached;

//pPlayer opp;

s16 spriteoffset;
s16 angleoffset;

//functions
int (*firefunc)(Player*);
int (*specfunc)(Player*);
int (*aifunc)(Player*, pObject,COUNT);
void (*loadfunc)(s16);
void (*loadpilots)(Player*);
void (*postfunc)(Player*);
void (*restorefunc)(Player*);

const PCMSOUND *ditty;

s32 ship_input_flags;

s16 pilot_sprite;

s8 EndGame;


Pilots pilots[5];

Object *objlist[19];
}Player, *pPlayer;


//crew/fuel/fuel regen/accel/turn rate/max vel/weapon range/weapon damage - max 9
#define GUARDIAN 0//androsyth guardian 4/5/3/6-9,4-9,3-9,5,4 bubble + blazer form
#define SKIFF 1 //ariloulalleelay skiff 1/4/5/9/9/8/2/1 auto target lazer + hyperspace
#define MARAUDER 2//KOHR-AH MARAUDER 9/9/8/4/4/5/9/4 mines (var range) + fire defense
#define BROODHOME 3 //chenjesu broodhome 8/6/6/3/3/5/9/7 cyrstal range var + recover?
#define AVATAR 4 //chmmr avatar 9/9/9/3/5/7/4/9 - lazer + tractor beam
#define AVENGER 7 //ilwarth avenger 5/3/5/8/7/4/3/4 flame + cloak
#define BLADE 16 //supox blade 3/4/5/8/8/8/5/1 gun + reverse

#define CRUISER 6 //earthling cruiser 5/5/3/2/9/4/9/4 homing missing+lazer defense
#define DREADNAUGHT 20 //ur-quan dreadnaught 9/9/8/4/4/5/9/9 fusion cannon + homing fighters
#define DRONE 19 // umgah drone 2/6/6/5/5/1/2/5 anti matter + reverse
#define ELUDER 15 // spathi eluder 6/2/1/8/8/9/5/2 gun + homing missile rear
#define FURY 12 // pkunk fury - 1/3/2/8/9/9/3/3 - 3 way gun + curse (resurection)

#define INTRUDER  22 //VUX INTRUDER 6/9/6/2/1/1/4/8 lazer + limpit cocoons
#define JUGGER  21 //UTWIG JUGGER 4/2-4/0-4/4/8/8/5/6 + lazer + shield (inc energy)

#define MAULER  5 // druuge mauler 4/6/1/1/4/1/9/6 high reoil cannon + furnice (kill crew for en)
#define NEMESIS  11 // orz nemesis 4/4/5/4/8/6/7/3 cannon (rotate) + marines
#define PENETRATOR  17 //syreen penetrator 3-9/3/4/8/6/6/5/2 + gun + call emeny crew
#define PODSHIP  10 //mycon podship 4/8/7/4/2/5/8/1-9 -homing plasmoid + crew growth
#define PROBE  13//slylandro probe 3/4/0-2/9/9/9/4/4 - lightnig + asteroid consumption
#define SCOUT  14 // shofixti scout 1/2/4/8/7/7/5/1 - gun + self destruct

#define STINGER  24//zoq-fot-pik stinger 2/2/6/7/8/8/3/2 spary + tongue attack
#define TERMINATOR  23 //yehat terminator 4/4/7/5/7/6/5/7 gun + shield
#define TORCH  18 //tharddash torch 1/5/5/5/8/5/6/1  missile (home?) + flame trail
#define TRADER  8 //melnorme trader 4/9/6/4/4/6/9/2-9 weapon var range + power + disable?
#define TRANSFORMER  9//mmrnmhrm transformer 4/2/8/6/7/1/1/6 + 4/2/5/9/1/9/9/2 lazer/missile + transform


enum type
{
	 PLAYER=0,
	ASTEROID,
	PLANET,
	TRAIL,
	EXP,
	SIMPLE,
	UR_FIGHTERS,
	UR_FIGHTERS_FIRE,
	ILWRATHFIRE,
	CREW,
	LASER,
	LIMPET,
	BUTT,
	BUBBLE,
	MISSILE,
	DOGI,
	FLAME,
	CONFUSE,
	SAT,
	TURRET,
	MARINE,
	BLADES,
	RECOIL

};


//1022 seems to be the last
//game
#define P1_SpriteStart  0
#define P2_SpriteStart  340
#define  PauseSpriteStart 680 //size ?
#define SpriteAsteroidStart 744  //size 64
#define FireSprite1   808//size 28
#define TrailSprite   836//size =2
#define PlanetSprite 838//128
#define SpriteStatusStart   966  //size 54


//menus
#define SpriteTitleStart   640
#define SpriteAllShips   0
#define SpriteLettersStart 950


void Thrust(pPlayer plr);
void LoadShip(pPlayer);
void LoadPal();
void SetupBackground(pBg,pBg);
void MoveSprite(OAMEntry* sp, int x, int y);
//void UpdateStatus();
void SetupStatus(pPlayer,pPlayer);
void UpdateCrew(pPlayer);
void UpdateBatt(pPlayer);
void Fire(pPlayer pl);
int Special(pPlayer pl);
void drawOnScreen(s16* x,s16* y,s32 xpos,s32 ypos, s16 screenx, s16 yscreen,s16 size,s16 pl=0);
//int can(s16 rate);
void SetShip(pPlayer pl);

void CopyOAM();
void InitializeSprites();
void WaitForVsync();

void Melee(pPlayer ,pPlayer,pBg,pBg);
void SetNew(pPlayer pl);
void LoadExp(s16 OAMStart);
void LoadTrail(s16 OAMStart);
void LoadPlanet(s16 OAMStart);
void LoadBattle();



void CreateTrail(pPlayer);
int FightersFire(pWeapon f,s16 angle);
int nextWeapon(pPlayer pl,int f=0,int l=11);

//status.c
void ModifyCrew(pPlayer,int,int ignoreshield=0);
void ModifyBatt(pPlayer,int);
void AddLimpet(pPlayer);



//ships.cpp
void TurnLeft(pPlayer);//,int i=15);
void TurnRight(pPlayer);//,int i=15);
void Thrust(pPlayer);
void Bounce(pPlayer pl);
void ProcessPlayer(pPlayer pl);
void Regen(pPlayer pl);
void DrawTrails();


const s32 screencentrex=3000;
const s32 screencentrey=3000;
const s32  centrex=120;
const s32  centrey=80;
void GenerateStart(pPlayer p,s32 range=1180);
void MoveOffscreen(OAMEntry* sp);

//planets.c
void LoadAsteroid(s16 OAMStart);
void ProcessAsteroids(pPlayer p1,pPlayer p2);
void DrawPlanet();
void CalcPlanet(pPlayer pl);

//melee.cpp
void CreateExplosion(pWeapon w,s16 life=5);
int ran(int min, int max);
int ModifyAngle(s16 a,int o);
s32 distanceBetweenPoints(s32 xpos1,s32 ypos1,s32 xpos2,s32 ypos2);
int TurnAngle(s16 yourangle, s16 desiredangle,s8 );
void LoadDreadnaught(s16 OAMStart, s16 SpriteStart);
s16 FindAngle(s32 x,s32 y, s32 ox, s32 oy);

#ifdef DEBUG
//debug
void print(char *s);
void print(s32 s);
void print(char *s);
#endif

int InRange(s32 xpos,s32 ypos,s32 txpos,s32 typos,s16 range);
int DetectWeaponToShip(pPlayer p,pWeapon w);

//sfx.cpp
void InterruptProcess(void);

//demo.cpp
void Demo(pPlayer p1,pPlayer p2,pBg bg0,pBg bg1);

//menu.cpp
void menu(pPlayer p1, pPlayer p2);
void DrawLargeNumber(int num,s16 sprite, s16 x, s16 y,s16 digits);
void DrawNumber(int num,s16 sprite,  s16 x, s16 y);
void DrawLetter(char letter,s16 sprite,  s16 x, s16 y);
void LoadLetters(s16 spriteStart);
void DrawTitle();

//choose.cpp
int ChooseNextShip(pPlayer pl, pPlrList list);
int DrawWinner(pPlrList list, pPlrList list2);
int DrawRemainingShips(pPlayer pl,pPlrList list,int sx,int sy);
int ChooseNextShipRand(pPlrList list);
int score(int ship);
void ChooseShips(pPlayer pl, pPlrList list);

//hope to some of these later

//fury
int DeathFury(pPlayer pl);
void SetFury(pPlayer pl);

//urquan
void SetDreadnaught(pPlayer pl);
void SetYehat(pPlayer pl);
void SetIlwrath(pPlayer pl);
void SetVux(pPlayer pl);
void SetSyreen(pPlayer pl);
void SetAndrosynth(pPlayer pl);
void SetArilou(pPlayer pl);
void SetBlackurq(pPlayer pl);
void SetChenjesu(pPlayer pl);
void SetChmmr(pPlayer pl);
void SetSupox(pPlayer pl);
void SetHuman(pPlayer pl);
void SetUmgah(pPlayer pl);
void SetSpathi(pPlayer pl);
void MoveButt(pWeapon);
void SetUtwig(pPlayer pl);
void SetDruuge(pPlayer pl);
void SetOrz(pPlayer pl);

void SetMycon(pPlayer pl);
void SetProbe(pPlayer pl);
void SetShofixti(pPlayer pl);
void SetZoqFot(pPlayer pl);
void SetThradd(pPlayer pl);
void SetMelnorme(pPlayer pl);
void SetMmrnmhrm(pPlayer pl);


//remove?
//void PursueShip (pPlayer ShipPtr, pPlayer enemyShipPtr);
//void Entice(pPlayer);
//void ship_intelligence (pPlayer ShipPtr);
void ship_intelligence (pPlayer ShipPtr, pObject ObjectsOfConcern, COUNT ConcernCounter);
s16 tactical_intelligence (pPlayer StarShipPtr );
int InRange(s32 xpos,s32 ypos,s32 txpos,s32 typos,s16 range);
COUNT PlotIntercept (pPlayer ElementPtr0, pObject ElementPtr1, COUNT max_turns, COUNT margin_of_error);

//intel
enum
{
	ENEMY_SHIP_INDEX = 0,
	CREW_OBJECT_INDEX,
	ENEMY_WEAPON_INDEX,
	GRAVITY_MASS_INDEX,
	FIRST_EMPTY_INDEX
};

//tmp



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





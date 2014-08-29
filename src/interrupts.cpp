#define INTERUPT_C

#include "gba.h"
//#include "headers/dma.h"
//#include "headers/sprite.h"
#include "interupts.h"

extern fp intr_main;
//these are my function definitios to let the c compiler know what I am talking about

void VBLANK(void) ;
void HBLANK(void) ;
void DMA0(void) ;
void DMA1(void) ;
void DMA2(void) ;
void DMA3(void) ;
void VCOUNT(void) ;
void TIMMER0(void) ;
void TIMMER1(void) ;
void TIMMER2(void) ;
void TIMMER3(void) ;
void KEYBOARD(void) ;
void CART(void) ;
void COMUNICATION(void) ;


//here is a function to enable interupts.  If it is VBLANK or HBLANK it sets REG_DISPSTAT to the apropriate values
//all others you will have to do on your own.

void EnableInterupts(u16 interupts)
{
	REG_IME = 0;  //probably not necessary but not a good idea to change interupt registers when an interupt is ocuring


	if(interupts == INT_VBLANK)
		REG_DISPSTAT |= 0x8;

	if(interupts == INT_HBLANK)
		REG_DISPSTAT |= 0x10;


	REG_IE |= interupts;

	REG_IME = 1;

}

void DissableInterupts(u16 interupts)
{
	REG_IE &= ~interupts;

	if(interupts == INT_ALL) //this is were that ALL comes in
		REG_IME = 0;  //disable all interupts;
}




//here is our table of function pointers.  I added the definition of fp to gba.h because I figure it would be usefull
//to have elsewhere
//typedef void (*fp)(void);   this is the definition you will find in gba.h.  It is just delaring fp to represent a pointer
//to a function


//here is our table of functions.  Once interupts are enabled in crt0.s you will not be able to link your code if this table is not pressent.
//the interupts must be listed in that order.

fp IntrTable[]  =
{
	VBLANK,
	HBLANK,
	VCOUNT,
	TIMMER0,
	TIMMER1,
	TIMMER2,
	TIMMER3,
	COMUNICATION,
	DMA0,
	DMA1,
	DMA2,
	DMA3,
	KEYBOARD,
	CART
};



//all our interupt functions empty for now.

void VBLANK()
{

	REG_IF |= INT_VBLANK;

}
void HBLANK(void)
{

	REG_IF |= INT_HBLANK;
}
void DMA0(void)
{

	REG_IF |= INT_DMA0;

}
void DMA1(void)
{

	REG_IF |= INT_DMA1;

}
void DMA2(void)
{

	REG_IF |= INT_DMA2;

}
void DMA3(void)
{
	REG_IF |= INT_DMA3;

}
void VCOUNT(void)
{
	REG_IF |= INT_VCOUNT;

}
void TIMMER0(void)
{
	REG_IF |= INT_TIMMER0;

}
void TIMMER1(void)
{
	REG_IF |= INT_TIMMER1;

}
void TIMMER2(void)
{
	REG_IF |= INT_TIMMER2;

}
void TIMMER3(void)
{
	REG_IF |= INT_TIMMER3;

}
void KEYBOARD(void)
{
	REG_IF |= INT_TIMMER3;

}
void CART(void)
{
	REG_IF |= INT_CART;

}
void COMUNICATION(void)
{
	REG_IF |= INT_COMUNICATION;

}



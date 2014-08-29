;; defines for interrupts taken from ThePernProject.com.
;; go there for C GBA tutorials. 

@define INT_VBLANK       0x0001
@define INT_HBLANK       0x0002 
@define INT_VCOUNT       0x0004 ;;you can set the display to generate an interrupt when it reaches a particular line on the screen
@define INT_TIMER0       0x0008
@define INT_TIMER1       0x0010
@define INT_TIMER2       0x0020 
@define INT_TIMER3       0x0040
@define INT_COMUNICATION 0x0080 ;;serial communication interupt
@define INT_DMA0         0x0100
@define INT_DMA1         0x0200
@define INT_DMA2         0x0400
@define INT_DMA3         0x0800
@define INT_KEYBOARD     0x1000
@define INT_CART         0x2000 ;;the cart can actually generate an interupt

;;some (one) added by GbaGuy.

@define INT_ENABLE 0x1
@define STAT_VBLANK 8
@define STAT_HBLANK 16

;;register define added by GbaGuy. :)

@define REG_INTADDR 0x3007FFC

;;other register defines taken from gba.h in DevKitAdv C GBA devkit.

@define REG_IE       0x4000200
@define REG_IF       0x4000202
@define REG_IME      0x4000208
@define REG_DISPSTAT 0x4000004

;; macros by GbaGuy

@macro Interrupts_Enable
ldr r1,=REG_IME
ldr r2,=0x1  ; these 3 lines do step 1: interrupt master enable
str r2,[r1]
@endm

@macro ReturnFromHandler Register
ldr r1,=REG_IF   ; these 5 lines are required to
ldr r2,[r1]           ; to orr the right bit for the interrupt
mov r3,Register   ;ldr r3,=int_define   ; handled to tell the GBA that we
orr r2,r2,r3        ; are done handling the interrupt and
str r2,[r1]     ; it can go back to doing what it was doing for now.
@endm


#
# first.mak
#
# a minimal makefile (and I do mean minimal, look to the others
# for real-life use)

PROJ= sc2gba
#CC= arm-elf-gcc
#OBJCOPY= arm-elf-objcopy

CC= gcc
OBJCOPY= objcopy

SRC=C:\gba\sc2gba

IFLAGS  = -I$(SRC)/gfx -I$(SRC) -I$(SRC)/sound

CFLAGS = $(IFLAGS) 

#CFLAGS= $(IFLAGS) -mthumb-interwork 

SOURCES = sc.cpp \
	bg.cpp \
	ships.cpp \
	status.cpp \
	fire.cpp \
	special.cpp \
	set.cpp \
	melee.cpp \
	choose.cpp \
	restore.cpp \
	pkunk.cpp \
	urquan.cpp \
	yehat.cpp \
	sfx.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.o .cpp:
.cpp.o:
	$(CC) $(CFLAGS) -c $<
    
.PHONY : build
build : 
	$(CC) $(CFLAGS) -c $(SOURCES) 
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROJ).elf -lm
	$(OBJCOPY) -v -O binary $(PROJ).elf $(PROJ).gba
	#gbafix $(PROJ).gba

.PHONY : clean
clean : 
	@rm -fv *.o
	@rm -fv $(PROJ).gba
	@rm -fv $(PROJ).elf

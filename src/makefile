CC= gcc # use GNU C++ compiler 
OBJCOPY= objcopy
SRC=/gba/sc2gba
IFLAGS  = -I$(SRC)/gfx -I$(SRC) -I$(SRC)/sound -I$(SRC)/letters -I$(SRC)/pilots
CFLAGS = $(IFLAGS)   
LINKER=gcc # use GNU C++ as linker  
LINKER-FLAGS=-o            # flags for linker 
RM=rm -f # how to remove files 
MAKE=make # name of make utility 
# you cannot add comments after the continuation character! 
CPPSOURCES = sc.cpp \
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
	ilwrath.cpp \
	sfx.cpp \
	cyborg.cpp

CSOURCES = pkunk_sfx.c \
	yehat_sfx.c \
	ilwrath_sfx.c \
	urquan_sfx.c \
	
COBJECTS = $(CSOURCES:.c=.o)
CPPOBJECTS = $(CPPSOURCES:.cpp=.o)
PROJ=sc2gba# name of executable 
.IGNORE: # ignore problems (as far as possible) 
.SUFFIXES: # get rid of predefined rules 
.SUFFIXES: .cpp .o
.SUFFIXES: .c .o

$(PROJ): $(COBJECTS) $(CPPOBJECTS)
	$(CC) $(CFLAGS) $(COBJECTS) $(CPPOBJECTS) -o $(PROJ).elf -lm
	$(OBJCOPY) -v -O binary $(PROJ).elf $(PROJ).gba 

.c.o: 
	$(CC) $(CFLAGS) -c $< 
.cpp.o: 
	$(CC) $(CFLAGS) -c $< 

# remove object files and core (if any) 
clean: 
	$(RM) $(CPPOBJECTS) core 

# remove object files, core dump, and executable (if any) 
distclean: 
	$(RM) *.o core  
	$(RM) $(TARGET) 

# remove object files, core dump, and executable (if any) and 
# make them again. 
again: 
	$(MAKE) distclean 
	 $(MAKE) $(TARGET) 
	 
#dependancies
$(CPPOBJECTS): sc.h


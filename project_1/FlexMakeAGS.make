#  Makefile: Editing of Tim Budd's "Makefile" by Bina Ramamurthy and 
#  Ken Regan, Fall 1996, and again by KWR in Spring 2000.
#  Tailored for LinkMain.make by KWR for CSE250, Fall 2009.
#  Edited by AGS for CSE250, Fall2010, Project 1
#  USAGE---enter at command prompt in directory:  make -f LinkMain.make


# Unix Macros---choose which compiler and options here!

CC     = g++ -Wall     #optional change to: g++ -Wall
# CC     = g++ -fhandle-exceptions -frtti
# CC     = CC     # name of the Sun C++ compiler---NOT available on timberlake
                  # if you have a command-line installation at home (as
                  # opposed to an IDE like Eclipse), put your compiler here.
# CC = gfilt

LIBS   = -lm
OBJ    = .o
RM     = rm -fr


# SYNTAX: Dependencies must be on same line as ":".  Actions must
# follow on succeeding line(s), and have at least one TAB indent.
# Hence, be careful mouse-copying makefile text if tabs convert to spaces.

all:	FlexClientAGS

# Invoke this via "make -f LinkMain.make clean".  No "-" before "clean"!
clean:
	$(RM) *$(OBJ)    # ISR/*$(OBJ)---etc. for any subdirectories

# Below I hard-coded ".o" in place of the variable "$(OBJ)" to save room.

StringWrap.o: StringWrap.cpp StringWrap.h  #lives in base directory
	$(CC) -c StringWrap.cpp

# LinkTemplate.o: LinkTemplate.cpp LinkTemplate.h   ## NO! with g++

FlexClientAGS.o: FlexClientAGS.cpp FlexArrayAGS.h StringWrap.h 
	$(CC) -c FlexClientAGS.cpp

FlexClientAGS: FlexClientAGS.o StringWrap.o
	$(CC) -o FlexClientAGS FlexClientAGS.o StringWrap.o $(LIBS)

# The .o file with main should come before all the other object files in the
# final linking stage.


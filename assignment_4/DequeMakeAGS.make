#  Tailored for DequeMakeAGS.cpp for CSE250, Fall 2010 - Assignment 4.
#
#  USAGE---enter at command prompt in directory:  make -f DequeMakeAGS.make


# Unix Macros---choose which compiler and options here!

CC     = g++      #optional change to: g++ -Wall
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

all:	DequeClientAGS

# Invoke this via "make -f DequeMakeAGS.make clean".  No "-" before "clean"!
clean:
	$(RM) *$(OBJ)    # ISR/*$(OBJ)---etc. for any subdirectories

StringWrap.o: StringWrap.cpp StringWrap.h
	$(CC) -c StringWrap.cpp

DequeClientAGS.o: DequeClientAGS.cpp DequeAGS.h StringWrap.h
	$(CC) -c DequeClientAGS.cpp

DequeClientAGS: DequeClientAGS.o StringWrap.o
	$(CC) -o DequeClientAGS DequeClientAGS.o StringWrap.o $(LIBS)

# The .o file with main should come before all the other object files in the
# final linking stage.


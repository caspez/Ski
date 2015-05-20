################################################################################
 # Ski
 # File				: Makefile
 # Description		: Makefile for Skier app
 ################################################################################

CFLAGS = -Wall 
#Define libraries
LIB = -g -lglut -lGL -lGLU -lm ./lib/libSOIL.a

#Define C source files
SRC = ski_main.c ski_display.c ski_input.c ski_skier.c ski_slope.c ski_view.c ski_utility.c ski_billboards.c

#Define C object files
#replaces each .c extension with .o from source files
OBJ = $(SRC:.c=.o)

#Define header files
HEAD = $(SRC:.c=.h)

#Define executable file name
EXE = ski

#Link and compile object files
$(EXE): $(OBJ) ski_main.h
	gcc -o $(EXE) $(OBJ) $(CFLAGS) $(LIB)

#Compile each source file into object
.c.o: $(SRC) $(HEAD)
	gcc $(CFLAGS) -c $< -o $@

#archive directory
archive:
	zip ski-$(EXE).zip *

#clean all program files in directory
clean:
	rm -rf *.o core $(EXE)

#clean all files in directory
cleanAll:	
	rm -rf *.o *.zip core ski *.errs .*.*.swp $(EXE)

#open all files in vim tabs to edit
edit:
	vim -p $(SRC) $(SRC:.c=.h)

clearscreen:
	clear

.PHONY: c
c: clearscreen clean $(EXE)

block: blockOnRamp.C
	gcc $(LIB) blockOnRamp.c -o block 

pmotion: projectile-motion.C
	gcc $(LIB) projectile-motion.C -o pmotion 

run: ski
	./ski

.PHONY: r
r: $(EXE) run
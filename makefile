#Written by squidKnight
#Last modified: 03/15/20
#really only needed for the simulator (but would be nice to include in final prorgam)
#based off of these examples: www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
#To use in the simulator, type "make mouseSim.x" in the "Build Command" space

COMP = gcc #compiler to be used
FLAGS = -O3 #optimization flag

OBJ = main.o \
	  API.o \
	  algo.o \
	  nodeCheck.o \
	  getID.o \

%.o : %.c
	$(COMP) ${FLAGS} -c -o $@ $<

mouseSim.x : ${OBJ} #creates the needed executable file
	$(COMP) -o $@ $^

clean : #removes all object files and executable
	rm -f mouseSim.x ${OBJ}

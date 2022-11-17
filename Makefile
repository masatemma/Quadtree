# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# Define libraries to be linked (for example -lm)
LDLIBS = dict4

# Define set of implementation source files and object files
SRC = dict4.c dict3.c data.c quadtree.c listops.c
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)

# Define the executable name(s)
EXE =  dict3 dict4

# the first target
dict3: dict3.o quadtree.o data.o listops.o
	gcc -o dict3 dict3.o quadtree.o data.o listops.o

dict4: dict4.o quadtree.o data.o listops.o
	gcc -o dict4 dict4.o quadtree.o data.o listops.o

dict3.o: dict3.c data.h quadtree.h listops.h
	gcc -c dict3.c data.h quadtree.h listops.h

dict4.o: dict4.c data.h quadtree.h listops.h
	gcc -c dict4.c data.h quadtree.h listops.h

quadtree.o: quadtree.c quadtree.h data.h listops.h
	gcc -c quadtree.c

data.o: data.c data.h  
	gcc -c data.c 

clean:
	rm -f $(OBJ) $(EXE)

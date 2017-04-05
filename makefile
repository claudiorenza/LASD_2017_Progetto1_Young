# the compiler to use
CC      = gcc


# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CCFLAGS = -g -Wall
RM      = rm -rf

SRC = num.c \
	random.c \
	io.c \
	tableau_minHeap.c \
	tableau.c

OBJ = $(SRC:.c=.o)

default: all

all: $(OBJ)
	$(CC) $(CCFLAGS) $(SRC) -lm -o tableauHeap main.c
	@echo "Build complete"

c99: $(OBJ)
	$(CC) -std=c99 $(CCFLAGS) $(SRC) -lm -o tableauHeap main.c
	@echo "Build complete"
	
clean:
	$(RM) *.dSYM *.o tableauHeap
	@echo "Clean complete"

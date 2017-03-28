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
	talbeau_minHeap.c \
	tableau.c

OBJ = $(SRC:.c=.o)

default: all

all: $(OBJ)
	$(CC) $(CCFLAGS) $(SRC) -o matrixHeap main.c
	@echo "Build complete"
	
clean:
	$(RM) *.dSYM *.o matrixHeap
	@echo "Clean complete"

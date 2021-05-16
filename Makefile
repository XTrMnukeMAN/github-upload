.PHONY: all clean maxclean

CC=gcc
CFLAGS=-Wextra -Wall -pedantic-errors -Werror -std=c99
EXEC=main
OBJ=block.o main.o

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

block.o: block.c
main.o: main.c


clean:
	rm -rf *.o $(EXEC) *.dot *.pdf

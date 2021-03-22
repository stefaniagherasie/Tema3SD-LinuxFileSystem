
CC = gcc
CFLAGS = -Wall -lm -Wunused-variable
SRC = tema3.c
EXE = tema3
 
build:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

run: build
	./$(EXE)
.PHONY: clean
clean :
	rm -f $(EXE) *~
